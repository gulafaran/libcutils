/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "memory.h"
#include "strings.h"

void str_replace(char *str, const char *val, int dest) {
  if(str == NULL || val == NULL) {
    fprintf(stderr, "str_replace: strings cant be NULL\n");
    return;
  }

  if(dest < 0) {
    fprintf(stderr, "str_replace: dest cant be a negative number\n");
    return;
  }

  if((strlen(val) + dest) > strlen(str)) {
    fprintf(stderr, "str_replace: destination longer then main string\n");
    return;
  }

  for(size_t i = 0; i < strlen(val); i++) {
    str[i + dest] = val[i];
  }
}

char *str_combine(const char *str1, const char *str2) {
  if(str1 == NULL || str2 == NULL) {
    fprintf(stderr, "str_combine: strings cant be NULL\n");
    return NULL;
  }

  char *str = malloc(strlen(str1) + strlen(str2) + 1);
  strcpy(str, str1);
  strcat(str, str2);

  return str;
}

bool str_contains(const char *str, const char *occ) {
  if(str == NULL && occ == NULL) {
    fprintf(stderr, "str_contains: strings cant be NULL\n");
    return false;
  }
  if(strstr(str, occ) != NULL) {
    return true;
  }
  return false;
}

bool str_empty(const char *str) {
  if(str == NULL) {
    return true;
  }

  if(!str && !str[0]) {
    return true;
  }

  bool retval = false;
  char *new_str = malloc(strlen(str) + 1);
  memset(new_str, '\0', strlen(str) + 1);
  str_remove_spaces(new_str, str);

  if(strlen(new_str) == 0) {
    retval = true;
  }

  safefree((void **)&new_str);
  return retval;
}

int str_number_chars(const char *str, const char *occ) {
  if(str_empty(str) || str_empty(occ)) {
    return 0;
  }

  char key = occ[0];
  int length = strlen(str);
  int occurences = 0;

  for(int i = 0; i < length; i++) {
    if(str[i] == key) {
      occurences++;
    }
  }

  return occurences;
}

void str_remove_spaces(char *restrict trim, const char *restrict untrim) {
  if(trim == NULL || untrim == NULL) {
    fprintf(stderr, "str_remove_spaces: strings cant be null\n");
    return;
  }

  while(*untrim != '\0') {
    if(!isspace(*trim)) {
      *trim = *untrim;
      trim++;
    }

  untrim++;
  }

  *trim = '\0';
}

char *str_shorten(const char *str, int length) {
  if(str_empty(str)) {
    fprintf(stderr, "str_shorten: string cant be NULL or empty\n");
    return NULL;
  }
  if(length <= 0) {
    fprintf(stderr, "str_shorten: length cant be zero or negative\n");
    return NULL;
  }

  char *ret_str = malloc((strlen(str) + 1) - length);
  int remove_size = strlen(str) - length;
  strncpy(ret_str, str, remove_size);
  ret_str[remove_size] = '\0';

  return ret_str;
}

char **str_split(char *str, const char *delim, int max_splits, int *num_splits) {
  if(str_empty(str) || delim == NULL) {
    if(num_splits != NULL) {
      *num_splits = 0;
    }
    fprintf(stderr, "str_split: string cant be NULL or empty\n");
    return NULL;
  }

  int found = 0;
  char **ret = malloc(max_splits * sizeof(char *));
  char *saveptr;
  char tok[strlen(str) + 1];
  memset(tok, 0, sizeof(tok));
  str_replace((char *)&tok, str, 0);
  char *tmp = strtok_r((char *)&tok, delim, &saveptr);

  while(tmp != NULL && found < max_splits) {
    ret[found] = strdup(tmp);
    tmp = strtok_r(NULL, delim, &saveptr);
    found++;
  }
  *num_splits = found;
  return ret;
}

bool str_startswith(const char *str, const char *prefix) {
  if(str == NULL || prefix == NULL) {
    fprintf(stderr, "str_startswith: string cant be NULL\n");
    return false;
  }

  size_t str_len = strlen(str);
  size_t p_len = strlen(prefix);

  return str_len < p_len ? false : strncmp(str, prefix, p_len) == 0;
}

char *int_to_str(int i) {
  int length = snprintf(NULL, 0, "%d", i);
  char* str = malloc(length + 1);

  snprintf(str, length + 1, "%d", i);
  return str;
}
