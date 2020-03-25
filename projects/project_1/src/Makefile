CC = gcc
CFLAGS = -Wall
DEPS = utils.h wrappers.h invokers.h
OBJ = utils.o wrappers.o invokers.o
TARGETS = simpledu

all: simpledu

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

simpledu: $(OBJ)
	@echo "Compiling $@ ..."
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo "Successfully compiled!"
	@echo "Run with ./$@"

clean:
	@rm *.o $(TARGETS)
