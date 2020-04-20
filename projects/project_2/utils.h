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

/* i, pid, tid, dur, pl */
typedef struct {
    int i;
    pid_t pid;
    pid_t tid;
    int dur;
    int pl;
} message_t ;

typedef struct {
    int seconds;
    char* server_fifo;
} client_args_t;



/* inst ; i ; pid ; tid ; dur ; pl ; oper */
void log_message(pid_t tid, int i, int dur, int pl, char* oper);

client_args_t parse_client_args(int argc, char** argv);

#endif //PROJECT_2_UTILS_H
