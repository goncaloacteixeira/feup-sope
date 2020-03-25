#include <stdlib.h>
#include <stdio.h>

void logReg(FILE* log, char* message) {
  fprintf(log, "%s\n" ,message);
}

int main(int argc, char const *argv[]) {

  FILE* logFile = fopen("output.txt", "w");
  logReg(logFile, "Hello There");

  return 0;
}
