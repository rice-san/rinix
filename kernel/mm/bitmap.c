#include <stdint.h>

#include <arch/info.h>
#include <arch/page.h>

#include <mm/bitmap.h>
#include <mm/mem.h>

#include <rinix/debug.h>
#include <rinix/multiboot.h>

#define HIGH_BIT (1 << (ADDR_BITS - 1))
#define UNMAP_KERNEL(x) ((unsigned long) (x) - 0xC0000000)

uint64_t _find_free_frame(void);
uint64_t _get_free_frame(void);

inline void _mark_frame_used(uint64_t chunk, uint32_t bit)
{
	(kernel_bitmap.ptr[chunk] |= (HIGH_BIT >> bit));
}

inline void _mark_frame_free(uint64_t chunk, uint32_t bit)
{
	(kernel_bitmap.ptr[chunk] &= ~(HIGH_BIT >> bit));
}
// mm/bitmap.c - Define Kernel Bitmap Interaction

// _free_frame() - Free the specified page frame, specified by bit


// _find_free_frame() - Find the nearest free frame, and return the number of the bit with a zero
uint64_t _find_free_frame(void)
{
	uint32_t pf_chunk = 0;
	uint64_t index = kernel_bitmap.last_checked / 32;
	uint8_t found = 0;
	while (found == 0)
	{
		pf_chunk = (kernel_bitmap.ptr[index] ^ 0xFFFFFFFF);
		int i = 0;
		while(pf_chunk > 0)
		{
			if ((pf_chunk & (HIGH_BIT >> i)) > 0)
			{
				return (index * 32) + i;
			}
			i++;
		}
		index++;
	}
}

// _get_free_frame() - Return first free page frame AND mark as used
uint64_t _get_free_frame(void)
{
	uint32_t pf_chunk = 0;
	uint64_t index = kernel_bitmap.last_checked / 32;
	uint8_t found = 0;
	while (found == 0)
	{
		pf_chunk = (kernel_bitmap.ptr[index] ^ (~0));
		int i = 0;
		while(pf_chunk > 0)
		{
			if ((pf_chunk & (HIGH_BIT >> i)) > 0)
			{
				_mark_frame_used(index, i);
				found = 1;
				return (uint64_t)(index * 32) + i;
			}
			i++;
		}
		index++;
	}
	if (found == 0)
	{
		return 0;
	}
}

uintptr_t get_frame(void)
{
	return _get_free_frame() * 0x1000;
}


int create_bitmap(uintptr_t loc, uint64_t size, multiboot_info_t* mbd)
{
    kernel_bitmap.ptr = loc;
    kernel_bitmap.length = size;
    kernel_bitmap.last_checked = 0;
    // Populate Memory Map
    int success = 0;
    int section_start = 0;
    int section_size = 0;
    int section_end = 0;
	multiboot_memory_map_t* mmap = mbd->mmap_addr;
	printd("Memory Map Info: \n==================\n");
	uint32_t i = 0;
	while(mmap < mbd->mmap_addr + mbd->mmap_length) {
		if(mmap->type == 1)
		{
			_debug( term_setcolor( make_color(COLOR_GREEN, COLOR_BLACK))); 
		}
		else
		{
			_debug( term_setcolor( make_color(COLOR_RED, COLOR_BLACK)));
		}
		section_start = (unsigned long)mmap->addr;
		section_size = (unsigned long)mmap->len;
		section_end = (unsigned long)mmap->addr + mmap->len - 1;
		unsigned long bitmap_sec_start = section_start/0x1000;
		unsigned long bitmap_sec_length = section_size/0x1000;
		unsigned long bitmap_sec_end = section_end/0x1000;
		printd("Memory Map: start=%x length=%x size=%x value=%x end=%x\n", section_start, section_size, (unsigned long)mmap->size, mmap->type, section_end);
		_debug( pause(); );
		if((section_start % 0x1000) > 0)
		{
			printd("Warning: Section not well divided. \n");
			pause();
		}
		i = bitmap_sec_start;
		while(i <= ((bitmap_sec_end) ))
		{
			if(mmap->type == 1)
			{
				if ((kernel_bitmap.ptr[i/32] >> (ADDR_BITS - i)) & 1) {
					kernel_bitmap.ptr[i/32] &= (~(HIGH_BIT >> i % 32)); //Set to O (free)
				}
			}
			else
			{
				kernel_bitmap.ptr[i/32] |= (HIGH_BIT >> i % 32); //Set to 1 (used)
			}
			i++;
		}
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
		_debug( term_setcolor( make_color(COLOR_LIGHT_GREY, COLOR_BLACK)));
	}
	// Start marking used pages
	
	// Mark memory below 1MB as used, 1MB = 256 * 4K
	memset(kernel_bitmap.ptr, ~(0), (size_t)((0x100000/0x1000)/8) );
	
	// Mark kernel memory as used
	printd("%x\n", kernel_bitmap.ptr + UNMAP_KERNEL(&kernel_start)/0x1000/32);
	printd("%x\n", kernel_bitmap.ptr + UNMAP_KERNEL(&kernel_start)/0x1000/32 +(((((&kernel_end - &kernel_start)/0x1000)/8) + 1)));
	memset(kernel_bitmap.ptr + ((UNMAP_KERNEL(&kernel_start)/0x1000)/32), ~(0), (size_t)(((kernel_length/0x1000)/8) + 1));
	return success;
}
