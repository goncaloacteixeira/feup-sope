#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>


#include "invokers.h"

arguments_t arguments;
struct timespec start;
FILE* logFile;
line_t lines[MAX_LINES];
int line_no = 0;
char* directory;


int main(int argc, char *argv[]) {
  arguments = parse_arguments(argc, argv);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  logFile = fopen(getenv("LOG_FILENAME"), "w");

  recursive_read(arguments.dir);
  print_lines(lines, line_no);
  Exit(0);
}
