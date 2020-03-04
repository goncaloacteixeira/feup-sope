#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(void) {
   write(STDOUT_FILENO,"1",1);

   int i;
   i = fork();

   if (i > 0) {    // processo-pai
     write(STDOUT_FILENO,"2",1);
     write(STDOUT_FILENO,"3",1);
   }
   if (i == 0) {  // processo-filho
     write(STDOUT_FILENO,"4",1);
     write(STDOUT_FILENO,"5",1);
   }

   write(STDOUT_FILENO,"\n",1);
   return 0;
}
