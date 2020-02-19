#include <stdio.h>


void print_usage() {
    printf("  Usage: \n");
    printf("\t./point_a <Name1> [<Name2> <Name3> ...]\n");
}


int main(int argc, char* argv[]) {
    if (argc == 1) {
        print_usage();
        return 1;
    }

    printf("Hello ");

    for (int i = 1; i < argc; i++) {
        printf("%s ",argv[i]);
    }
    printf("!\n");
    return 0;
}