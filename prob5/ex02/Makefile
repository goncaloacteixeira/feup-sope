CC = gcc
CFLAGS = -Wall
DEPS = calculations.h
OBJ = calculations.o
TARGETS = ex02

all: ex02

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ex02: $(OBJ)
	@echo "Compiling $@ ..."
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo "Successfully compiled!"
	@echo "Run with ./$@"

clean:
	@rm *.o $(TARGETS)
