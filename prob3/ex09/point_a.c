#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>


int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;

    if (argc != 2) {
        printf("usage: %s dirname\n",argv[0]);
        exit(1);
    }

    pid = fork();

    if (pid > 0) {
        printf("My child is going to execute command \"ls -laR %s\"\n", argv[1]);
    }
    else if (pid == 0) {
        char* cmd = "ls";
        char* arg[4];
        arg[0] = "ls";
        arg[1] = "-laR";
        arg[2] = argv[1];
        arg[3] = NULL; 

        execvp(cmd, arg);
        printf("Command not executed !\n");
        exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    if ( WIFEXITED(status) ) {
        int exit_status = WEXITSTATUS(status);
        printf("\nThe exit of child process was: %d\n", exit_status);
    }

    exit(0);
} 