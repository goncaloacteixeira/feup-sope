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

  printf("-l: %d\n", arguments.count_links);
  printf("path: %s\n", arguments.dir);
  printf("-a: %d\n", arguments.all);
  printf("-b: %d\n", arguments.bytes);
  printf("-B: %d\n", arguments.block_size);
  printf("-L: %d\n", arguments.dereference);
  printf("-S: %d\n", arguments.separate_dirs);
  printf("--max_depth: %d\n", arguments.max_depth);

  Exit(0);
}
