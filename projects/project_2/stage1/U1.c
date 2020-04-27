#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "utils.h"

int server;
struct timespec start;


void* thr_function(void* arg) {
    pid_t tid;
    tid = syscall(SYS_gettid);  /* pode ser detetado erro com clang mas compila sem erros */
    ((message_t*) arg)->tid = tid;
    ((message_t*) arg)->pid = getpid();

    char client_fifo[64];
    sprintf(client_fifo, "/tmp/%d.%d", getpid(), tid);

    if (mkfifo(client_fifo, 0660) != 0) {
        printf("Error on mkfifo\n");
        exit(1);
    }
    int client = open(client_fifo, O_RDONLY | O_NONBLOCK);

    write(server, (message_t *) arg, sizeof(message_t));
    log_message(((message_t*) arg)->id, ((message_t*) arg)->pid, ((message_t*) arg)->tid, ((message_t*) arg)->dur, ((message_t*) arg)->pl, "IWANT");

    /*
     * TODO - Se já não for possivel obter uma resposta do servidor emitir o código "FAILD"
     *      Possivelmente: Determinar se o fifo ainda está aberto em cada itereação do while
     */

    message_t reply;
    while (read(client, &reply, sizeof(message_t)) <= 0) {
        usleep(10000);  /* enquanto não tiver uma resposta do servidor */
    }
    
    log_message(reply.id, getpid(), tid, reply.dur, reply.pl, (reply.pl != -1) ? "IAMIN" : "CLOSD");

    close(client);
    unlink(client_fifo);

    return NULL;
}


int main(int argc, char** argv) {
    if (argc != 4) {
        printf("--- CLIENT 1 ---\n");
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
        /* a duração do pedido do cliente para utilizar 
        a casa de banho será um valor entre 1 e 5 milisegundos */

        /* o tempo aqui fica em ms para ser mais simples de verificar se o cliente ainda
         * tem tempo para aceder ao servidor */
        request.dur = (rand() % (5 - 1 + 1)) + 1;
        request.id = request_id++;
        request.pl = -1;

        pthread_create(&tid, NULL, thr_function, &request);
        usleep(100000);
    }

    exit(0);
}
