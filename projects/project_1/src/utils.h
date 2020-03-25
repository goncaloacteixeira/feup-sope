#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <fcntl.h>
#include <signal.h>

extern FILE* logFile;
extern struct timespec start;
extern pid_t pid;


void logReg(pid_t pid, char* message);

pid_t Fork(void);

int Wexitstatus(pid_t pid, int status);

void handler(int sig);
