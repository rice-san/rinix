#include <arch/page.h>
#include <mm/mem.h>

// page.c - Paging for the i386 architecture



/*
 * page_init initialises the page table for the kernel once it's past
 * booting. (boot.s)
 * It takes one argument, 'address' that points to the page directory
 * (or other top level directory).
 *
 * */

#define PAGE_DIR_BASE_VADDR 0xFFC00000 // This is the base virtual address for page mappings
#define PAGE_DIR_VADDR 0xFFFFF000 // This is the virtual address for editing PDE entries

static inline void _flush_tlb(unsigned long addr)
{
   asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}

void _swap_pd(uintptr_t phys_addr)
{
	set_pd(phys_addr);
}

void page_init(uintptr_t pd)
{
	/*
	_swap_pd(pd);
	((uintptr_t *)0xFFFFF000)[0] = 0;
	uintptr_t map_area_start = 0xC0000000;
	uintptr_t map_area_size = kernel_length + kernel_bitmap.length + 0x1000;
	uintptr_t map_area_end = map_area_start + map_area_size;
	uintptr_t* temp_kernel_pt = 0xC0400000;
	uintptr_t page_dir = create_page_dir(temp_kernel_pt);
	for (int i = map_area_start; i<= map_area_end; i += 0x1000){
		pte_t this_pte = (i - map_area_start) | 3;
		pte_t* this_pt = (uaddr_t)(page_dir + (i+0x1000)) & ~(0xFFF);
		if (page_dir[i >> 22] & 1) // If the page table is present
		{
			// Well... that's... just strange...
		}
		else
		{
			// This is the likely scenario
			page_dir[i >> 22] = create_page_table((uintptr_t)this_pt | 3 ); // Create a new page table with proper flags
			this_pt[(i >> 12) & 0x3FF] = this_pte; // Set this page entry to the correct value
		}
	}
	*/
}

uintptr_t create_page_dir(uintptr_t* temp_addr)
{
	uaddr_t* page_frame = get_frame(); // Grab a new page frame
	map_page(temp_addr, page_frame, 3); // Set a page table so we can use this
	memset(temp_addr, 0, (size_t)0x1000); // Fill in all the initial values of the table to zero.
	temp_addr[1023] = (0xFFC00000 | 3); // Set up a recursive mapping for the table
	
	_flush_tlb(temp_addr);
	return (uintptr_t)page_frame;
}

uintptr_t create_page_table(uintptr_t* temp_addr)
{
	uaddr_t* page_frame = get_frame(); // Grab a new page frame
	map_page(temp_addr, page_frame, 3); // Set a page table so we can use this
	memset(temp_addr, 0, (size_t)0x1000); // Fill in all the initial values of the table to zero.
	
	_flush_tlb(temp_addr);
	return (uintptr_t)page_frame;
}

void map_page(uintptr_t virt, uintptr_t phys, uint32_t flags)
{
	uint32_t pde_num = (virt >> 22);
	uaddr_t* pde = (0xFFFFF000 | (pde_num << 12));
	if( *pde & 0x1 )
	{
		// The page table is present, so keep going.
		set_page(virt, phys, flags);
	}
	else
	{
		// The page table isn't present, so let's make one.
		alloc_page_table(virt, flags);
		set_page(virt, phys, flags);
	}
}

// Set the value of a page directory entry or a page table
void set_page_table(uintptr_t virt_addr, uintptr_t phys_addr, uint32_t flags)
{
	virt_addr &= ~(0xFFF); // Page-align addresses
	phys_addr &= ~(0xFFF); // Page-align addresses
	uaddr_t* pde = 0xFFFFF000 | (virt_addr >> 22); // Get page directory entry
	*pde = phys_addr | flags;
	_flush_tlb(virt_addr);
}

void alloc_page_table(uintptr_t virt_addr, uint32_t flags)
{
	uintptr_t phys_addr = get_frame(); // Grab a page frame
	set_page_table(virt_addr, phys_addr, flags); // Set the page frame accordingly
}

void dealloc_page_table(uintptr_t virt_addr)
{
	virt_addr &= ~(0xFFF); // Page-align addresses
	uintptr_t* this_pt = (uintptr_t *) (0xFFFFF000 | (virt_addr >> 22)); // Select address for the page table
	uintptr_t phys_addr = *this_pt; // Set a pointer for this page table's physical address
	free_frame(phys_addr); // Pass the physical address so that it can be freed.
	*this_pt = 0; // Set our deceased page table to empty
}

void set_page(uintptr_t virt_addr, uintptr_t phys_addr, uint32_t flags)
{
	virt_addr &= ~(0xFFF); // Page-align addresses
	phys_addr &= ~(0xFFF); // Page-align addresses
	uintptr_t* pt = ((uintptr_t *)0xFFFFF000)[virt_addr >> 22];
	uint32_t pte = ((virt_addr >> 10) & 0x3FF);
	pt[pte] = (phys_addr | flags);
}

void alloc_page(uintptr_t virt_addr, uint32_t flags)
{
	uintptr_t phys = get_frame();
	set_page(virt_addr, phys, flags);
}

void dealloc_page(uintptr_t virt_addr)
{
	virt_addr &= ~(0xFFF); // Page-align addresses
	uaddr_t* pt = ((uintptr_t *)0xFFFFF000)[virt_addr >> 22];
	uint32_t pte = ((virt_addr >> 10) & 0x3FF);
	uaddr_t phys_addr = pt[pte];
	phys_addr &= ~(0xFFF);
	free_frame(phys_addr);
	pt[pte] = (uaddr_t)0;
}

void edit_pde(uintptr_t* pd, uintptr_t virt, uintptr_t phys, uintptr_t flags)
{
	pd[virt >> 22] = ( (phys & ~(0xFFF)) | flags );
}

void edit_pte(uintptr_t* pt, uintptr_t virt, uintptr_t phys, uintptr_t flags)
{
	pt[(virt >> 12) & 0x3FF] = ( (phys & ~(0xFFF)) | flags );
}