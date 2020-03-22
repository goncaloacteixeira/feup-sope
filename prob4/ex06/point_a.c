#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int delay = 0;
pid_t pid;
int alarmflag = 0;

void sig_handler(int signo) {
  if (signo == SIGCHLD) {
    int status;
    pid_t pid;
    pid = wait(&status);
    printf("Child %d terminated within %d seconds.\n", pid, delay);
  }
  if (signo == SIGALRM) {
    printf("Prog exceeded time limit!\n");
    kill(pid, SIGKILL);
    alarmflag = 1;
  }
}

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    printf("Usage: %s <time> ./prog <message>\n", argv[0]);
    exit(0);
  }

  struct sigaction action;
  action.sa_handler = sig_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  if (sigaction(SIGCHLD, &action, NULL) < 0) {
    fprintf(stderr, "Error on sigaction handler\n");
    exit(1);
  }

  if (sigaction(SIGALRM, &action, NULL) < 0) {
    fprintf(stderr, "Error on sigaction handler\n");
    exit(2);
  }

  pid = fork();

  if (pid == 0)
      execl(argv[2], argv[2], argv[3], NULL);

  else {
      sscanf(argv[1], "%d", &delay); /* read delay from command line */
      alarm(delay);
      while (!alarmflag) pause();
  }
  exit(0);
}
