#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include "calculations.h"

int main(int argc, char **argv) {
	double numbers[2];
	numbers[0] = 0; numbers[1] = 0;

	int fd;
	int fd2;

	do {
		fd = open("/tmp/fifo_req", O_WRONLY);
		fd2 = open("/tmp/fifo_ans", O_RDONLY);
		sleep(1);
		printf("Waiting for server response ...\n");
	} while (fd == -1 || fd2 == -1);

	do {
		printf("Numbers (separated with a space): ");
		scanf("%lf %lf", &numbers[0], &numbers[1]);
		write(fd, &numbers, 2*sizeof(double));

		if (numbers[0] == 0 && numbers[1] == 0) break;

		response_t response;
		while (read(fd2, &response, sizeof(response)) <= 0) { };

		printf("Mul: %lf\n", response.mul);
		printf("Div: %lf\n", response.div);
		printf("Sub: %lf\n", response.sub);
		printf("Sum: %lf\n", response.sum);

	} while (numbers[0] != 0 || numbers[1] != 0);

	close(fd); close(fd2);

	return 0;
}
