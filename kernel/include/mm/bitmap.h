#ifndef _MM_BITMAP_H
#define _MM_BITMAP_H

#include <stdint.h>

#include <rinix/multiboot.h>

int create_bitmap(uintptr_t, multiboot_info_t*);
int free_frame(uintptr_t);
uintptr_t get_frame(void);

#endif