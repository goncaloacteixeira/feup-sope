#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ  0
#define WRITE 1

/**
*   A little util. function to print lines on the stdout
*/
void readlines(char *str) {
  char *p, *temp;
  p = strtok_r(str, "\n", &temp);
  do {
      printf("%s\n", p);
  } while ((p = strtok_r(NULL, "\n", &temp)) != NULL);
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <file.txt>\n", argv[0]);
    exit(1);
  }

  int fd[2];
  pipe(fd);

  switch (fork()) {
    case 0:
      close(fd[READ]);
      dup2(fd[WRITE], STDOUT_FILENO);  // redirect stdout
      execlp("sort", "sort", argv[1], NULL);
      fprintf(stderr, "Exec failed\n");
    default:
      close(fd[WRITE]);
      char buffer[1024];
      read(fd[READ], buffer, 1024*sizeof(char));
      readlines(buffer);
      close(fd[READ]);
  }
  return 0;
}
