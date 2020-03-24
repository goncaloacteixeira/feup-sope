#include "utils.h"

void print_array(char** array, int size) {
  for (int i = 0; i < size; i++) {
    printf("%s\n", array[i]);
  }
}


char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s) {
    return rtrim(ltrim(s));
}


void parse_string(char* string, char** arr) {
  char* token;
  int i = 0;
  while ((token = strtok_r(string, " ", &string))) {
    arr[i] = (char*) malloc (256*sizeof(char));
    arr[i] = token;
    i++;
  }
  arr[i] = NULL;
}


int parse_commands(char* command, command_t* commands) {
  char* token;
  int i = 0;
  while ((token = strtok_r(command, "|", &command))) {
    parse_string(trim(token), commands[i].commands);
    commands[i].command = commands[i].commands[0];
    i++;
  }

  return i;
}

void exec_cmd(command_t cmd) {
    execvp(cmd.command, cmd.commands);
}
