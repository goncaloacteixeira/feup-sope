CC = gcc
CFLAGS = -Wall
DEPS = utils.h wrappers.h invokers.h
OBJ = utils.o wrappers.o invokers.o
TARGETS = simpledu

all: simpledu

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

simpledu: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

clean:
	@rm *.o $(TARGETS)
