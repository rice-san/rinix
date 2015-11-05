#include <arch/page.h>
#include <mm/mem.h>
/*
 * page_init initialises the page table for the kernel once it's past
 * booting. (boot.s)
 * It takes one argument, 'address' that points to the page directory
 * (or other top level directory).
 * 
 * */

static inline void _flush_tlb(unsigned long addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void page_init(uintptr_t address)
{
	pd_address = address;
	pd = (uaddr_t *)pd_address;
	page_arch_init();
	set_pgt_entry(0xFFC00000, ( ((unsigned int)*pd) | 0x03 ));
	pg = (uaddr_t *)0xFFFFF000;
}

// TODO: Set Page Middle And Page Global Tables

uintptr_t get_pgt_entry(uintptr_t virt)
{
	uaddr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGDT_BITS));
	
	pg = (uaddr_t *)pd_address;
	
	return pg[pg_select];
}

uintptr_t get_pmt_entry(uintptr_t virt)
{
	uaddr_t* pm;
	
	uaddr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGDT_BITS));
	uaddr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGDT_BITS - _PGMT_BITS));
	
	pd = (uaddr_t *)pd_address;
	
	#if (_PGMT_BITS != 0)
		pm = (uaddr_t *)pd[pg_select];
		return pm[pm_select];
	#else
		return 0;
	#endif
	
}

uintptr_t get_pt_entry(uintptr_t virt)
{
	uaddr_t* pm;
	uaddr_t* pt;
	virt = (virt & (~_PGOFFSET_BITS));
	
	uaddr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGDT_BITS));
	uaddr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGDT_BITS - _PGMT_BITS));
	uaddr_t pt_select = ((virt & _PGLT_MASK) >> (ADDR_BITS - _PGDT_BITS - _PGMT_BITS - _PGLT_BITS));
	
	pd = (uaddr_t *)pd_address;
	
	#if (_PGMT_BITS != 0)
		pm = (uaddr_t *)pd[pg_select];
		pt = (uaddr_t *)pm[pm_select];
	#else
		pt = (uaddr_t *)pd[pg_select];
	#endif
	
	return pt[pt_select];
}



void set_pgt_entry(uintptr_t virt, uintptr_t value)
{
	uaddr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGDT_BITS));
	
	pd = (uaddr_t *)pd_address;
	
	pd[pg_select] = value;
	_flush_tlb(virt);
}

void set_pmt_entry(uintptr_t virt, uintptr_t value)
{
	uaddr_t* pm;
	
	uaddr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGDT_BITS));
	uaddr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGDT_BITS - _PGMT_BITS));
	
	
	pd = (uaddr_t *)pd_address;
	
	#if (_PGMT_BITS != 0)
		pm = (uaddr_t *)pd[pg_select];
		pm[pm_select] = value;
	#endif
	_flush_tlb(virt);
}

void set_pt_entry(uintptr_t virt, uintptr_t value)
{
	uaddr_t* pm;
	uaddr_t* pt;
	
	virt = (virt & (~_PGOFFSET_BITS));
	
	uaddr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGDT_BITS));
	uaddr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGDT_BITS - _PGMT_BITS));
	uaddr_t pt_select = ((virt & _PGLT_MASK) >> (ADDR_BITS - _PGDT_BITS - _PGMT_BITS - _PGLT_BITS));
	
	pd = (uaddr_t *)pd_address;
	
	#if (_PGMT_BITS != 0)
		pm = (uaddr_t *)pd[pg_select];
		pt = (uaddr_t *)pm[pm_select];
	#else
		pt = (uaddr_t *)pd[pg_select];
	#endif
	pt[pt_select] = value;
	_flush_tlb(virt);
}
