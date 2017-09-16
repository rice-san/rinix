#include <mm/bitmap.h>
#include <mm/kmem.h>
#include <mm/mem.h>
#include <mm/pmm.h>

#include <arch/info.h>
#include <arch/page.h>

#include <arch/multiboot.h>
#include <arch/multiboot_stub.h>
#include <mm/mmap.h>
#include <rinix/pause.h>
#include <rinix/timer.h>



#define UNMAP_KERNEL(x) ((unsigned long) (x) - 0xC0000000)
#define ROUND_DOWN(x) (((unsigned long)(x) & ~0x0fff) - 0x1000)
#define ROUND_UP(x) (((unsigned long)(x) & ~0x0fff) + 0x1000)



int bitmap_size = 0;


void kernel_bitmap_init()
{
	kernel_length = (&kernel_end - &kernel_start);

	unsigned long section_start = 0;
	unsigned long section_end = 0;
	unsigned long section_size = 0;

	unsigned long bitmap_place;

	unsigned long try_bitmap_start = 0;
	unsigned long try_bitmap_end = 0;
	printd("Multiboot Info: %x\n", &multiboot_info);
	_debug( term_setcolor(make_color(COLOR_CYAN, COLOR_BLACK)) );
	printd("Memory Map At: %x\n", multiboot_info->mmap_addr);
	printd("Memory Map Length: %x\n", multiboot_info->mmap_length);
	if((multiboot_info->flags & 0x40))
	{
		if(multiboot_info->flags & 1)
		{
			printd("Memory Length: %lK\n", ((memory_length = (multiboot_info->mem_lower)+(multiboot_info->mem_upper))));
			bitmap_size = (memory_length / 0x4) + 1;
			printd("Kernel Bitmap Size: %x\n", bitmap_size);
		}
		// Find place for bitmap
		multiboot_memory_map_t* mmap = multiboot_info->mmap_addr;
		printd("Memory Map Info: \n==================\n");
		unsigned long usable_mem = 0;
		unsigned long unusable_mem = 0;
		while(mmap < multiboot_info->mmap_addr + multiboot_info->mmap_length) {
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
			printd("Memory Map: start=%x length=%x size=%x value=%x end=%x\n", (section_start = (unsigned long)mmap->addr), (section_size = (unsigned long)mmap->len), (unsigned long)mmap->size, mmap->type, (section_end = (unsigned long)mmap->addr + mmap->len - 1));
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
							create_bitmap(bitmap_place, bitmap_size);
							break;
						}
					}
				}
				if(newline > 0)
				{
					printd("\n");
				}
				else
				{
					printd("");
				}
			}
			mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
			_debug( term_setcolor( make_color(COLOR_LIGHT_GREY, COLOR_BLACK)));
		}
	}
	else if((multiboot_info->flags & 1))
	{
		printd("Memory Length: %x\n", multiboot_info->mem_upper);
	}
	else
	{
		puts("Fatal Error: No valid memory info. Halted.");
		for(;;);
	}
	_debug( term_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK)) );
}

void mem_init(void)
{
	create_mmap();
	__pmm_init();
	kmem_init();
}

#undef UNMAP_KERNEL
#undef ROUND_DOWN
#undef ROUND_UP
