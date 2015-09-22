#ifndef _MM_MEM_H
#define _MM_MEM_H

#include <stdint.h>

extern kernel_end;

uintptr_t pd_address;
uintptr_t *pd;

void page_init(uintptr_t);
uintptr_t page_phys(uintptr_t);
uintptr_t page_virt(uintptr_t);

uintptr_t get_pgt_entry(uintptr_t);
uintptr_t get_pmt_entry(uintptr_t);
uintptr_t get_pt_entry(uintptr_t);

void set_pgt_entry(uintptr_t, uintptr_t);
void set_pmt_entry(uintptr_t, uintptr_t);
void set_pt_entry(uintptr_t, uintptr_t);

#endif
