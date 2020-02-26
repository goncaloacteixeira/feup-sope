#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    int fd1, nr;
    unsigned char buffer[BUFFER_SIZE];
    if (argc != 2) {
        printf("Usage: %s <source>\n", argv[0]);
        return 1;
    }

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        perror(argv[1]);
        return 2;
    }

    // STDIN_FILENO - Macro para o Standard Output
    while ((nr = read(fd1, buffer, BUFFER_SIZE)) > 0) {
        write(STDIN_FILENO, buffer, nr);
    }

    write(STDIN_FILENO, "\n",1);
    
    close(fd1);
    return 0;
}