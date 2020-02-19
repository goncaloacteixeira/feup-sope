#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER  256

void print_array(char** array, int size) {
  for (int i = 0; i < size; i++) {
    printf("%s\n", array[i]);
  }
}

int main(void) {
  char buf[MAX_BUFFER];
  printf("input command: ");
  fgets(buf, MAX_BUFFER, stdin);

  char* token = strtok(buf, "|;");

  char* arr[20];

  int i = 0;
  while (token != NULL) {
    arr[i] = (char*)malloc(256*sizeof(char));
    arr[i] = token;
    i++;
    token = strtok(NULL, "|;");
  }

  print_array(arr, i);

  return 0;
}
