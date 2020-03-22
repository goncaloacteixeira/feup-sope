#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

int v = 0;

void sigusr_handler(int signo) {
  if (signo == SIGUSR1)
    v++;
  else if (signo == SIGUSR2)
    v--;
}

int main(void) {
  srand(time(0));  // random every run
  pid_t pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Error fork\n");
    exit(1);
  }
  if (pid == 0) {  // child-process
    struct sigaction action;
    action.sa_handler = sigusr_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGUSR1,&action,NULL) < 0) {
      fprintf(stderr,"Unable to install SIGUSR1 handler\n");
      exit(2);
    }

    if (sigaction(SIGUSR2,&action,NULL) < 0) {
      fprintf(stderr,"Unable to install SIGUSR2 handler\n");
      exit(2);
    }

    for(;;) {
      pause();
      printf("v: %d\n", v);
    }
  }
  else {   // parent-process
    printf("x");
    for (int i = 0; i < 50; i++) {
      (rand() % 2 == 0) ? kill(pid, SIGUSR1) : kill(pid, SIGUSR2);
      sleep(1);
    }
    kill(pid, SIGTERM);
  }
  exit(0);
}
