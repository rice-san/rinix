#ifndef _MM_MEM_H
#define _MM_MEM_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arch/page.h>

#include <mm/bitmap.h>

#include <rinix/debug.h>
#include <rinix/multiboot.h>
#include <rinix/vga.h>

extern unsigned int kernel_start;
extern unsigned int kernel_end;
unsigned int kernel_length;


uintptr_t pd_address;
uintptr_t *pd;
uaddr_t *pg;
size_t memory_length;

void mem_init(multiboot_info_t*);	// Go from nothing to bitmap to buddy allocator

void page_init(uintptr_t);	// Initialize paging
uintptr_t page_phys(uintptr_t);
uintptr_t page_virt(uintptr_t);

uintptr_t create_page_dir(uintptr_t*);
uintptr_t create_page_table(uintptr_t*);
void map_page(uintptr_t, uintptr_t, uint32_t);

void set_pd(uintptr_t);

#endif
