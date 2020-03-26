#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>


#include "invokers.h"

arguments_t arguments;
struct timespec start;
// FILE* for recursive
int logFile;
line_t lines[MAX_LINES];
int line_no = 0;
char* directory;


int main(int argc, char *argv[]) {
  arguments = parse_arguments(argc, argv);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  logFile = open(getenv("LOG_FILENAME"), O_RDWR | O_CREAT | O_TRUNC, 0666);
  fork_read(arguments.dir);
  print_lines(lines, line_no);
  Exit(0);
}
