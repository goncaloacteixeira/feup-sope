#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <string.h>

#define BUFFER_SIZE 512
#define MAX_STUDENTS 100

int main(int argc, char *argv[])
{
    int fd1, nr, nw;
    unsigned char buffer[BUFFER_SIZE];
    if (argc != 2) {
        printf("Usage: %s <source>\n", argv[0]);
        return 1;
    }

    fd1 = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (fd1 == -1) {
        perror(argv[1]);
        return 2;
    }

    int i = 1;
    write(STDOUT_FILENO, "Name (^C to exit): ", 19);
    while ((nr = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) {
        if (i % 2 == 0) {
            write(STDOUT_FILENO, "Name (^C to exit): ", 19);
        } else {
            write(STDOUT_FILENO, "Grade: ", 7);
        }
        if ((nw = write(fd1, buffer, nr)) <= 0 || nw != nr) {
            perror(argv[2]);
            close(fd1);
            return 4;
        }
        i++;
    }

    close(fd1);
    return 0;
}
