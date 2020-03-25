#include "wrappers.h"

void Exit(int status) {
  char message[30];
  sprintf(message, "EXIT - %d", status);
  logReg(message);
  exit(status);
}
