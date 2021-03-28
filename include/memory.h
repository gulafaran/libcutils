/*
 * --------------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <tomenglund26@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Tom Englund
 * --------------------------------------------------------------------------------
 */


#ifndef MEMORY_H__
#define MEMORY_H__

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

int getpidmemusage(int pid);
void safefree(void **pp);

#endif