#include <mm/mem.h>

#include <arch/info.h>
#include <arch/page.h>

#include <rinix/multiboot.h>
#include <rinix/pause.h>
#include <rinix/timer.h>



#define UNMAP_KERNEL(x) ((unsigned long) (x) - 0xC0000000)
#define ROUND_DOWN(x) (((unsigned long)(x) & ~0x0fff) - 0x1000)
#define ROUND_UP(x) (((unsigned long)(x) & ~0x0fff) + 0x1000)

#define HIGH_BIT 1 << (ADDR_BITS - 1)

int bitmap_size = 0; 


void mem_init(multiboot_info_t* mbd)
{
	unsigned long kernel_size = &kernel_end - &kernel_start;
	
	unsigned long section_start = 0;
	unsigned long section_end = 0;
	unsigned long section_size = 0;
	
	unsigned long bitmap_place;
	
	unsigned long try_bitmap_start = 0;
	unsigned long try_bitmap_end = 0;
	printd("Multiboot Info: %x\n", &mbd);
	_debug( term_setcolor(make_color(COLOR_CYAN, COLOR_BLACK)) );
	printd("Memory Map At: %x\n", mbd->mmap_addr);
	printd("Memory Map Length: %x\n", mbd->mmap_length);
	if((mbd->flags & 0x40))
	{
		if(mbd->flags & 1)
		{
			printd("Memory Length: %lK\n", ((memory_length = (mbd->mem_lower)+(mbd->mem_upper))/1024));
			bitmap_size = (memory_length / 0x1000);
			printd("Kernel Bitmap Size: %x\n", bitmap_size);
		}
		// Find place for bitmap
		multiboot_memory_map_t* mmap = mbd->mmap_addr;
		printd("Memory Map Info: \n==================\n");
		unsigned long usable_mem = 0;
		unsigned long unusable_mem = 0;
		while(mmap < mbd->mmap_addr + mbd->mmap_length) {
			if(mmap->type == 1)
			{
				usable_mem += mmap->len;
				_debug( term_setcolor( make_color(COLOR_GREEN, COLOR_BLACK))); 
			}
			else
			{
				unusable_mem += mmap->len;
				_debug( term_setcolor( make_color(COLOR_RED, COLOR_BLACK)));
			}
			printd("Memory Map: start=%x length=%x size=%x value=%x end=%x\n", (section_start = (unsigned long)mmap->addr), (section_size = (unsigned long)mmap->len), (unsigned long)mmap->size, mmap->type, (section_end = (unsigned long)mmap->addr + mmap->len));
			if( section_size > bitmap_size  && mmap->type == 1)
			{
				short int newline = 0;
				if( UNMAP_KERNEL(&kernel_start) >= section_start && UNMAP_KERNEL(&kernel_start) <= section_end)
				{
					printd("Kernel starts in this section");
					newline = 1;
					if( ROUND_UP(UNMAP_KERNEL(&kernel_end)) <= ROUND_UP(section_end) )
					{
						printd(" and ends in this section");
						if( (ROUND_UP(section_end) - ROUND_UP(UNMAP_KERNEL(&kernel_end))) >= bitmap_size)
						{
							bitmap_place = ROUND_UP(UNMAP_KERNEL(&kernel_end));
							printd("\nPut the bitmap here: %x\n", bitmap_place);
							kernel_bitmap = (uint32_t*) bitmap_place;
							break;
						}
					}
				}
				(newline > 0) ? printd("\n") : printd("");
				
			}
			mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
			_debug( term_setcolor( make_color(COLOR_LIGHT_GREY, COLOR_BLACK)));
		}
		
		// Populate Memory Map
		mmap = mbd->mmap_addr;
		printd("Memory Map Info: \n==================\n");
		usable_mem = 0;
		unusable_mem = 0;
		uint32_t i = 0;
		while(mmap < mbd->mmap_addr + mbd->mmap_length) {
			if(mmap->type == 1)
			{
				usable_mem += mmap->len;
				_debug( term_setcolor( make_color(COLOR_GREEN, COLOR_BLACK))); 
			}
			else
			{
				unusable_mem += mmap->len;
				_debug( term_setcolor( make_color(COLOR_RED, COLOR_BLACK)));
			}
			section_start = (unsigned long)mmap->addr;
			section_size = (unsigned long)mmap->len;
			section_end = (unsigned long)mmap->addr + mmap->len;
			unsigned long bitmap_sec_start = section_start/0x1000;
			unsigned long bitmap_sec_length = section_size/0x1000;
			unsigned long bitmap_sec_end = section_end/0x1000;
			printd("Memory Map: start=%x length=%x size=%x value=%x end=%x\n", section_start, section_size, (unsigned long)mmap->size, mmap->type, section_end);
			pause();
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
					printd("Bitmap Bit #: %i\n", i);
					printd("Bitmap Section: %i\n", i / 32);
					printd("Bitmap Section Bit: %i\n", i % 32);
					printd("Usable\n");
					if (kernel_bitmap[i/32] >> (ADDR_BITS - i) & 1) {	
						kernel_bitmap[i/32] &= ~(HIGH_BIT >> i % 32); //Set to O
					}
				}
				else
				{
					printd("Bitmap Bit #: %i\n", i);
					printd("Bitmap Section: %i\n", i / 32);
					printd("Bitmap Section Bit: %i\n", i % 32);
					printd("Unusable\n");
					kernel_bitmap[i/32] |= (HIGH_BIT >> i % 32); //Set to 1
				}
				i++;
			}
			mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
			_debug( term_setcolor( make_color(COLOR_LIGHT_GREY, COLOR_BLACK)));
		}
		printd("Usable Memory: %l\n", usable_mem);
		printd("Unusable Memory %l\n", unusable_mem);
		printd("Re-measured Bitmap Memory: %x\n", ROUND_UP(usable_mem + unusable_mem) / 0x1000);
	}
	else if((mbd->flags & 1))
	{
		printd("Memory Length: %x\n", mbd->mem_upper);
	}
	else
	{
		puts("Fatal Error: No valid memory info. Halted.");
		for(;;);
	}
	_debug( term_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK)) );
}

#undef UNMAP_KERNEL
#undef BIT
#undef ROUND_DOWN
#undef ROUND_UP
