#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_BUFFER  256

typedef struct {
  char* command;
  char* commands[20];
} command_t;

int parse_commands(char* command, command_t* commands);

void parse_string(char* string, char** arr);

char *trim(char *s);

void print_array(char** array, int size);

void exec_cmd(command_t cmd);
