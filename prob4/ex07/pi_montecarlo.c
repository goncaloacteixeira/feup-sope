#include "stdlib.h"
#include "stdio.h"
#include "time.h"

int main() {
  float i, interval;
  float rand_x, rand_y, origin_dist, pi;
  int circle_points = 0, square_points = 0;

  interval = 5000.0;

  printf("Going to calculate pi using Monte-Carlo method\n");
  printf("Interval: %f\n", interval);

  // Initializing rand()
  srand(time(NULL));

  // Total Random numbers generated = possible x
  // values * possible y values
  for (i = 0; i < (interval * interval); i++) {

      // Randomly generated x and y values
      rand_x = (float)(rand() % (int)(interval + 1)) / interval;
      rand_y = (float)(rand() % (int)(interval + 1)) / interval;

      // Distance between (x, y) from the origin
      origin_dist = rand_x * rand_x + rand_y * rand_y;

      // Checking if (x, y) lies inside the define
      // circle with R=1
      if (origin_dist <= 1)
          circle_points++;

      // Total number of points generated
      square_points++;

      // estimated pi after this iteration
      pi = (float)(4 * circle_points) / square_points;
  }

  // Final Estimated Value
  printf("\nFinal Estimation of Pi = %f\n",pi);

  return 0;
}
