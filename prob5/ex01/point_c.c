#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#include "calculations.h"

#define READ  0
#define WRITE 1


int main(int argc, char const *argv[]) {
  int fd[2];
  pid_t pid;

  pipe(fd);
  pid = fork();

  if (pid > 0) {  // parent-process
    char aux[50];
    printf("PARENT:\n");
    printf("x y: ");
    fgets(aux, 50, stdin);

    int a[2];
    sscanf(aux, "%d %d", &a[0], &a[1]);

    close(fd[READ]);
    write(fd[WRITE], a, 2*sizeof(int));
    close(fd[WRITE]);
  }
  else if (pid == 0) {
    int b[2];
    close(fd[WRITE]);
    read(fd[READ], b, 2*sizeof(int));
    printf("SON:\n");
    printf("x + y = %f\n", sum(b[0], b[1]));
    printf("x * y = %f\n", multiplication(b[0], b[1]));
    printf("x / y = %f\n", division(b[0], b[1]));
    printf("x - y = %f\n", subtraction(b[0], b[1]));
    close(fd[READ]);
  }

  return 0;
}
