#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arch/info.h>

#include <mm/buddy.h>
#include <mm/mem.h>

#include <rinix/arch_init.h>
#include <rinix/multiboot.h>
#include <rinix/pause.h>
#include <rinix/version.h>
#include <rinix/tty.h>
#include <rinix/timer.h>

#define ADJUST_MMAP(x) ((multiboot_memory_map_t *)(0xC0400000 | ((unsigned int)x & 0xFFF)))

uint32_t initialPageDirectory;

void initk(multiboot_info_t *mbd, uint32_t initialPD)
{
	initialPageDirectory = initialPD;
	term_init();
	arch_init();	// Perform architecture dependent initialization./
	page_init(initialPageDirectory);
	
	// Put the Copyright Notice
	puts("(c) Copyright 2015 Royston Martha. All Rights Reserved.");
	printf("rinix-%s v%d.%d.%d\n", ARCH_NAME, _RINIX_VERSION_MAJOR, _RINIX_VERSION_MINOR, _RINIX_VERSION_RELEASE);
	// Check multiboot structure for errors 
	printf("Multiboot Info: %x\n", &mbd);
	if((mbd->flags & 0x40))
	{
		printf("Memory Length: %x\n", mbd->mem_upper);
	}
	else if((mbd->flags & 1))
	{
		printf("Memory Length: %x\n", mbd->mem_upper);
	}
	else
	{
		puts("Fatal Error: No valid memory info. Halted.");
		for(;;);
	}
}

void kmain(void)
{
	
	// printf("Architecture: %s\n", ARCH_NAME);
	printf("Page Directory is at address: %x\n", initialPageDirectory);
	printf("Kernel Begins at %x\n", &kernel_start);
	printf("Kernel Ends at: %x\n", &kernel_end);
	set_pgt_entry(0, 0);
	printf("%x\n", get_pgt_entry(0));
	printf("%x\n", get_pgt_entry(0xC0000000));
	for(;;);
}
