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

typedef struct {
    int id;
    pid_t pid;
    int tid;
    int dur;
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

/* -t <nsecs> <fifoname> */
server_args_t parse_server_args(char** argv);

#endif //PROJECT_2_UTILS_H
