#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

#include "utils.h"

int server;

void* thr_function(void* arg) {
    pid_t tid;
    tid = syscall(SYS_gettid);  /* pode ser detetado erro com clang mas compila sem erros */

    write(server, (request_t*) arg, sizeof(request_t));
    log_message((int) tid, ((request_t*) arg)->id, ((request_t*) arg)->dur, -1, "IWANT");
    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("--- CLIENT 1 ---\n");
        printf("Usage: %s <-t nsec> <fifoname>\n", argv[0]);
        exit(1);
    }
    client_args_t args = parse_client_args(argc, argv);

    do {
        server = open(args.server_fifo, O_WRONLY);
        if (server == -1) {
            printf("Connecting to server ...\n");
            sleep(1);
        }
    } while(server == -1);

    long int timeout = args.seconds * 1000 * 1000;
    long int time = 0;
    int request_id = 0;

    while (time < timeout) {
        pthread_t tid;
        request_t request;
        request.dur = 10;
        request.id = request_id++;

        pthread_create(&tid, NULL, thr_function, &request);
        usleep(100000);
        time += 100000;
    }

    exit(0);
}
