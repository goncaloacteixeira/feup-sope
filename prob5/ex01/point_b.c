#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "calculations.h"

#define READ  0
#define WRITE 1

typedef struct {
  int a;
  int b;
} numbers;

int main(int argc, char const *argv[]) {
  int fd[2];
  pid_t pid;

  pipe(fd);
  pid = fork();

  if (pid > 0) {  // parent-process
    numbers numbers;
    printf("PARENT:\n");
    printf("x y: ");
    scanf("%d %d", &numbers.a, &numbers.b);
    close(fd[READ]);
    write(fd[WRITE], &numbers, sizeof(numbers));
    close(fd[WRITE]);
  }
  else if (pid == 0) {
    numbers numbers;
    close(fd[WRITE]);
    read(fd[READ], &numbers, 2*sizeof(int));
    printf("SON:\n");
    printf("x + y = %f\n", sum(numbers.a, numbers.b));
    printf("x * y = %f\n", multiplication(numbers.a, numbers.b));
    printf("x / y = %f\n", division(numbers.a, numbers.b));
    printf("x - y = %f\n", subtraction(numbers.a, numbers.b));
    close(fd[READ]);
  }

  return 0;
}
