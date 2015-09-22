#include <arch/page.h>
#include <mm/mem.h>
/*
 * page_init initialises the page table for the kernel once it's past
 * booting. (boot.s)
 * It takes one argument, 'address' that points to the page directory
 * (or other top level directory).
 * 
 * */

void page_init(uintptr_t address)
{
	pd_address = address;
	pd = (uintptr_t *)pd_address;
}

// TODO: Set Page Middle And Page Global Tables

uintptr_t get_pgt_entry(uintptr_t virt)
{
	uintptr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGMT_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	
	pd = (uintptr_t *)pd_address;
	
	return pd[pg_select];
}

uintptr_t get_pmt_entry(uintptr_t virt)
{
	uintptr_t* pm;
	
	uintptr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGMT_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	uintptr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	
	pd = (uintptr_t *)pd_address;
	
	if (pm_select != 0)
	{
		pm = (uintptr_t *)pd[pg_select];
		return pm[pm_select];
	}
	
}

uintptr_t get_pt_entry(uintptr_t virt)
{
	uintptr_t* pm;
	uintptr_t* pt;
	
	virt = (virt & (~_PGOFFSET_BITS));
	
	uintptr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGMT_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	uintptr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	uintptr_t pt_select = ((virt & _PGLT_MASK) >> (ADDR_BITS - _PGOFFSET_BITS));
	
	pd = (uintptr_t *)pd_address;
	
	if (pm_select != 0)
	{
		pm = (uintptr_t *)pd[pg_select];
		pt = (uintptr_t *)pm[pm_select];
	}
	else
	{
		pt = (uintptr_t *)pd[pg_select];
	}
	return pt[pt_select];
}



void set_pgt_entry(uintptr_t virt, uintptr_t value)
{
	uintptr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGMT_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	
	pd = (uintptr_t *)pd_address;
	
	pd[pg_select] = value;
}

void set_pmt_entry(uintptr_t virt, uintptr_t value)
{
	uintptr_t* pm;
	
	uintptr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGMT_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	uintptr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	
	pd = (uintptr_t *)pd_address;
	
	if (pm_select != 0)
	{
		pm = (uintptr_t *)pd[pg_select];
		pm[pm_select] = value;
	}
	
}

void set_pt_entry(uintptr_t virt, uintptr_t value)
{
	uintptr_t* pm;
	uintptr_t* pt;
	
	virt = (virt & (~_PGOFFSET_BITS));
	
	uintptr_t pg_select = ((virt & _PGDT_MASK) >> (ADDR_BITS - _PGMT_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	uintptr_t pm_select = ((virt & _PGMT_MASK) >> (ADDR_BITS - _PGLT_BITS - _PGOFFSET_BITS));
	uintptr_t pt_select = ((virt & _PGLT_MASK) >> (ADDR_BITS - _PGOFFSET_BITS));
	
	pd = (uintptr_t *)pd_address;
	
	if (pm_select != 0)
	{
		pm = (uintptr_t *)pd[pg_select];
		pt = (uintptr_t *)pm[pm_select];
	}
	else
	{
		pt = (uintptr_t *)pd[pg_select];
	}
	pt[pt_select] = value;
}
