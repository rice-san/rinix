#include <arch/info.h>
#include <arch/page.h>

#include <mm/bitmap.h>
#include <mm/mem.h>

#include <rinix/debug.h>
#include <rinix/multiboot.h>

#define HIGH_BIT 1 << (ADDR_BITS - 1)

#define _MARK_FRAME_USED(chunk, bit) \
(kernel_bitmap.ptr[chunk] |= (HIGH_BIT >> bit))

#define _MARK_FRAME_FREE(chunk, bit) \
(kernel_bitmap.ptr[chunk] &= ~(HIGH_BIT >> bit))

// mm/bitmap.c - Define Kernel Bitmap Interaction

// _free_frame() - Free the specified page frame, specified by bit


// _find_free_frame() - Find the nearest free frame, and return the number of the bit with a zero
static uint64_t _find_free_frame(void)
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
			if (pf_chunk & (HIGH_BIT >> i))
			{
				return (index * 32) + i;
			}
			i++;
		}
		index++;
	}
}

// _get_free_frame() - Return first free page frame AND mark as used
static uint64_t _get_free_frame(void)
{
	uint32_t pf_chunk = 0;
	uint64_t index = kernel_bitmap.last_checked / 32;
	uint8_t found = 0;
	while (found == 0)
	{
		pf_chunk = (kernel_bitmap.ptr[index] ^ ~(0));
		int i = 0;
		while(pf_chunk > 0)
		{
			if (pf_chunk & (HIGH_BIT >> i) == 0)
			{
				_MARK_FRAME_USED(index, i);
				found = 1;
				return (index * 32) + i;
			}
			i++;
		}
		index++;
	}
}

uintptr_t get_frame(void)
{
	
	return (_find_free_frame() * 0x1000);
}


int create_bitmap(uintptr_t loc, multiboot_info_t* mbd)
{
    kernel_bitmap.ptr = loc;
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
				if (kernel_bitmap.ptr[i/32] >> (ADDR_BITS - i) & 1) {
					kernel_bitmap.ptr[i/32] &= ~(HIGH_BIT >> i % 32); //Set to O (free)
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
	return success;
}

#undef HIGH_BIT