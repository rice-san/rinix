#include <stdint.h>

#include <arch/info.h>
#include <arch/page.h>

#include <mm/bitmap.h>
#include <mm/mem.h>

#include <rinix/debug.h>
/*
#include <arch/multiboot.h>
#include <arch/multiboot_stub.h>
*/
#include <mm/mmap.h>

#define HIGH_BIT (1 << (8 - 1))
#define UNMAP_KERNEL(x) (((unsigned long) (x)) - 0xC0000000)

uint64_t _find_free_frame(void);
uint64_t _get_free_frame(void);

inline void _mark_frame_used(uint64_t chunk, uint8_t bit)
{
	(kernel_bitmap.ptr[chunk] |= (HIGH_BIT >> bit));
}

inline void _mark_frame_free(uint64_t chunk, uint8_t bit)
{
	(kernel_bitmap.ptr[chunk] &= ~(HIGH_BIT >> bit));
}


// mm/bitmap.c - Define Kernel Bitmap Interaction

// _free_frame() - Free the specified page frame, specified by bit
int _free_frame(uint64_t frame_num)
{
	uint64_t index = frame_num / 8;
	kernel_bitmap.last_checked = index;
	int i = frame_num % 8;
	if ((kernel_bitmap.ptr[index] & (HIGH_BIT >> i)) > 0)
	{
		_mark_frame_free(index, i);
		return 0;
	}
	else
	{
		return -1;
	}
}

// free_frame() - Actually free the frame, by physical address
int free_frame(uintptr_t pf_addr)
{
	return _free_frame(pf_addr / 0x1000);
}


// _find_free_frame() - Find the nearest free frame, and return the number of the bit with a zero
uint64_t _find_free_frame(void)
{
	uint8_t pf_chunk = 0;
	uint64_t index = kernel_bitmap.last_checked / 8;
	uint8_t found = 0;
	while (found == 0)
	{
		pf_chunk = (kernel_bitmap.ptr[index] ^ 0xFFFFFFFF);
		int i = 0;
		while(pf_chunk > 0)
		{
			if ((pf_chunk & (HIGH_BIT >> i)) > 0)
			{
				return (index * 8) + i;
			}
			i++;
		}
		index++;
	}
}

// _get_free_frame() - Return first free page frame AND mark as used
uint64_t _get_free_frame(void)
{
	uint8_t pf_chunk = 0;
	uint64_t index = kernel_bitmap.last_checked / 8;
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
				return (uint64_t)(index * 8) + i;
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


int create_bitmap(uintptr_t loc, uint64_t size)
{
    kernel_bitmap.ptr = loc;
    kernel_bitmap.length = size;
    kernel_bitmap.last_checked = 0;
    // Populate Memory Map
    int success = 0;
    int section_start = 0;
    int section_size = 0;
    int section_end = 0;
		int mmap_region = 0;
	printd("Memory Map Info: \n==================\n");
	uint32_t i = 0;
	while(mmap.regions[mmap_region].size > 0) {
		if(mmap.regions[mmap_region].type == MMAP_USABLE)
		{
			_debug( term_setcolor( make_color(COLOR_GREEN, COLOR_BLACK)));
		}
		else
		{
			_debug( term_setcolor( make_color(COLOR_RED, COLOR_BLACK)));
		}
		section_start = (unsigned long)mmap.regions[mmap_region].start;
		section_size = (unsigned long)mmap.regions[mmap_region].size;
		section_end = (unsigned long)mmap.regions[mmap_region].start + mmap.regions[mmap_region].size - 1;
		unsigned long bitmap_sec_start = section_start/0x1000;
		unsigned long bitmap_sec_length = section_size/0x1000;
		unsigned long bitmap_sec_end = section_end/0x1000;
		printd("Memory Map: start=%x length=%x value=%x\n", section_start, section_size, mmap.regions[mmap_region].type);
		_debug( pause(); );
		if((section_start % 0x1000) > 0)
		{
			printd("Warning: Section not well divided. \n");
			pause();
		}
		i = bitmap_sec_start;
		while(i <= ((bitmap_sec_end) ))
		{
			if(mmap.regions[mmap_region].type == MMAP_USABLE)
			{
				if ((kernel_bitmap.ptr[i/8] >> (ADDR_BITS - i)) & 1) {
					kernel_bitmap.ptr[i/8] &= (~(HIGH_BIT >> i % 8)); //Set to O (free)
				}
			}
			else
			{
				kernel_bitmap.ptr[i/8] |= (HIGH_BIT >> i % 8); //Set to 1 (used)
			}
			i++;
		}
		mmap_region++;
		_debug( term_setcolor( make_color(COLOR_LIGHT_GREY, COLOR_BLACK)));
	}
	// Start marking used pages

	// Mark memory below 1MB as used, 1MB = 256 * 4K
	// Since the first 1MB is a very even size, we can cheat and use memset
	memset(kernel_bitmap.ptr, ~(0), (size_t)((0x100000/0x1000)/8) );

	// Mark kernel memory as used
	for(unsigned int i = UNMAP_KERNEL(&kernel_start); i <= UNMAP_KERNEL(&kernel_end); i +=0x1000)
	{
		_mark_frame_used((i/0x1000)/8, (i/0x1000)%8);
	}

	// Mark this bitmap as used
	for(unsigned int i = kernel_bitmap.ptr; i <= (kernel_bitmap.ptr + kernel_bitmap.length); i +=0x1000)
	{
		_mark_frame_used((i/0x1000)/8, (i/0x1000)%8);
	}

	return success;
}
