#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <signal.h>

#include "utils.h"

int server;
struct timespec start;
char * server_path;

void* thr_function(void* arg) {
    pid_t tid;
    tid = syscall(SYS_gettid);  /* pode ser detetado erro com clang mas compila sem erros */
    ((message_t*) arg)->tid = tid;
    ((message_t*) arg)->pid = getpid();

    char client_fifo[64];
    sprintf(client_fifo, "/tmp/%d.%d", getpid(), tid);

    if (mkfifo(client_fifo, 0660) != 0) {
        perror("Failed to create fifo: ");
        exit(1);
    }
    int client = open(client_fifo, O_RDONLY | O_NONBLOCK);

    write(server, (message_t *) arg, sizeof(message_t));
    log_message(((message_t*) arg)->id, ((message_t*) arg)->pid, ((message_t*) arg)->tid, ((message_t*) arg)->dur, ((message_t*) arg)->pl, "IWANT");

    signal(SIGPIPE, SIG_IGN);
    if (access(server_path, F_OK) != -1) {
        message_t reply;
        /*
         * A variável counter representa o numero de tentativas de leitura do servidor
         * antes de considerar que o servidor já fechou. Depois de fechar será emitido
         * um sinal de SIGPIPE que vai ser ignorado, voltado para o else, e dessa forma
         * dado um output de "FAILD".
         */
        int counter = 0;
        while (read(client, &reply, sizeof(message_t)) <= 0 && counter < 5) {
            usleep(10000);
            counter++;
        }
        if (counter < 5)
            log_message(reply.id, getpid(), tid, reply.dur, reply.pl, (reply.pl != -1) ? "IAMIN" : "CLOSD");
        else
            log_message(((message_t*) arg)->id, ((message_t*) arg)->pid, ((message_t*) arg)->tid, ((message_t*) arg)->dur, ((message_t*) arg)->pl, "FAILD");
    } else {
        log_message(((message_t*) arg)->id, ((message_t*) arg)->pid, ((message_t*) arg)->tid, ((message_t*) arg)->dur, ((message_t*) arg)->pl, "FAILD");
    }

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
    server_path = args.server_fifo;

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
        pthread_detach(tid); /* detach para maior paralelismo */

        usleep(50000); /* pedidos com intervalo de 20ms */
    }

    exit(0);
}
