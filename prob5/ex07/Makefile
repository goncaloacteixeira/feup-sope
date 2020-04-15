CC = gcc
CFLAGS = -Wall
DEPS = calculations.h
OBJ = calculations.o
TARGETS = p07_client p07_server

all: p07_client p07_server

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

p07_client: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@
	
p07_server: $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ) -lm
	@echo $@

clean:
	@rm *.o $(TARGETS)