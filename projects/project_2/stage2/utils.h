//
// Created by skidr on 20/04/2020.
//

#ifndef PROJECT_2_UTILS_H
#define PROJECT_2_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

extern struct timespec start;

typedef struct {
    /* ID do pedido -> começa em zero */
    int id;
    /* process ID no pedido atual */
    pid_t pid;
    /* thread ID no pedido atual */
    int tid;
    /* duração do acesso ao servidor (pedido pelo cliente) em microseconds (para facilitar o uso de usleep) */
    int dur;
    /* posição atribuida pelo servidor ao cliente (-1 no pedido) */
    int pl;
} message_t ;

typedef struct {
    int seconds;
    char* server_fifo;
} client_args_t;

typedef struct {
    int seconds;
    int nplaces;
    int nthreads;
    char* fifoname;
} server_args_t;

/* inst ; i ; pid ; tid ; dur ; pl ; oper */
void log_message(int i, pid_t pid, pid_t tid, int dur, int pl, char *oper);

/* -t <time> <fifoname> */
client_args_t parse_client_args(char** argv);

/* -t <nsecs> -l <nplaces> -n <nthreads> fifoname */
server_args_t parse_server_args(int argc, char **argv);

/* calculates the time (ms) since the start of the program */
double delta();

#endif //PROJECT_2_UTILS_H
