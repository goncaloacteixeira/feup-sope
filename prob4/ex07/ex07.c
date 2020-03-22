#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char const *argv[]) {
  if (argc == 1) {
    printf("Usage: %s <./prog1> [<./prog2> <./prog3> <./prog4> ...]\n", argv[0]);
    exit(1);
  }

  pid_t pid;
  int status;
  for (int i = 1; i < argc; i++) {
      pid = fork();
      if (pid == 0) {
          execl(argv[i], argv[i], NULL);
          /*
            Exec functions have no return, so if the program
            jumps to the next line is because there was some kind of error
            executing a program.
          */
          fprintf(stderr, "Error\n");
          exit(1);
      }
  }

  while((pid = wait(&status)) != -1 ){
      printf("Child with PID=%d finished with exit code %d\n", pid, WEXITSTATUS(status));
      if (WEXITSTATUS(status) != 0)
          kill(0, SIGKILL); // kills every process created by parent
  }

  exit(0);
}
