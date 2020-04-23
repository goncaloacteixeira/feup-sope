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
    ((message_t*) arg)->tid = tid;
    ((message_t*) arg)->pid = getpid();

    char client_fifo[64];
    sprintf(client_fifo, "/tmp/%d.%d", getpid(), tid);

    if (mkfifo(client_fifo, 0660) != 0) {
        printf("Error on mkfifo\n");
        exit(1);
    }
    int client = open(client_fifo, O_RDONLY | O_NONBLOCK);

    write(server, (message_t *) arg, sizeof(message_t));
    log_message(((message_t*) arg)->id, ((message_t*) arg)->pid, ((message_t*) arg)->tid, ((message_t*) arg)->dur, ((message_t*) arg)->pl, "IWANT");


    message_t reply;
    while (read(client, &reply, sizeof(message_t)) <= 0) {
        usleep(10000);
    }

    if (reply.pl != -1)
        log_message(reply.id, getpid(), tid, reply.dur, reply.pl, "IAMIN");

    close(client);
    unlink(client_fifo);

    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("--- CLIENT 1 ---\n");
        printf("Usage: %s <-t nsec> <fifoname>\n", argv[0]);
        exit(1);
    }
    client_args_t args = parse_client_args(argv);

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
        message_t request;
        /* a duração do pedido do cliente para utilizar 
        a casa de banho será um valor entre 1s e 5s */
        request.dur = (rand() % (5000000 - 1000000 + 1)) + 1000000; 
        request.id = request_id++;
        request.pl = -1;

        pthread_create(&tid, NULL, thr_function, &request);
        sleep(1000000);    /* microsecs */
        time += 1000000;
    }

    exit(0);
}
