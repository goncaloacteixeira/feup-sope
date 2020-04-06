#include "wrappers.h"

void Exit(int status) {
  char message[64];
  sprintf(message, "%.02f - %-8d - EXIT - %d\n", delta(), getpid(), status);
  write(logFile, message, strlen(message));
  exit(status);
}

void create(int pid) {
  char message[64];
  sprintf(message, "%.02f - %-8d - CREATE - %d\n", delta(), getpid(), pid);
  write(logFile, message, strlen(message));
}

void sendPipe(long int size) {
  char message[64];
  sprintf(message, "%.02f - %-8d - SEND_PIPE - %ld\n", delta(), getpid(), size);
  write(logFile, message, strlen(message));
}

void recievePipe(long int size) {
  char message[64];
  sprintf(message, "%.02f - %-8d - RECV_PIPE - %ld\n", delta(), getpid(), size);
  write(logFile, message, strlen(message));
}

void entry(char* path) {
  char* message;
  message = (char*) malloc (BUFFER_SIZE * 2 * sizeof(char));
  sprintf(message, "%.02f - %-8d - ENTRY - %s", delta(), getpid(), path);
  write(logFile, message, strlen(message));
  free(message);
}
