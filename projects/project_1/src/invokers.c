#include "invokers.h"

int recursive_read(char * name) {
  DIR* dir;
  if ((dir = opendir(name)) == NULL) {
    perror(name);
    exit(2);
  }
  struct dirent *ent;

  int size = 0L;

  while ((ent = readdir(dir)) != NULL) {
    char *entry_name = ent->d_name;
    if (!strcmp(".", entry_name) || !strcmp("..", entry_name))
        continue;

    struct stat st_buf;
    if (!strcmp(".", name) || !strcmp("..", name))
      return 0;

    char path[1024];
    sprintf(path, "%s%s", name, ent->d_name);

    // Checking if we want to distinguish between symlinks and regfiles or not
    (arguments.dereference) ? stat(path, &st_buf) : lstat(path, &st_buf); // check for dereference
    // size in bytes or in block size
    // The stat, fstat, lstat gives blocks of 512B, but by defauld, du uses 1024B for block size.
    size += (arguments.bytes) ? (st_buf.st_size) : (st_buf.st_blocks * 512.0/arguments.block_size);

    if (S_ISDIR(st_buf.st_mode)) {
      char *next = (char*) malloc(strlen(name) + strlen(entry_name) + 2);
      sprintf(next, "%s%s/", name, entry_name);
      size += recursive_read(next);
      free(next);
    }
    else {
      int fileSize = (arguments.bytes) ? (st_buf.st_size) : (st_buf.st_blocks * 512.0/arguments.block_size);
      if (arguments.all)
        lines[line_no++] = newLine(fileSize, path);
    }
  }

  int dirSize = (arguments.bytes) ? size + 4096 : size; // hack
  lines[line_no++] = newLine(dirSize, name);
  closedir(dir);
  return size;
}

int fork_read(char* path) {
  /*int key = rand() % 113;
    printf("--------\n--------\n");
    printf("New function\n");
    printf("Key: %d\n", key);
    printf("path: %s\npid: %d\n", path, getpid());
    printf("--------\n--------\n");*/
    int status;
    pid_t pID = 1;
    char name[1024];
    struct stat statbuf;
    if(stat(path, &statbuf) == -1)
        return -1;

    /* if the item is a file */
    if(S_ISREG(statbuf.st_mode)){
        // printf("pID: %d   ", getpid());
        if (arguments.all)
          printf("%ld\t%s\n", statbuf.st_size, path);
        // lines[line_no++] = newLine(statbuf.st_size, path);
    }

    /* if the item is a directory */
    else if((statbuf.st_mode & S_IFMT) == S_IFDIR){
      pID = fork();

      if(pID > 0){    //parent
       //printf("Forked child with pID: %d\n", pID);
       waitpid(pID, &status, WUNTRACED);
       //printf("Killed: %d\n", pID);
      }
      else if(pID == 0){   //child
        //printf("Child: %d\n", getpid());
        DIR *dir;
        struct dirent *dp = NULL;
        if ((dir = opendir(path)) == NULL){
          printf("Cannot open %s\n", path);
          Exit(EXIT_FAILURE);
        }
        else {
          //printf("DIR: %s/\n", path);
          while((dp = readdir(dir)) != NULL){
            //printf("pID: %d key: %d dp = %s\n", getpid(), key, dp->d_name);
            if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
              continue;
            sprintf(name, "%s/%s", path, dp->d_name);
            //printf("Process: %d  Key: %d\n", getpid(), key);
            //printf("I was passed: %s\n", path);
            //printf("Calling recFork(%s)\n\n", name);
            fork_read(name);
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
     printf("%ld\t%s\n", statbuf.st_size, path);
  }

    //printf("Returning from : %d with key: %d\n", getpid(), key);

    return 0;
}
