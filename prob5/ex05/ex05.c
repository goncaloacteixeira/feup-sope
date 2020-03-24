#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "utils.h"

#define READ  0
#define WRITE 1


int main(int argc, char *argv[]) {
    if (argc != 2) {
      printf("Program to execute a pipe command\n");
      printf("Usage: %s \"<command>\"\n", argv[0]);
      exit(1);
    }


    command_t commands[10];

    int cmds_length = parse_commands(argv[1], commands);

    int fd[2];
    pid_t pid = 0;
    int fdd = 0;

    for (int i = 0; i < cmds_length; i++) {
      pipe(fd);
      if ((pid = fork()) < 0) {
        perror("Error fork\n");
        exit(1);
      }

      else if (pid == 0) {
        dup2(fdd, STDIN_FILENO);  // redirect stdin to fdd
          if (i < cmds_length - 1) {
            dup2(fd[1], STDOUT_FILENO); // redirect stdout to fd
          }
        close(fd[0]);
        exec_cmd(commands[i]);
        exit(1);
      }

      else {
        wait(NULL); 		/* Collect childs */
        close(fd[1]);
        fdd = fd[0];
      }
    }

    exit(0);
}
