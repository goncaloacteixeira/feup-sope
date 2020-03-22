#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Usage %s <message>\n", argv[0]);
    exit(1);
  }

  for (int i = 0; i < 6; i++) {
    printf("%s\n", argv[1]);
    sleep(5);
  }
  exit(0);
}
