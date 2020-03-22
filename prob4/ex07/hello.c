#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

int main(int argc, char const *argv[]) {
  printf("Going to do some hello's!\n");
  for (int i = 0; i < 20; i++) {
    printf("Hello There!\n");
    sleep(3);
  }
  exit(0);
}
