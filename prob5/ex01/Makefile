CC = gcc
CFLAGS = -Wall -pedantic
DEPS = calculations.h
OBJ = calculations.o
TARGETS = point_a point_b point_c

all: point_a point_b point_c

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

point_a: $(OBJ)
	@echo "Compiling $@ ..."
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ)
	@echo "Successfully compiled!"
	@echo "Run with ./$@"

point_b: $(OBJ)
	@echo "Compiling $@ ..."
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ)
	@echo "Successfully compiled!"
	@echo "Run with ./$@"

point_c: $(OBJ)
	@echo "Compiling $@ ..."
	@$(CC) $(CFLAGS) -o $@ $@.c $(OBJ)
	@echo "Successfully compiled!"
	@echo "Run with ./$@"

clean:
	@rm *.o $(TARGETS)
