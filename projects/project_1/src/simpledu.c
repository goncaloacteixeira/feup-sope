#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

#include "wrappers.h"

static arguments_t arguments;
struct timespec start;
FILE* logFile;


int main(int argc, char *argv[]) {
  arguments = parse_arguments(argc, argv);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  logFile = fopen(getenv("LOG_FILENAME"), "w");
  line_t lines[MAX_LINES];


  

  Exit(1);
}
