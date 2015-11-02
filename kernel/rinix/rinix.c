#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arch/info.h>

#include <mm/bitmap.h>
#include <mm/buddy.h>
#include <mm/mem.h>

#include <rinix/arch_init.h>
#include <rinix/debug.h>
#include <rinix/multiboot.h>
#include <rinix/pause.h>
#include <rinix/version.h>
#include <rinix/tty.h>
#include <rinix/timer.h>
#include <rinix/vga.h>

#define ADJUST_MMAP(x) ((multiboot_memory_map_t *)(0xC0400000 | ((unsigned int)x & 0xFFF)))

inline uint8_t _check_frame(uintptr_t frame)
{
	return ( (kernel_bitmap.ptr[(frame/0x1000)/8] >> 7-((frame/0x1000)&7)) & 1 );
}

uint32_t initialPageDirectory;

void initk(multiboot_info_t *mbd, uint32_t initialPD)
{
	initialPageDirectory = initialPD;
	term_init();
	arch_init();	// Perform architecture dependent initialization./
	page_init(initialPageDirectory);
	
	// Put the Copyright Notice
	puts("(c) Copyright 2015 Royston Martha. All Rights Reserved.");
	printf("rinix-%s v%d.%d.%d", ARCH_NAME, _RINIX_VERSION_MAJOR, _RINIX_VERSION_MINOR, _RINIX_VERSION_RELEASE);
	// Check multiboot structure for errors 
	printd(" (Debug Mode)");
	printf("\n");
	mem_init(mbd);
	
	arch_finish_init();
}

void kmain(void)
{
	
	// printf("Architecture: %s\n", ARCH_NAME);
	printd("Page Directory is at address: %x\n", initialPageDirectory);
	printd("Kernel Begins at %x\n", &kernel_start);
	printd("Kernel Ends at: %x\n", &kernel_end);
	set_pgt_entry(0, 0);
	// set_pgt_entry(0xC0000000, 0);
	// set_pd(pd);
	printd("Page Entry is: %x\n", get_pgt_entry(0xC0000000));
	uintptr_t frame1;
	uintptr_t frame2;
	uintptr_t frame3;
	uintptr_t frame4;
	printd("First Free Frame is: %x\n", frame1 = get_frame());
	printd("Second Free Frame is: %x\n", frame2 = get_frame());
	printd("Freeing Frame at: %x\n", frame1);
	free_frame(frame1);
	printd("Next Free Frame is: %x\n", frame3 = get_frame());
	printd("The Next Free Frame is: %x\n", frame4 = get_frame());
	char *used_str = "used";
	char *free_str = "free";
	for(uintptr_t i = 0; i <= 0x100000; i += 0x1000)
	{
		printd("Frame #%d at %x is %s\n", i/0x1000, i, ((_check_frame(i) == 1) ? used_str : free_str )  );
		if(_check_frame(i) == 0)
		{
			pause();
		}
	}
	for(uintptr_t i = ((unsigned long)(&kernel_start)/0x1000); i <= ((unsigned long)(&kernel_end)/0x1000); i += 0x1000)
	{
		printd("Frame #%d at %x is %s\n", i/0x1000, i, ((_check_frame(i) == 1) ? used_str : free_str )  );
		if(_check_frame(i) == 0)
		{
			pause();
		}
	}
	
	for(;;);
}
