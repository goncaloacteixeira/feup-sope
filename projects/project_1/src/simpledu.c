#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

#include "utils.h"

static arguments_t arguments;

int main(int argc, char *argv[]) {
  arguments = parse_arguments(argc, argv);

  printf("-l: %d\n", arguments.count_links);
  printf("path: %s\n", arguments.dir);
  printf("-a: %d\n", arguments.all);
  printf("-b: %d\n", arguments.bytes);
  printf("-B: %d\n", arguments.block_size);
  printf("-L: %d\n", arguments.dereference);
  printf("-S: %d\n", arguments.separate_dirs);
  printf("--max_depth: %d\n", arguments.max_depth);

  return 0;
}
