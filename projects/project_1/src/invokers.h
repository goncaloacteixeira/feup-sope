#include <dirent.h>
#include <sys/stat.h>
#include "wrappers.h"

extern arguments_t arguments;
extern line_t lines[500];
extern int line_no;

int recursive_read(char * dir);
