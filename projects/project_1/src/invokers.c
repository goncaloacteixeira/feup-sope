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
    stat(path, &st_buf);

    size += st_buf.st_size;

    if (S_ISDIR(st_buf.st_mode)) {
      char *next = (char*) malloc(strlen(name) + strlen(entry_name) + 2);
      sprintf(next, "%s%s/", name, entry_name);
      size += recursive_read(next);
      free(next);
    }
    else {
      lines[line_no++] = newLine(st_buf.st_size, path);
    }
  }

  lines[line_no++] = newLine(size + 4096, name);
  closedir(dir);
  return size;
}
