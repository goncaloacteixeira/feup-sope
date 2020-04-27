//
// Created by skidr on 20/04/2020.
//

#include "utils.h"

void log_message(int i, pid_t pid, pid_t tid, int dur, int pl, char *oper) {
    char* message;
    message = (char*) malloc (128 * sizeof(char));
    time_t t = time(NULL);

    sprintf(message, "%ld ; %d ; %d ; %d ; %d ; %d ; %s\n", t, i, pid, tid, dur, pl, oper);
    write(STDOUT_FILENO, message, strlen(message));
}

client_args_t parse_client_args(char **argv) {
    client_args_t result;

    result.seconds = atoi(argv[2]);
    result.server_fifo = argv[3];

    return result;
}

server_args_t parse_server_args(char **argv) {
    
    server_args_t result;
 
    result.seconds = atoi(argv[2]);
    result.nplaces = 0;
    result.nthreads = 0;
    result.fifoname = argv[3];

    return result;
}

double delta() {
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
}
