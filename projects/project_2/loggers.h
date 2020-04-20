//
// Created by skidr on 20/04/2020.
//

#ifndef PROJECT_2_LOGGERS_H
#define PROJECT_2_LOGGERS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void log_message(pid_t tid, int i, int dur, int pl, char* oper);



#endif //PROJECT_2_LOGGERS_H
