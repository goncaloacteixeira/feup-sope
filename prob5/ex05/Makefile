CC = gcc
CFLAGS = -Wall
DEPS = utils.h
OBJ = utils.o
TARGETS = ex05

all: ex05

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ex05: $(OBJ)
	@echo "Compiling $@ ..."
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo "Successfully compiled!"
	@echo "Run with ./$@"

clean:
	@rm *.o $(TARGETS)
