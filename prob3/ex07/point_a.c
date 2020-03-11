#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
  char prog[20];
  sprintf(prog, "%s.c", argv[1]);
  execlp("gcc", "gcc", prog, "-Wall", "-o", argv[1], NULL);
  printf("error compiling %s", prog);
  exit(1);
}
