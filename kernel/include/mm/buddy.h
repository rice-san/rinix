#ifndef _MM_BUDDY_H
#define _MM_BUDDY_H

#include <arch/info.h>
#include <arch/multiboot.h>

#define BUDDY_MAX_ORDER 7
// buddy.h - Buddy Allocator

typedef uint32_t buddy_t; 

void buddy_build(uintptr_t);

#endif
