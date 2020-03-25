#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

#include "utils.h"

FILE* logFile;
struct timespec start;
pid_t pid;

int main(int argc, char const *argv[]) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  logFile = fopen(getenv("LOG_FILENAME"), "w");

  struct sigaction action;
  // prepare the 'sigaction struct'
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  // install the handler
  sigaction(SIGINT, &action, NULL);

  pid = Fork();

  if (pid == 0) {
    printf("I am child with PID: %d\n", getpid());
    printf("CHILD: WORKING ...\n");
    sleep(5);
    printf("CHILD: WORK DONE\n");
    exit(1);
  }

  int status;
  waitpid(pid, &status, 0);

  printf("I am father with PID: %d\n", getpid());
  printf("PARENT: WORKING ...\n");
  sleep(5);
  printf("PARENT: WORK DONE\n");


  if ( WIFEXITED(status) ) {
      int exit_status = Wexitstatus(pid, status);
      printf("\nThe exit of child process was: %d\n", exit_status);
  }

  exit(0);

  return 0;
}
