#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
int vprintf(const char* __restrict, va_list);
int putchar(int);
int puts(const char*);

#ifdef __cplusplus
}
#endif

#endif
