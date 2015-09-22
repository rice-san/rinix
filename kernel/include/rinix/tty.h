#ifndef _RINIX_TTY_H
#define _RINIX_TTY_H

#include <stddef.h>
#include <stdint.h>

void term_init(void);
void term_setcolor(uint8_t color);
void term_putentry(char c, uint8_t color, size_t x, size_t y);
void term_putchar(char c);
void term_write(const char* data, size_t size);
void term_writestr(const char* data);

#endif
