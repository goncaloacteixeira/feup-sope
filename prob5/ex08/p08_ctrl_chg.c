#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>


#define TIMEOUT   60000*1000  // 1 minute in micros
#define BUFFER    256

int main(int argc, char const *argv[]) {
  int fd;
  char message[BUFFER];

  int micros = 0;

  fd = open("/tmp/fifo_chg", O_RDONLY | O_NONBLOCK);

  while (micros < TIMEOUT) {
    if (read(fd, &message, BUFFER) > 0)
      printf("CHEGOU %s\n", message);
    usleep(500);
    micros += 500;
  }

  close(fd);
  return 0;
}
