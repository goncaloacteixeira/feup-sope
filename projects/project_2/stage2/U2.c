#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "utils.h"

int server;
struct timespec start;

void* thr_function(void* arg) {
    pthread_t tid;
    pthread_detach(tid = pthread_self());

    message_t request;
    memcpy(&request, ((message_t*) arg), sizeof(message_t));

    request.tid = tid;
    request.pid = getpid();

    char client_fifo[64];
    sprintf(client_fifo, "/tmp/%d.%ld", getpid(), tid);

    if (mkfifo(client_fifo, 0660) != 0) {
        perror("Failed to create fifo");
        exit(1);
    }

    write(server, &request, sizeof(message_t)); // TODO - lidar com erros no write (assumir um FAILD)

    /* abrir o fifo do cliente em read mode para que o servidor consiga abrir em write mode */
    int client;
    if ((client = open(client_fifo, O_RDONLY)) < 0) {
        char* error = (char*) malloc (128 * sizeof(char));
        sprintf(error, "Cannot open private FIFO id: %d for reading", request.id);
        perror(error);
        free(error);
        log_message(request.id, request.pid, request.tid, request.dur, request.pl, "FAILD");

        if (unlink(client_fifo) < 0) { perror("Unable to unlink private fifo"); }
        return NULL;
    }

    log_message(request.id, request.pid, request.tid, request.dur, request.pl, "IWANT");

    int counter = 0;
    message_t reply;
    /* o cliente tem 20 tentativas para receber uma resposta do servidor
     * (de forma a tolerar atrasos e permitir a execução com uma única thread */
    while (read(client, &reply, sizeof(message_t)) <= 0 && counter < 20) {
        usleep(1000);
        counter++;
    }
    /* caso o numero de tentativas seja excedido assume-se que um erro */
    if (counter == 20) {
        log_message(request.id, request.pid, request.tid, request.dur, request.pl, "FAILD");
        close(client);
        if (unlink(client_fifo) < 0) { perror("Error unlinking client fifo"); }
        return NULL;
    }
    /* o campo 'pl' será -1 caso o servidor não tenha tempo suficiente para processar o pedido */
    log_message(reply.id, getpid(), tid, reply.dur, reply.pl, (reply.pl == -1) ? "CLOSD" : "IAMIN");

    if (unlink(client_fifo) < 0) { perror("Error unlinking client fifo"); }
    close(client);
    return NULL;
}


int main(int argc, char** argv) {
    if (argc != 4) {
        printf("--- CLIENT 2 ---\n");
        printf("Usage: %s <-t nsec> <fifoname>\n", argv[0]);
        exit(1);
    }
    client_args_t args = parse_client_args(argv);

    do {
        server = open(args.server_fifo, O_WRONLY);
        if (server == -1) {
            printf("Connecting to server ...\n");
            sleep(1);
        }
    } while(server == -1);

    long int timeout = args.seconds * 1000;
    int request_id = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    while (delta() < timeout) {
        pthread_t tid;
        message_t request;
        /*
         * a duração do pedido do cliente para utilizar
         * a casa de banho será um valor entre 20 e 100 milissegundos
         */
        request.dur = (rand() % (100 - 20 + 1)) + 20;
        request.id = request_id++;
        request.pl = -1;

        pthread_create(&tid, NULL, thr_function, &request);

        usleep(40000); /* pedidos com intervalo de 40ms */
    }


    exit(0);
}
