#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[]) {
    
    for (int i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], "USER",4) == 0) {
            printf("Hello %s !\n", envp[i]);
        }
    }

    return 0;
}