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
#include <semaphore.h>
#include "utils.h"
#include "places.h"

static int thread_limited = 0;  /* flag que sera ativada caso o numero de threads for limitado */
static sem_t nthreads; /* semaforo que lida com o numero de threads ativas */

static int places_limited = 0;  /* flag que sera ativada caso o numero de lugares for limitado */
static places_t places; /* fila de lugares disponíveis */
static sem_t nplaces;   /* semaforo que lida com o numero de lugares preenchidos */
static pthread_mutex_t pl_mut = PTHREAD_MUTEX_INITIALIZER;

struct timespec start;
long int timeout;

void* thr_function(void* arg) {
    pthread_t tid;
    /* thread faz o detach dela mesma */
    pthread_detach(tid = pthread_self());

    message_t request;
    memcpy(&request, ((message_t*) arg), sizeof(message_t));

    /* pedido recebido */
    log_message(request.id, getpid(), tid, request.dur, request.pl, "RECVD");

    /* construir a string do caminho do fifo do cliente */
    char client_fifo[128];
    sprintf(client_fifo, "/tmp/%d.%ld", request.pid, request.tid);

    /* tentar abrir o fifo privado, caso não seja possivel emitir um GAVUP */
    int client;
    if ((client = open(client_fifo, O_WRONLY)) < 0) {
        fprintf(stderr, "Error opening private fifo with request %d\n", request.id);
        log_message(request.id, getpid(), tid, request.dur, request.pl, "GAVUP");

        if (thread_limited) { sem_post(&nthreads); }
        return NULL;
    }

    message_t reply;
    reply.id = request.id;
    reply.pid = getpid();
    reply.tid = tid;
    reply.dur = request.dur;

    /*************************** ATRIBUIR O LUGAR AO PEDIDO ********************************
     * Caso o número de lugares seja ilimitado o servidor atribui 1 a todos os pedidos.    *
     * caso o número de lugares seja limutado o servidor atribui o proximo lugar na fila.  *
     ***************************************************************************************/
    int place = 1;
    if (places_limited) {
        sem_wait(&nplaces);
        pthread_mutex_lock(&pl_mut);
        place = pop_front(&places);
        pthread_mutex_unlock(&pl_mut);
    }
    reply.pl = place;

    /* caso o tempo que quer utilizar não ultrapassa o tempo de execução
     * considerando o tempo decorrido então pode entrar */
    if (delta() + request.dur < timeout) {
        if (write(client, &reply, sizeof(message_t)) < 0) { /* caso não consiga establecer ligação com o cliente */
            fprintf(stderr, "Error to private fifo with request %d (ACCEPTED)\n", request.id);
            log_message(reply.id, reply.pid, reply.tid, reply.dur, reply.pl, "GAVUP");
            close(client); /* nao há mais comunicação com o fifo privado */
            if (thread_limited) { sem_post(&nthreads); } /* sync threads */
            return NULL;
        }
        close(client); /* nao há mais comunicação com o fifo privado */
        log_message(reply.id, getpid(), tid, reply.dur, reply.pl, "ENTER");
        /* client a utilizar o serviço do servidor */
        usleep(reply.dur * 1000);
        /* o tempo de utilização acabou */
        log_message(reply.id, getpid(), tid, reply.dur, reply.pl, "TIMUP");
    }
    /* caso contrário significa que o servidor vai fechar brevemente */
    else {
        reply.pl = -1; /* o -1 vai ser entendido pelo cliente como o encerramento do WC */
        if (write(client, &reply, sizeof(message_t)) < 0) {
            fprintf(stderr, "Error to private fifo with request %d (DENIED)\n", request.id);
            log_message(reply.id, reply.pid, reply.tid, reply.dur, reply.pl, "GAVUP");
            close(client); /* nao há mais comunicação com o fifo privado */

            if (thread_limited) { sem_post(&nthreads); } /* sync threads */
            return NULL;
        }
        log_message(reply.id, getpid(), tid, reply.dur, -1, "2LATE");
    }

    if (thread_limited) { sem_post(&nthreads); }
    if (places_limited) {
        pthread_mutex_lock(&pl_mut);
        push(&places, place);
        pthread_mutex_unlock(&pl_mut);
        sem_post(&nplaces);
    }
    /* nao há mais comunicação com o fifo privado */
    close(client);
    return NULL;
}


int main(int argc, char** argv) {
    if (argc < 4 || argc > 8) {
        printf("--- SERVER 2 ---\n");
        printf("Usage: %s <-t nsec> <-l nplaces> <-t nthreads> <fifoname>\n", argv[0]);
        exit(1);
    }
    server_args_t args = parse_server_args(argc, argv);

    if (mkfifo(args.fifoname, 0660) != 0) {
        perror("Failed to create fifo: ");
        exit(1);
    }
    int fd = open(args.fifoname, O_RDONLY | O_NONBLOCK);

    if (args.nthreads) { thread_limited = 1; }
    if (args.nplaces) { places_limited = 1; }


    /************ THREAD SYNC INIT ************/
    if (thread_limited) {
        sem_init(&nthreads, 0, args.nthreads);
    }
    if (places_limited) {
        sem_init(&nplaces, 0, args.nplaces);
        places = places_new(args.nplaces);
        fill(&places);
    }
    /******************************************/

    timeout = args.seconds * 1000;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    while (delta() < timeout) {
        message_t request;
        while (read(fd, &request, sizeof(message_t)) <= 0 && delta() < timeout) { usleep(1000); }
        /* Esta linha verifica se o tempo já passou devido ao usleep
         * assim evita ler duas vezes a mesma mensagem */
        if (delta() >= timeout) break;

        if (thread_limited) { sem_wait(&nthreads); } /* sync threads */

        pthread_t tid;
        pthread_create(&tid, NULL, thr_function, &request);
    }

    close(fd);
    if (unlink(args.fifoname) < 0) {
        perror("Error unlinking public FIFO");
    }

    exit(0);
}
