#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>

#include "utils.h"


void* thr_function(void* arg) {
    pid_t tid;
    tid = syscall(SYS_gettid);  /* pode ser detetado erro com clang mas compila sem erros */

    log_message((pid_t) tid, 0, 0, 0, (char*) arg);
    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("--- CLIENT 1 ---\n");
        printf("Usage: %s <-t nsec> <fifoname>\n", argv[0]);
        exit(1);
    }

    client_args_t args = parse_client_args(argc, argv);

    long int timeout = args.seconds * 1000 * 1000;
    long int time = 0;

    while (time < timeout) {
        pthread_t tid;
        pthread_create(&tid, NULL, thr_function, "Hi there");
        usleep(500);
        time += 500;
    }

    exit(0);
}
