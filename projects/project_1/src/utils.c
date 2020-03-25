#include "utils.h"

void logReg(pid_t pid, char* message) {
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  int delta_us = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
  fprintf(logFile, "%d - %d - %s\n", delta_us, pid, message);
}

pid_t Fork(void) {
  pid_t pid = fork();
  if (pid > 0) {
    char message[30];
    sprintf(message, "CREATE - PID%d", pid);
    logReg(getpid(), message);
  }
  return pid;
}

int Wexitstatus(pid_t pid, int status) {
  int exit_code = WEXITSTATUS(status);
  char message[30];
  sprintf(message, "EXIT - %d", exit_code);
  logReg(pid, message);
  return exit_code;
}

void handler(int sig) {
  // char message[30];
  if (sig == SIGINT) {
    printf("Got signal: SIGINT\n");
    // sprintf(message, "RECV_SIGNAL - SIGINT");
    // logReg(pid, message);
  }

}
