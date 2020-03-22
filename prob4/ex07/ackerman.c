#include "stdlib.h"
#include "stdio.h"


int ackerman(int m, int n) {
  if (m == 0)
    return n + 1;
  else if (m > 0 && n == 0)
    return ackerman(m - 1, 1);
  else if (m > 0 && n > 0)
    return ackerman(m - 1, ackerman(m, n-1));

  return 0;
}


int main(int argc, char const *argv[]) {
  printf("Going to run a very complex recursive algorithm ... \n");

  for (int m = 0; m < 5; m++)
    for (int n = 0; n < 5; n++)
      printf("Ackerman(%d,%d): %d\n", m,n,ackerman(m,n));

  return 0;
}
