#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int v = 0;

void sigusr_handler(int signo) {
  if (signo == SIGUSR1)
    v++;
  else if (signo == SIGUSR2)
    v--;
}

int main(void) {
  struct sigaction action;
  action.sa_handler = sigusr_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  if (sigaction(SIGUSR1,&action,NULL) < 0) {
    fprintf(stderr,"Unable to install SIGUSR1 handler\n");
    exit(1);
  }

  if (sigaction(SIGUSR2,&action,NULL) < 0) {
    fprintf(stderr,"Unable to install SIGUSR2 handler\n");
    exit(1);
  }

  for(;;) {
    sleep(1);
    printf("v: %d\n", v);
  }
  return 0;
}
