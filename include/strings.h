#ifndef STRINGS_H__
#define STRINGS_H__

#include <stdbool.h>

void str_replace(char *str, const char *val, int dest);
char *str_combine(const char *str1, const char *str2);
bool str_contains(const char *str, const char *occ);
bool str_empty(const char *str);
int str_number_chars(const char *str, const char *occ);
void str_remove_spaces(char *restrict trim, const char *restrict untrim);
char *str_shorten(const char *str, int length);
char **str_split(char *str, const char *delim, int max_splits, int *num_splits);
bool str_startswith(const char *str, const char *prefix);
char *int_to_str(int i); 

#endif
