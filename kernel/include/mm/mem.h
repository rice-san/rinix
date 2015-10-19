#ifndef _MM_MEM_H
#define _MM_MEM_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <mm/bitmap.h>

#include <rinix/debug.h>
#include <rinix/multiboot.h>
#include <rinix/vga.h>

extern kernel_start;
extern kernel_end;

uintptr_t pd_address;
uintptr_t *pd;
size_t memory_length;

void mem_init(multiboot_info_t*);	// Go from nothing to bitmap to buddy allocator

void page_init(uintptr_t);	// Initialize paging
uintptr_t page_phys(uintptr_t);
uintptr_t page_virt(uintptr_t);

uintptr_t get_pgt_entry(uintptr_t);
uintptr_t get_pmt_entry(uintptr_t);
uintptr_t get_pt_entry(uintptr_t);

void set_pgt_entry(uintptr_t, uintptr_t);
void set_pmt_entry(uintptr_t, uintptr_t);
void set_pt_entry(uintptr_t, uintptr_t);

#endif
