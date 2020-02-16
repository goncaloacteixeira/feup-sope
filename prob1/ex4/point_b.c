#include <stdio.h>
#include <stdlib.h>


void print_usage() {
    printf("  Usage: \n");
    printf("\t./point_a <Name1> [<Name2> <Name3> ...] <number of greetings>\n");
}


int main(int argc, char* argv[]) {
    if (argc <= 2) {
        print_usage();
        return 1;
    }

    for (int j = 0; j < atoi(argv[argc-1]); j++) {
        printf("Hello ");

        for (int i = 1; i < argc - 1; i++) {
            printf("%s ",argv[i]);
        }

        printf("!\n");
    }
    return 0;
}