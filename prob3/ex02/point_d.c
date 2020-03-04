#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main(void) {
   printf("1\n");

   int i;
   i = fork();

   if (i > 0) {    // processo-pai
      printf("2");
      printf("3");
   }
   if (i == 0) {  // processo-filho
     printf("4");
     printf("5");
   }

   printf("\n");
   return 0;
}
