#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

#include "invokers.h"

/****************************************************************
      Dev Option - To generate a log file set a Env. Var:
      LOG_FILENAME="path/to/desired/logfile"
*****************************************************************/

arguments_t arguments;
struct timespec start;
int logFile;
char* directory;
int fd[2];


int main(int argc, char *argv[]) {
  arguments = parse_arguments(argc, argv);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  logFile = open(getenv("LOG_FILENAME"), O_RDWR | O_CREAT | O_TRUNC, 0666);

  pipe(fd);
  fork_read(arguments.dir, 0);
  Exit(0);
}
