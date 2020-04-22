//
// Created by skidr on 22/04/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>
#include <wait.h>
#include <pthread.h>
#include "utils.h"

void* thr_function(void* arg) {
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
        request_t request;
        while (read(fd, &request, sizeof(request_t)) <= 0) { }
        printf("Got Message: %d\n", request.id);

        // pthread_t tid;
        // pthread_create(&tid, NULL, thr_function, &request);
        usleep(100000);
        time += 100000;
    }

    close(fd);
    unlink(args.fifoname);

    exit(0);
}
