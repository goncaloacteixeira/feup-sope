#include "utils.h"

/* Calculates time (in ms) since the start of the program */
int delta() {
  struct timespec end;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
}


void parse_string(char* string, char** arr, char* delim) {
  char* token;
  int i = 0;
  while ((token = strtok_r(string, delim, &string))) {
    arr[i] = (char*) malloc (256*sizeof(char));
    arr[i] = token;
    i++;
  }
  arr[i] = NULL;
}


arguments_t parse_arguments(int argc, char* argv[]) {
  arguments_t arguments;
  arguments.all = 0;
  arguments.bytes = 0;
  arguments.block_size = 1024;
  arguments.count_links = 0;
  arguments.dereference = 0;
  arguments.separate_dirs = 0;
  arguments.max_depth = INT_MAX;
  arguments.dir = "./";

  int num_flags = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp("-l", argv[i]) == 0 || strcmp("--count-links", argv[i]) == 0) {
      arguments.count_links = 1;
      num_flags++;
    }
    else if (strcmp("-a", argv[i]) == 0 || strcmp("--all", argv[i]) == 0) {
      arguments.all = 1;
      num_flags++;
    }
    else if (strcmp("-b", argv[i]) == 0 || strcmp("--bytes", argv[i]) == 0) {
      arguments.bytes = 1;
      num_flags++;
    }
    else if (strncmp("--block-size", argv[i], 12) == 0) {
      char* arr[2];
      parse_string(argv[i], arr, "=");
      arguments.block_size = atoi(arr[1]);
      num_flags++;
    }
    else if (strcmp("-B", argv[i]) == 0) {
      arguments.block_size = atoi(argv[++i]);
      num_flags++;
    }
    else if (strcmp("-L", argv[i]) == 0 || strcmp("--dereference", argv[i]) == 0) {
      arguments.dereference = 1;
      num_flags++;
    }
    else if (strcmp("-S", argv[i]) == 0 || strcmp("--separate-dirs", argv[i]) == 0) {
      arguments.separate_dirs = 1;
      num_flags++;
    }
    else if (strncmp("--max-depth", argv[i], 2) == 0) {
      char* arr[2];
      parse_string(argv[i], arr, "=");
      arguments.max_depth = atoi(arr[1]);
      num_flags++;
    }
    else if (strcmp(argv[i], "./") == 0 || strcmp(argv[i], ".") == 0 || (strcmp(argv[i], "./.")) == 0) {
      directory = argv[i];
    }
    else {
      char *path = (char*) malloc(strlen(argv[i]) + 1 + 1 ); /* one for extra char, one for trailing zero */
      strcpy(path, argv[i]);
      directory = argv[i];

       //  working for recursive version
      if (argv[i][strlen(argv[i]) - 1] != '/') {
        path[strlen(argv[i])] = '/';
        path[strlen(argv[i]) + 1] = '\0';
      }

      // for fork version
      // if (argv[i][strlen(argv[i]) - 1] == '/')
      //   path[strlen(argv[i]) - 1] = '\0';

      arguments.dir = path;
    }
  }

  /* a little hack to parse the arguments */
  if (!directory) {
    if (arguments.count_links) {
      if (argc - 1 == num_flags) {
        directory = ".";
      } else {
        directory = argv[2];
      }
    }
    else {
      directory = ".";
    }
  }

  return arguments;
}
