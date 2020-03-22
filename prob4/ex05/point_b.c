#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

int status;
pid_t pid;


void sig_handler(int signo) {
  pid = wait(&status);
  if (pid != -1)
    printf("PARENT: child with PID=%d terminated with exit code %d\n", pid, WEXITSTATUS(status));
}


int main() {
  struct sigaction action;
  action.sa_handler = sig_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;
  sigaction(SIGCHLD, &action, NULL);

  int i, n;
  for (i = 1; i <= 10; i++) {
    pid = fork();
    if (pid == 0){
      printf("CHILD no. %d (PID=%d) working ... \n", i, getpid());
      sleep(5); // child working ...
      printf("CHILD no. %d (PID=%d) exiting ... \n", i, getpid());
      exit(0);
    }
  }

  printf("PARENT: working hard ...\n");
  n = 20; while ((n=sleep(n)) != 0);
  printf("PARENT: end of task\n");


  exit(0);
}
