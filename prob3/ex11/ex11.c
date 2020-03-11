#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 256


int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;
    char buffer[BUFFER_SIZE];

    while (1) {
        pid = fork();
        if (pid > 0) {
            write(STDOUT_FILENO, "minish > ", 10);
        }
        else if (pid == 0) {
            fgets(buffer, BUFFER_SIZE, stdin);

            if (strncmp(buffer, "quit", 4) == 0) {
                exit(5);
            }

            char* res[20];
            char* p = strtok (buffer, " ");
            
            int i = 0;
            /* split string and append tokens to 'res' */
            while (p) {
                res[i++] = p;
                p = strtok (NULL, " ");
            }
            
            res[i - 1][strlen(res[i - 1]) - 1] = '\0'; 
            res[i] = NULL;
            
            execvp(res[0], res);
        }

        int status;
        waitpid(pid, &status, 0);

        if ( WIFEXITED(status) ) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status == 5) break;
        }
    }
} 