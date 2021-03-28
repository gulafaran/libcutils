/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */


#ifndef FILES_H__
#define FILES_H__

#include <stdbool.h>
#include <sys/types.h>

bool file_exists(char *path);
bool create_file_recursively(char *path, mode_t mode);
bool create_dir(char *path, mode_t mode);
bool create_file(char *name, char *mode);
int copy_file(const char *to, const char *from);
void print_file_bufsize(char *file, int pbufsize);
char **read_file(char *file, int *array_size, int *array_allocated);
void write_tofile(char *file, char *val, char *mode);

#endif
