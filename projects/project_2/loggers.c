//
// Created by skidr on 20/04/2020.
//

#include "loggers.h"

void log_message(pid_t tid, int i, int dur, int pl, char *oper) {
    char* message;
    message = (char*) malloc (128 * sizeof(char));
    time_t t = time(NULL);

    /* inst ; i ; pid ; tid ; dur ; pl ; oper */
    sprintf(message, "%ld ; %d ; %d ; %d ; %d ; %d ; %s\n", t, i, getpid(), tid, dur, pl, oper);
    write(STDOUT_FILENO, message, strlen(message));
}
