#include <arch/info.h>
#include <arch/page.h>

#include <mm/bitmap.h>
#include <mm/mem.h>

#include <rinix/debug.h>
#include <rinix/multiboot.h>

#define HIGH_BIT 1 << (ADDR_BITS - 1)


// mm/bitmap.c - Define Kernel Bitmap Interaction
int create_bitmap(uintptr_t loc, multiboot_info_t* mbd)
{
    kernel_bitmap = loc;
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
				if (kernel_bitmap[i/32] >> (ADDR_BITS - i) & 1) {	
					kernel_bitmap[i/32] &= ~(HIGH_BIT >> i % 32); //Set to O
				}
			}
			else
			{
				kernel_bitmap[i/32] |= (HIGH_BIT >> i % 32); //Set to 1
			}
			i++;
		}
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
		_debug( term_setcolor( make_color(COLOR_LIGHT_GREY, COLOR_BLACK)));
	}
	return success;
}

#undef HIGH_BIT