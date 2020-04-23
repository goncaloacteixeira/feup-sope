//
// Created by skidr on 22/04/2020.
//
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <pthread.h>
#include "utils.h"

void* thr_function(void* arg) {
    pid_t tid;
    tid = syscall(SYS_gettid);  /* pode ser detetado erro com clang mas compila sem erros */

    message_t* request = (message_t*) arg;
    /* pedido recebido */
    log_message(request->id, getpid(), tid, request->dur, request->pl, "RECVD");

    // TODO - Verificar se o cliente pode realmente entrar no WC (good Issue)

    /* o cliente pode entrar no WC */
    log_message(request->id, getpid(), tid, request->dur, 1, "ENTER");

    /* construir a string do caminho do fifo do cliente */
    char client_fifo[64];
    sprintf(client_fifo, "/tmp/%d.%d", request->pid, request->tid);
    int client = open(client_fifo, O_WRONLY);

    message_t reply;
    reply.id = request->id;
    reply.pid = getpid();
    reply.tid = tid;
    reply.pl = 1;

    write(client, &reply, sizeof(message_t));

    // TODO - Verificar o tempo de utilização do WC:

    /* -> utilizar usleep ?
     *      usleep conta o tempo em microsecs
     *
     * -> O servidor é responsável por emitir a mensagem de TIMEUP não o cliente:
     *      será que o tempo de espera fica no lado do servidor ou no lado do cliente?
     */

    close(client);
    return NULL;
}


int main(int argc, char** argv) {
    if (argc < 4) {
        printf("--- SERVER 1 ---\n");
        printf("Usage: %s <-t nsec> <fifoname>\n", argv[0]);
        exit(1);
    }
    server_args_t args = parse_server_args(argc, argv);

    if (mkfifo(args.fifoname, 0660) != 0) {
        printf("Error on mkfifo\n");
        exit(1);
    }
    int fd = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    long int timeout = args.seconds * 1000 * 1000;
    long int time = 0;

    while (time < timeout) {
        message_t request;
        while (read(fd, &request, sizeof(message_t)) <= 0 && time < timeout) {
            usleep(10000);  /* microsecs */
            time += 10000;
        }
        pthread_t tid;
        pthread_create(&tid, NULL, thr_function, &request);
    }

    close(fd);
    unlink(args.fifoname);

    exit(0);
}
