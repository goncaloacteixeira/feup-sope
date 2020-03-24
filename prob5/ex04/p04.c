#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define READ  0
#define WRITE 1


void readlines(char *str) {
  char *p, *temp;
  p = strtok_r(str, "\n", &temp);
  do {
      printf("%s\n", p);
  } while ((p = strtok_r(NULL, "\n", &temp)) != NULL);
}


int main(int argc, char const *argv[]) {
  if (argc != 3) {
    printf("Program to execute the following command: \n");
    printf("  ls <dir> –laR | grep <arg> | sort\n");
    printf("Usage: %s <dir> <args>\n", argv[0]);
    exit(-1);
  }
  /*
    argv[1] = dir
    argv[2] = arg
  */

    int pipe1[2];
    int pipe2[2];
    pid_t pid;

    // pipe1
    if (pipe(pipe1) == -1) {
      perror("Pipe1 error\n");
      exit(1);
    }

    // fork (ls <dir> -laR)
    if ((pid = fork()) == -1) {
      perror("Fork1 error\n");
      exit(1);
    } else if (pid == 0) {
      // stdin --> ps --> pipe1
      // output from STDOUT
      dup2(pipe1[WRITE], STDOUT_FILENO);
      // close pipe
      close(pipe1[READ]);
      close(pipe1[WRITE]);
      // exec command
      execlp("ls", "ls", argv[1], "-laR", NULL);
      perror("Exec1 error\n");
      _exit(1);
    }

    // Parent process

    // pipe2
    if (pipe(pipe2) == -1) {
      perror("Pipe2 error\n");
      exit(2);
    }

    // fork (grep <arg>)
    if ((pid = fork()) == -1) {
      perror("bad fork2");
      exit(2);
    } else if (pid == 0) {
      // pipe1 --> grep --> pipe2
      // input from Pipe1
      dup2(pipe1[READ], STDIN_FILENO);
      // output to pipe2
      dup2(pipe2[WRITE], STDOUT_FILENO);
      // close pipes fds
      close(pipe1[READ]);
      close(pipe1[WRITE]);
      close(pipe2[READ]);
      close(pipe2[WRITE]);
      // exec command
      execlp("grep", "grep", argv[2], NULL);
      perror("Exec2 error\n");
      _exit(2);
    }

    // Parent process

    // close unused fds
    close(pipe1[0]);
    close(pipe1[1]);

    // fork (sort)
    if ((pid = fork()) == -1) {
      perror("bad fork3");
      exit(3);
    } else if (pid == 0) {
      // pipe2 --> grep --> stdout
      // input from Pipe2
      dup2(pipe2[READ], STDIN_FILENO);
      // output to stdout already done
      // closing fds
      close(pipe2[READ]);
      close(pipe2[WRITE]);
      // Exec command
      execlp("sort", "sort", NULL);
      perror("Exec3 error\n");
      _exit(3);
    }

    // Parent process
    exit(0);
}
