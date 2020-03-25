#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

FILE* logFile;
struct timespec start;


void logReg(char* message) {
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  int delta_us = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
  fprintf(logFile, "%d - %d - %s\n", delta_us, getpid(), message);
}

pid_t Fork(void) {
  pid_t pid = fork();
  if (pid > 0) {
    char message[30];
    sprintf(message, "CREATE - PID%d", pid);
    logReg(message);
  }
  return pid;
}

int main(int argc, char const *argv[]) {
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  logFile = fopen(getenv("LOG_FILENAME"), "w");
  sleep(5);

  if (Fork() > 0) {
    printf("I am father with PID: %d\n", getpid());
    printf("PARENT: WORKING ...\n");
    sleep(5);
    printf("PARENT: WORK DONE\n");
  }
  else {
    printf("I am child with PID: %d\n", getpid());
    printf("CHILD: WORKING ...\n");
    sleep(5);
    printf("CHILD: WORK DONE\n");
    exit(0);
  }

  exit(0);

  return 0;
}
