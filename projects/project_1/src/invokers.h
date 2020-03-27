#include <dirent.h>
#include <sys/stat.h>
#include "wrappers.h"

#define READ        0
#define WRITE       1

extern arguments_t arguments;
extern int fd[2];

int recursive_read(char * dir);

int fork_read(char* name, int level);

int fork_read2(char* path, int level);
