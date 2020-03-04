#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <wait.h>
#include <stdlib.h>

int main(void) {

   pid_t pid, wpid;
   int status = 0;
   pid = fork();

   if (pid == 0) {
     write(STDOUT_FILENO, "Hello", 5);
     exit(0);
   }
   if (pid > 0) {
     while ((wpid = wait(&status)) > 0);    // process-parent waits for child to be done
     write(STDOUT_FILENO, " world!\n", 8);
   }


   return 0;
}
