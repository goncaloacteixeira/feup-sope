#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main(void) {

   pid_t pid;
   pid = fork();

   if (pid == 0) {
     write(STDOUT_FILENO, " world!\n", 8);
   }
   if (pid > 0) {
     write(STDOUT_FILENO, "Hello", 5);
   }


   return 0;
}
