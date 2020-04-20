#include <stdio.h>

#include "utils.h"

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("--- CLIENT 1 ---\n");
        printf("Usage: %s <-t nsec> <fifoname>\n", argv[0]);
        exit(1);
    }

    client_args_t args = parse_client_args(argc, argv);


    printf("Seconds: %d\n", args.seconds);
    printf("Server: %s\n", args.server_fifo);

    exit(0);
}
