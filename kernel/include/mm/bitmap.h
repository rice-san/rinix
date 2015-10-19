#ifndef _MM_BITMAP_H
#define _MM_BITMAP_H

#include <stdint.h>

#include <rinix/multiboot.h>

int create_bitmap(uintptr_t, multiboot_info_t*);
int free_frame(uintptr_t);
uintptr_t get_frame(void);

typedef struct pfbitmap{
    uint32_t* ptr;          // ptr to array used as: bitmap.ptr[i] when (bit/32) is i
    uint64_t length;        // length of array
    uint64_t last_checked;  // last checked BIT in bitmap, used to speed up free bit check;
} pf_bitmap_t;

pf_bitmap_t kernel_bitmap;

#endif