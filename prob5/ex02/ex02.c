#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <math.h>
#include <string.h>

#include "calculations.h"

#define READ  0
#define WRITE 1

typedef struct {
  double result;
  char message[10];
} Result;

void parseResults(int* arr, Result result[4]) {
  result[0].result = sum(arr[0], arr[1]);
  result[1].result = subtraction(arr[0], arr[1]);
  result[2].result = division(arr[0], arr[1]);
  result[3].result = multiplication(arr[0], arr[1]);

  for (int i = 0; i < 4; i++) {
    if (fabsf(roundf(result[i].result) - result[i].result) <= 0.00001f) {
      strcpy(result[i].message, "Integer");
    } else {
      strcpy(result[i].message, "Float");
    }
  }

  if (fabsf(roundf(result[2].result) - INT_MAX) <= 0.00001f) {
    strcpy(result[2].message, "Invalid");
  }
}


int main(int argc, char const *argv[]) {
  int fd1[2];
  int fd2[2];
  pid_t pid;

  pipe(fd1);
  pipe(fd2);
  pid = fork();

  if (pid > 0) {  // parent-process
    close(fd1[READ]);
    close(fd2[WRITE]);

    int a[2];
    printf("PARENT:\n");
    printf("x y: ");
    scanf("%d %d", &a[0], &a[1]);
    write(fd1[WRITE], a, 2*sizeof(int));

    waitpid(pid, NULL, 0);  // waiting for child process

    Result result[4];
    read(fd2[READ], result, 4*sizeof(Result));

    printf("Results: \n");
    printf("Adition -> %f (%s)\n", result[0].result, result[0].message);
    printf("Subtra. -> %f (%s)\n", result[1].result, result[1].message);
    printf("Divisi. -> %f (%s)\n", result[2].result, result[2].message);
    printf("Multip. -> %f (%s)\n", result[3].result, result[3].message);

    close(fd1[WRITE]);
    close(fd2[READ]);
  }
  else if (pid == 0) {
    close(fd1[WRITE]);
    close(fd2[READ]);

    int b[2];
    Result result[4];

    read(fd1[READ], b, 2*sizeof(int));
    parseResults(b, result);
    write(fd2[WRITE], result, 4*sizeof(Result));

    close(fd1[READ]);
    close(fd2[WRITE]);
  }

  return 0;
}
