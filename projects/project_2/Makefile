CC = gcc
CFLAGS = -Wall -pthread
DEPS = utils.h
OBJ = utils.o
TARGETS = client_U1

all: client_U1

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

client_U1: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

clean:
	@rm *.o $(TARGETS)