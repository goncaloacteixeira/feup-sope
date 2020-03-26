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

void sendPipe(long int size) {
  char message[30];
  sprintf(message, "%d - %d - SEND_PIPE - %ld\n", delta(), getpid(), size);
  write(logFile, message, strlen(message));
}

void recievePipe(long int size) {
  char message[30];
  sprintf(message, "%d - %d - RECV_PIPE - %ld\n", delta(), getpid(), size);
  write(logFile, message, strlen(message));
}

void entry(char* path) {
  char message[50];
  sprintf(message, "%d - %d - ENTRY - %s\n", delta(), getpid(), path);
  write(logFile, message, strlen(message));
}
