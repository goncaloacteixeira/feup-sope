#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>

#define BUFFER_SIZE 512


int main(int argc, char *argv[])
{
    int fd1, fd2, nr;
    unsigned char buffer[BUFFER_SIZE];
    if (argc < 2 || argc > 3) {
        printf("Usage: %s <source> [<destination>]\n", argv[0]);
        return 1;
    }

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        perror(argv[1]);
        return 3;
    }

    fd2 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);
    
    if (argc == 3) {
        dup2(fd2, STDOUT_FILENO);
    }

    // STDIN_FILENO - Macro para o Standard Output
    while ((nr = read(fd1, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, nr);    
    }

    if (argc != 3) {
        write(STDOUT_FILENO, "\n",1);
    }
    
    close(fd1);
    return 0;
}