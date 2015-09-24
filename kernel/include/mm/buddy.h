#ifndef _MM_BUDDY_H
#define _MM_BUDDY_H

#include <arch/info.h>
#include <rinix/multiboot.h>

#define BUDDY_MAX_ORDER 7
// buddy.h - Buddy Allocator

typedef struct buddy {
	struct buddy* next;
	uint32_t bit;
} buddy_t;

void buddy_build(multiboot_info_t* mbd);

#endif
