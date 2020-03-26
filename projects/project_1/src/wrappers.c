#include "wrappers.h"

void Exit(int status) {
  char message[30];
  sprintf(message, "%d - %d - EXIT - %d\n", delta(), getpid(), status);
  write(logFile, message, strlen(message));
  exit(status);
}

void create(int pid) {
  char message[30];
  sprintf(message, "%d - %d - CREATE - %d\n", delta(), getpid(), pid);
  write(logFile, message, strlen(message));
}
