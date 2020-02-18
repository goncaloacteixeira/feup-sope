#include <stdio.h>
#include <stdlib.h>

void handler1(void) {
  printf("Executing handler1\n");
}

void handler2(void) {
  printf("Executing handler2\n");
}

int main(void) {
  atexit(handler1);
  atexit(handler2);

  printf("Main done!");
  return 0;
}

/*
  Acontece que o programa envia a mensagem "Main done!"
  e só depois executa os handlers, primeiro o 2, e depois
  o 1.
*/
