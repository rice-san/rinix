#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arch/info.h>

#include <mm/buddy.h>
#include <mm/mem.h>

#include <rinix/arch_init.h>
#include <rinix/multiboot.h>
#include <rinix/version.h>
#include <rinix/tty.h>
#include <rinix/timer.h>

multiboot_info_t *mbd;
uint32_t initialPageDirectory;

void initk(multiboot_info_t *mboot, uint32_t initialPD)
{
	initialPageDirectory = initialPD;
	mbd = mboot;
	term_init();
	arch_init();	// Perform architecture dependent initialization./
	// Check multiboot structure for errors 
	page_init(initialPageDirectory);
	if((mbd->flags >> 6) & 1)
	{
		buddy_build(mbd);
	}
	else if((mbd->flags & 1))
	{
		
	}
	else
	{
		puts("Fatal Error: No valid memory info. Halted.");
		for(;;);
	}
}

void kmain(void)
{
	puts("(c) Copyright 2015 Royston Martha. All Rights Reserved.");
	printf("rinix-%s v%d.%d.%d\n", ARCH_NAME, _RINIX_VERSION_MAJOR, _RINIX_VERSION_MINOR, _RINIX_VERSION_RELEASE);
	// printf("Architecture: %s\n", ARCH_NAME);
	printf("Page Directory is at address: %x\n", initialPageDirectory);
	printf("Kernel Ends at: %x\n", &kernel_end);
	// set_pgt_entry(0, 0);
	printf("%x\n", get_pgt_entry(0));
	printf("%x\n", get_pgt_entry(0xC000000));
	for(;;);
}
