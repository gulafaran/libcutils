/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "files.h"
#include "strings.h"
#include "memory.h"

int getpidmemusage(int pid) {
  if(pid < 0) {
    fprintf(stderr, "getpidmemusage: Invalid PID: %d\n", pid);
    return -1;
  }
  char buffer[1024] = "";
  const char *proc = "/proc/";
  const char *file = "/status";
  char *strpid = int_to_str(pid);
  int *virtmemory = malloc(sizeof(int));
  char *c1 = str_combine(proc, strpid);
  char *path = str_combine(c1, file);
  int memory = -1;

  if(!file_exists(path)) {
    fprintf(stderr, "getpidmemusage: File doesnt exist:: %s\n", path);
    goto clean;
  }

  FILE* f = fopen(path, "r");

  while (fscanf(f, " %1023s", buffer) == 1) {
    if(strcmp(buffer, "VmSize:") == 0) {
      fscanf(f, " %d", virtmemory);
      memory = *virtmemory;
    }
  }
  fclose(f);

  clean:
    safefree((void**)&strpid);
    safefree((void**)&c1);
    safefree((void**)&path);
    safefree((void**)&virtmemory);

    return memory;
}

void safefree(void **pp) {
  assert(pp);
  if(pp != NULL) {
    free(*pp);
    *pp = NULL;
  }
}