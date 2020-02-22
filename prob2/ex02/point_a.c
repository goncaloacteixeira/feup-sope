#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_LENGTH 256

void print_usage() {
    printf("  Usage: \n");
    printf("\t./<exec_name> <file1.txt> <file2.txt>\n");
}

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        print_usage();
        exit(3);
    }
    
    FILE *src, *dst;
    char buf[BUF_LENGTH];
    if ( ( src = fopen( argv[1], "r" ) ) == NULL ) {
        printf("Error %d\n", errno);
        exit(1);
    }

    if ( ( dst = fopen( argv[2], "w" ) ) == NULL ) {
        printf("Error %d\n", errno);
        exit(2);
    }

    while( ( fgets( buf, __INT_MAX__, src ) ) != NULL ) {
        fputs( buf, dst );
    }

    fclose( src );
    fclose( dst );
    exit(0); // zero é geralmente indicativo de "sucesso"
}