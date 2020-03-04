#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int global=1;

int main(void) {
   int local = 2;

   /*
      fork()

      "Faz uma espécie de clone"
      surge um processo novo, identico ao primeiro:

          |
          |
          |
         fork()
          |   \
          |    \
          |     \

      As informações são replicadas para o processo filho.

      O fork() retorna o PID do processo-filho
   */

   int i;
   i = fork();

   if (i > 0) {   // processo-pai
     printf("PID = %d; PPID = %d\n", getpid(), getppid());
     global++;
     local--;
   }
   if (i == 0) {  // processo-filho
     printf("PID = %d; PPID = %d\n", getpid(), getppid());
     global--;
     local++;
   }

   // depois de completo o if, os dois processos continuam aqui
   printf("PID = %d - global = %d ; local = %d\n", getpid(), global, local);
   return 0;
}
