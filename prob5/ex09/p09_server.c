#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>
#include <wait.h>

#define SERVER_PATH "/tmp/fifo.s"
#define MAXCOMMANDS 256

int readline(int fd, char *str) {
  int n;
  do {
      n = read(fd, str, 1);
  } while (n > 0 && *str++ != '\n');
  *(--str) = '\0';
  return (n > 0);
}

pid_t parse_commands(char string[64], char* arr[MAXCOMMANDS]) {
  int i = 0;
  pid_t pid;
  char* token = strtok( string, " " );
  while( token != NULL ) {
    if (i == 0)
      pid = atoi(token);
    else
      arr[i - 1] = token;
    i++;
    token = strtok( NULL, " " );
  }
  arr[i] = NULL;
  return pid;
}


int main(int argc, char const *argv[]) {
  mkfifo(SERVER_PATH, 0660);
  int fd = open(SERVER_PATH, O_RDONLY | O_NONBLOCK);


  while (1) {
    char message[64];
    while (readline(fd, message) == 0) { }
    printf("GOT MESSAGE: %s\n", message);

    char* command[MAXCOMMANDS];
    pid_t client_pid = parse_commands(message, command);

    pid_t pid;
    if ((pid = fork()) == 0) {  /* child */
      int filefd = open("/tmp/file.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
      close(1);
      dup(filefd);
      execvp(command[0], command);

      exit(1);
    }
    else if (pid < 0) {
      printf("Error on fork\n");
      exit(1);
    }

    int status;
    waitpid(pid, &status, 0);

    if ( WIFEXITED(status) ) {
      // int exit_status = WEXITSTATUS(status);
      // printf("\nThe exit of child process was: %d\n", exit_status);
    }

    FILE *f = fopen("/tmp/file.txt", "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

    char *string = (char*) malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);

    string[fsize] = 0;

    char fifo_client[64];
    sprintf(fifo_client, "/tmp/fifo.%d", client_pid);
    int client = open(fifo_client, O_WRONLY);
    write(client, string, strlen(string) + 1);
    close(client);
  }

  close(fd);
  unlink(SERVER_PATH);

  exit(0);
}
