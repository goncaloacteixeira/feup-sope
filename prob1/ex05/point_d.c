#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
    
    /**
     *  To add a new variable to the system we simply type:
     *      > export <variable name>=<variable data>
     *  with quotes on this case
     */ 

    const char* user = getenv("USER_NAME");
    printf("Hello %s !\n", user);

    return 0;
}