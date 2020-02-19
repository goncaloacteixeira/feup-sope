#include <stdio.h>
#include <string.h>

#define MAX_BUFFER  256

int main(void) {
  char buf[MAX_BUFFER];
  printf("input command: ");
  fgets(buf, MAX_BUFFER, stdin);

  char* token = strtok(buf, " ");

  while (token != NULL) {
    printf("%s ", token);
    token = strtok(NULL, " ");
  }
  printf("\n");

  return 0;
}
