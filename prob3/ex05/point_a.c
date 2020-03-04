#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <wait.h>
#include <stdlib.h>

int main(void) {

   pid_t pid1, pid2, wpid;
   int status;

   pid1 = fork();
   pid2 = fork();

   if (pid1 > 0 && pid2 > 0) {    // parent
     // process-parent waits for all children to be done
     while ((wpid = wait(&status)) > 0);
     printf(" friends!\n");
   }

   if (pid1 == 0 && pid2 > 0) {   // child-1
     printf("Hello");
     exit(0);
   }

   if (pid1 == 0 && pid2 == 0) {  // child-2
     printf(" my");
     exit(0);
   }

   return 0;
}
