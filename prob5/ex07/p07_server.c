#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "calculations.h"


response_t process_numbers(double numbers[2]) {
	response_t response;
	response.sum = sum(numbers[0], numbers[1]);
	response.div = division(numbers[0], numbers[1]);
	response.sub = subtraction(numbers[0], numbers[1]);
	response.mul = multiplication(numbers[0], numbers[1]);
	return response;
}

int main(int argc, char **argv) {
	int fd;
	double numbers[2];
	mkfifo("/tmp/fifo_req", 0660);
	fd = open("/tmp/fifo_req", O_RDONLY);

	int fd2;
	response_t response;
	mkfifo("/tmp/fifo_ans", 0660);
	fd2 = open("/tmp/fifo_ans", O_WRONLY);


	while (read(fd, &numbers, 2 * sizeof(double)) > 0) {
		if (numbers[0] == 0 && numbers[1] == 0) break;
		response = process_numbers(numbers);
		write(fd2, &response, sizeof(response_t));
	}

	close(fd); close(fd2);
	unlink("/tmp/fifo_req"); unlink("/tmp/fifo_ans");

	return 0;
}


