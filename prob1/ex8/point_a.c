#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_usage() {
  printf("  Generate random numbers from 0 to n1 until it finds n2\n");
  printf("  Usage: \n");
  printf("\t./<app.c> <n1> <n2>\n");
}

/*
 * Point a) and b) covered here
 */

int main(int argc, char* argv[]) {

  if (argc != 3) {
    print_usage();
    exit(1);
  }

  int n1 = atoi(argv[1]);
  int n2 = atoi(argv[2]);

  srand(time(0));

  printf("Generating number from 0 to %d until %d is found ...\n\n", n1, n2);

  int number;
  int i = 0;

  clock_t begin = clock();

  do {
    number = rand() % n1;
    printf("Iteration %d:\t%d\n", i, number);
    i++;
  } while (number != n2);

  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("\n\ntime-spent: %f", time_spent);

  /*
   * Run code with:
   *    time ./<name> <n1> <n2> to get real, usr, sys times
   */

  return 0;
}
