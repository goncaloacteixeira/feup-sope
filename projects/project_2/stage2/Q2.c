//
// Created by skidr on 22/04/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <pthread.h>
#include "utils.h"


struct timespec start;
long int timeout;

void* thr_function(void* arg) {
    pthread_t tid;
    pthread_detach(tid = pthread_self());

    message_t* request = (message_t*) arg;
    /* pedido recebido */
    log_message(request->id, getpid(), tid, request->dur, request->pl, "RECVD");

    /* construir a string do caminho do fifo do cliente */
    char client_fifo[64];
    sprintf(client_fifo, "/tmp/%d.%ld", request->pid, request->tid);
    int client = open(client_fifo, O_WRONLY);

    message_t reply;
    reply.id = request->id;
    reply.pid = getpid();
    reply.tid = tid;
    reply.dur = request->dur;

    /* caso o tempo que quer utilizar não ultrapassa o tempo de execução
     * considerando o tempo decorrido então pode entrar */
    if (delta() + request->dur < timeout) {
        reply.pl = 1; /* TODO - a mudar para um id sequencial */

        write(client, &reply, sizeof(message_t));

        log_message(reply.id, getpid(), tid, reply.dur, 1, "ENTER");

        /* client a utilizar o serviço do servidor */
        usleep(reply.dur * 1000);
        /* registar evento (time up) */
        log_message(reply.id, getpid(), tid, reply.dur, 1, "TIMUP");
    }
    /* caso contrário significa que o servidor vai fechar brevemente */
    else {
        log_message(reply.id, getpid(), tid, reply.dur, -1, "2LATE");
        reply.pl = -1; /* o -1 vai ser entendido pelo cliente como o encerramento do WC */
        write(client, &reply, sizeof(message_t));
    }

    close(client);
    return NULL;
}


int main(int argc, char** argv) {
    if (argc != 4) {
        printf("--- SERVER 1 ---\n");
        printf("Usage: %s <-t nsec> <fifoname>\n", argv[0]);
        exit(1);
    }
    server_args_t args = parse_server_args(argc, argv);

    if (mkfifo(args.fifoname, 0660) != 0) {
        perror("Failed to create fifo: ");
        exit(1);
    }
    int fd = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    timeout = args.seconds * 1000;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

     while (delta() < timeout) {
        message_t request;
        while (read(fd, &request, sizeof(message_t)) <= 0 && delta() < timeout) {
            usleep(10000);
        }

        /* Esta linha verifica se o tempo já passou devido ao usleep
         * assim evita ler duas vezes a mesma mensagem */
        if (delta() >= timeout) break;

        pthread_t tid;
        pthread_create(&tid, NULL, thr_function, &request);
     }

    close(fd);
    unlink(args.fifoname);

    exit(0);
}
