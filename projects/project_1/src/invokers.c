#include "invokers.h"

int recursive_read(char * path) {
  DIR* dir;
  if ((dir = opendir(path)) == NULL) {
    perror(path);
    exit(2);
  }
  struct dirent *ent;

  int size = 0L;

  while ((ent = readdir(dir)) != NULL) {
    char *entry_name = ent->d_name;
    if (!strcmp(".", entry_name) || !strcmp("..", entry_name))
        continue;

    struct stat st_buf;
    if (!strcmp(".", path) || !strcmp("..", path))
      return 0;

    char name[1024];
    sprintf(name, "%s%s", path, ent->d_name);

    // Checking if we want to distinguish between symlinks and regfiles or not
    (arguments.dereference) ? stat(name, &st_buf) : lstat(name, &st_buf); // check for dereference
    // size in bytes or in block size
    // The stat, fstat, lstat gives blocks of 512B, but by defauld, du uses 1024B for block size.
    size += (arguments.bytes) ? (st_buf.st_size) : (st_buf.st_blocks * 512.0/arguments.block_size);

    if (S_ISDIR(st_buf.st_mode)) {
      char *next = (char*) malloc(strlen(path) + strlen(entry_name) + 2);
      sprintf(next, "%s%s/", path, entry_name);
      size += recursive_read(next);
      free(next);
    }
    else {
      int fileSize = (arguments.bytes) ? (st_buf.st_size) : (st_buf.st_blocks * 512.0/arguments.block_size);
      if (arguments.all)
        lines[line_no++] = newLine(fileSize, name);
    }
  }

  int dirSize = (arguments.bytes) ? size + 4096 : size; // hack
  lines[line_no++] = newLine(dirSize, path);
  closedir(dir);
  return size;
}

int fork_read(char* path, int level) {
    int status;
    pid_t pID = 1;
    char name[1024];
    struct stat statbuf;
    long int dirSize = 0;

    (arguments.dereference) ? stat(path, &statbuf) : lstat(path, &statbuf); // check for dereference

    /* if the item is a file or a link */
    if (S_ISREG(statbuf.st_mode) || S_ISLNK(statbuf.st_mode)) {
        // printf("pID: %d   ", getpid());
        /* if the block size or size in bytes - 512B -> the stat block size is set to 512B */
        long int fileSize = (arguments.bytes) ? (statbuf.st_size) : (statbuf.st_blocks * 512.0/arguments.block_size);
        dirSize += fileSize;

        if (level <= arguments.max_depth && arguments.all)
          printf("%ld\t%s\n", fileSize, path);
        // lines[line_no++] = newLine(statbuf.st_size, path);
    }

    /* if the item is a directory */
    else if ((statbuf.st_mode & S_IFMT)){
      pID = fork();

      if (pID > 0) {    //parent
        create(pID);
        waitpid(pID, &status, WUNTRACED);
      }
      else if (pID == 0) {   //child
        DIR *dir;
        struct dirent *dp = NULL;
        if ((dir = opendir(path)) == NULL) {
          printf("Cannot open %s\n", path);
          Exit(EXIT_FAILURE);
        }
        else {
          while ((dp = readdir(dir)) != NULL){
            if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
              continue;
            sprintf(name, "%s/%s", path, dp->d_name);
            fork_read(name, level + 1);
          }
          // printf("%ld\t%s\n", statbuf.st_size, path);
          closedir(dir);
          Exit(0);
        }
     }
     else {   //failed to fork
       printf("Failed to fork\n");
       Exit(EXIT_FAILURE);
     }
     dirSize += (arguments.bytes) ? (statbuf.st_size) : (statbuf.st_blocks * 512.0/arguments.block_size);
     /* if the directory is in the allowed depth */
     if (level <= arguments.max_depth)
       printf("%ld\t%s\n", dirSize, path);
  }

  return 0;
}

int fork_read2(char* path, int level) {
  int status;
  pid_t pID;
  char name[1024];
  long int dirSize = 0;

  DIR* dir;
  if ((dir = opendir(path)) == NULL) {
    perror(path);
    exit(2);
  }
  struct dirent *ent;

  while ((ent = readdir(dir)) != NULL) {
    char *entry_name = ent->d_name;
    if (!strcmp(".", entry_name) || !strcmp("..", entry_name))
        continue;

    struct stat st_buf;
    if (!strcmp(".", path) || !strcmp("..", path))
      return 0;

    // printf("Name: %s\n", name);
    // Checking if we want to distinguish between symlinks and regfiles or not
    sprintf(name, "%s/%s", path, ent->d_name);
    // printf("Name: %s\n", name);
    (arguments.dereference) ? stat(name, &st_buf) : lstat(name, &st_buf); // check for dereference

    if (S_ISDIR(st_buf.st_mode)) {
      char *next = (char*) malloc(strlen(path) + strlen(entry_name) + 2);
      sprintf(next, "%s/%s", path, entry_name);

      pID = fork();

      if (pID > 0) {    //parent
        create(pID);  // log process creation
        waitpid(pID, &status, WUNTRACED);
        long int tmp;
        read(fd[READ], &tmp, sizeof(long int));
        if (!arguments.separate_dirs)   /* check for separated dir size */
          dirSize += tmp;
        // printf("Got %ld from pipe | Current size on %s: %ld\n", tmp, path, dirSize);
      }
      else if (pID == 0) {   //child
        fork_read2(next, level + 1);
        closedir(dir);
        Exit(0);
      }
      else {   //failed to fork
        printf("Failed to fork\n");
        Exit(EXIT_FAILURE);
      }
      if (!arguments.separate_dirs)   /* check for separated dir size */
        dirSize += (arguments.bytes) ? (st_buf.st_size) : (st_buf.st_blocks * 512.0/arguments.block_size);
      free(next);
    }
    else {
      long int fileSize = (arguments.bytes) ? (st_buf.st_size) : (st_buf.st_blocks * 512.0/arguments.block_size);
      dirSize += fileSize;
      if (level < arguments.max_depth && arguments.all)
        printf("%ld\t%s\n", fileSize, name);
    }
  }
  write(fd[WRITE], &dirSize, sizeof(long int));
  dirSize += (arguments.bytes) ? 4096 : 0;
  if (level <= arguments.max_depth) {
    printf("%ld\t%s\n", dirSize, path);
    // printf("Finished with %s | size: %ld\n", path, dirSize);

  }
  return dirSize;
}
