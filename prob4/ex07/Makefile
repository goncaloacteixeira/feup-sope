all: examples ex07

examples: ackerman.c pi_montecarlo.c hello.c
	gcc -Wall -o ackerman ackerman.c
	gcc -Wall -o pi_montecarlo pi_montecarlo.c
	gcc -Wall -o hello hello.c

ex07: ex07.c
	gcc -Wall -o ex07 ex07.c

clean:
	rm ex07 ackerman pi_montecarlo hello
