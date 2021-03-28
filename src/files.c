/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>

#include "files.h"
#include "strings.h"
#include "memory.h"

static const char *path_sep = "/";

bool file_exists(char *path) {
  if(str_empty(path)) {
    fprintf(stderr, "file_exist: path cant be empty\n");
    return false;
  }

  if(access(path, F_OK ) != -1) {
    // file exists
    return true;
  }

  // file doesn't exist
  return false;
}

bool create_file_recursively(char *path, mode_t mode) {
  if(path == NULL) {
    fprintf(stderr, "create_file_recursively: path cant be NULL\n");
    return false;
  }

  int max_dir_depth = str_number_chars(path, path_sep);
  int num_splits = 0;
  bool retval = true;
  char **dirs = str_split(path, path_sep, max_dir_depth, &num_splits);
  char *file_dir = str_shorten(path, (strlen(dirs[num_splits -1] +1)));

  if(num_splits <= 0) {
    retval = false;
    goto clean;
  }

  if(!file_exists(file_dir)) {
    if(!create_dir(file_dir, mode)) {
      retval = false;
      goto clean;
    }
  }

  for(int i = 0; i < num_splits; i++) {
    if(i == (num_splits -1)) {
      create_file(path, "w+"); //assume last split is file?
    }
  }

clean:
  for(int i = 0; i < num_splits; i++) {
    safefree((void **)&dirs[i]);
  }

  safefree((void **)&dirs);
  safefree((void **)&file_dir);
  return retval;
}

bool create_dir(char *path, mode_t mode) {
    assert(path && *path);
    for (char* p = strchr(path + 1, '/'); p; p = strchr(p + 1, '/')) {
        *p = '\0';
        if(mkdir(path, mode) == -1) {
            if(errno != EEXIST) {
                *p = '/';
                return false;
            }
        }
        *p = '/';
    }
    return true;
}

bool create_file(char *name, char *mode) {
  if(name == NULL || mode == NULL) {
    fprintf(stderr, "create_file: name or mode cant be NULL\n");
    return false;
  }

  FILE *f = fopen(name, mode);
  if(f == NULL) {
    fprintf(stderr, "create_file: Error creating file: %s , %s\n", name, strerror(errno));
    return false;
  }

  fclose(f);

  return true;
}

int copy_file(const char *to, const char *from) {
  int source = open(from, O_RDONLY, 0);
  int dest = open(to, O_WRONLY | O_CREAT , 0644);

    // struct required, rationale: function stat() exists also
  struct stat stat_source;
  fstat(source, &stat_source);

  sendfile(dest, source, 0, stat_source.st_size);

  close(source);
  close(dest);
  return 0;
}

void print_file_bufsize(char *file, int pbufsize) {
  if(file == NULL || pbufsize <= 0) {
    fprintf(stderr, "print_file_bufsize: file cant be NULL and pbufsize cant be 0 or negative\n");
    return;
  }

  FILE *fp;
  char buffer[pbufsize];
  fp = fopen(file, "r");

  if(!fp) {
    fprintf(stderr, "print_file_bufsize: Error while opening file: %s\n", file);
    fprintf(stderr, "%s\n", strerror(errno));
    return;
  }

  while(fgets(buffer, pbufsize, fp) != NULL) {
    fprintf(stdout, "%s", buffer);
  }

  fclose(fp);
}

char **read_file(char *file, int *array_size, int *array_allocated) {
  int lines_allocated = 15;
  int max_line_len = 100;

  /* Allocate lines of text */
  char **words = (char **)malloc(sizeof(char*)*lines_allocated);
  memset(words, 0, sizeof(char*)*lines_allocated);

  FILE *fp = fopen(file, "r");
  if(fp == NULL) {
    fprintf(stderr,"read_file: Error while opening file: %s\n", file);
    fprintf(stderr, "%s\n", strerror(errno));
    goto clean;
  }

  int i;
  for(i=0; 1; i++) {
    int j;

    /* Have we gone over our line allocation? */
    if(i >= lines_allocated) {
      int new_size;

      /* Double our allocation and re-allocate */
      new_size = lines_allocated*2;
      words = (char **)realloc(words,sizeof(char*)*new_size);
      //memset??
      lines_allocated = new_size;
    }

    /* Allocate space for the next line */
    words[i] = malloc(max_line_len);
    if(fgets(words[i],max_line_len-1,fp)==NULL) {
      break;
    }

    /* Get rid of CR or LF at end of line */
    for(j=strlen(words[i])-1; j>=0 && (words[i][j]=='\n' || words[i][j]=='\r');j--);

    words[i][j+1]='\0';
  }

  /* Close file */
  fclose(fp);
  *array_size = i;
  *array_allocated = lines_allocated;
  return words;
clean:
  safefree((void **)&words);
  return NULL;
}

void write_tofile(char *file, char *val, char *mode) {
  if(file == NULL || val == NULL) {
    fprintf(stderr, "write_tofile: file or val cant be NULL \n");
    return;
  }

  FILE *fp;
  fp = fopen(file, mode);

  if(!fp) {
    fprintf(stderr, "write_tofile: Error while opening file: %s\n", file);
    fprintf(stderr, "%s\n", strerror(errno));
  }

  fprintf(fp, "%s", val);
  fclose(fp);
}
