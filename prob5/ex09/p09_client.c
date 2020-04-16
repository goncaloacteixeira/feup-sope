#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>

#define SERVER_PATH   "/tmp/fifo.s"
#define MAXBUFFER     65536

int readlines(int fd, char *str) {
  int n;
  do {
      n = read(fd, str, 1);
  } while (n > 0 && *str++ != '\0');
  return (n > 0);
}


int main(int argc, char const *argv[]) {
  int client;
  int server;

  do {
    server = open(SERVER_PATH, O_WRONLY);
    if (server == -1) {
      printf("Connecting to server ...\n");
      sleep(1);
    }
  } while(server == -1);

  char fifo_client[MAXBUFFER];
  sprintf(fifo_client, "/tmp/fifo.%d", getpid());
  mkfifo(fifo_client, 0660);
  client = open(fifo_client, O_RDONLY | O_NONBLOCK);

  printf("Command: ");
  char command[256];
  fgets(command, 256, stdin);

  char message[256];
  sprintf(message, "%d %s", getpid(), command);

  write(server, message, strlen(message));
  close(server);

  char buffer[MAXBUFFER];
  while (!readlines(client, buffer)) { }
  printf("%s\n", buffer);
  close(client);

  unlink(fifo_client);

  exit(0);
}
