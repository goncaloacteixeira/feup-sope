#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <string.h>

#define MAX_LINES 500

extern FILE* logFile;
extern struct timespec start;
extern char* directory;

typedef struct {
  char* dir;            //!< Directory to run command
  /**
  * The information is also relative to files
  */
  int all;              //!< 1 if -a or --all
  /**
  * Prints real byte data (files) or allocated (directories)
  */
  int bytes;            //!< 1 if -b or --bytes
  /**
  * Defines the block size (byte) for representation effects
  */
  int block_size;       //!< block size (bytes) for representation
  /**
  * Count each hard link of files with multiple links
  */
  int count_links;      //!< 1 if -l or --count-links
  /**
  * Dereference symlinks that are command line arguments
  */
  int dereference;      //!< 1 if -L or --dereference
  /**
  * Do not add sizes of subdirectories
  */
  int separate_dirs;    //!< 1 if -S or --separate-dirs
  /**
  * Show the total for each directory (and file if --all) that is at
  * most MAX_DEPTH levels down from the root of the hierarchy.  The root
  * is at level 0, so `du --max-depth=0' is equivalent to `du -s'.
  */
  int max_depth;        //!< Directory max depth
} arguments_t;

typedef struct {
  int size;
  char path[1024];
} line_t;

line_t newLine(int size, char* path);

void logReg(char* message);

void parse_string(char* string, char** arr, char* delim);

arguments_t parse_arguments(int argc, char* argv[]);

void print_lines(line_t* lines, int size);
