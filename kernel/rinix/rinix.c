#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arch/info.h>

#include <mm/bitmap.h>
#include <mm/buddy.h>
#include <mm/kmem.h>
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
#define ADJUST_KERNEL(x) ((((unsigned long)x) - (unsigned long)0xC0000000))

inline uint8_t _check_frame(uintptr_t frame)
{
	return ( (kernel_bitmap.ptr[(frame/0x1000)/8] >> 7-((frame/0x1000)&7)) & 1 );
}

uint32_t initialPageDirectory;

extern void panic_splash(void);

void initk(multiboot_info_t *mbd, uint32_t initialPD)
{
	initialPageDirectory = initialPD;
	term_init();
	
	// Put the Copyright Notice
	puts("(c) Copyright 2015 Royston Martha. All Rights Reserved.");
	printf("rinix-%s v%d.%d.%d", ARCH_NAME, _RINIX_VERSION_MAJOR, _RINIX_VERSION_MINOR, _RINIX_VERSION_RELEASE);
	printd(" (Debug Mode)");
	printf("\n");
	arch_init();	// Perform architecture dependent initialization./
	
	// Initialize the Memory Allocators with the Multiboot Information
	page_init(initialPageDirectory);
	mem_init(mbd);
	
	arch_finish_init();
}

void kmain(void)
{
	// printf("Architecture: %s\n", ARCH_NAME);
	printd("Page Directory is at address: %x\n", initialPageDirectory);
	printd("Kernel Begins at %x\n", &kernel_start);
	printd("Kernel Ends at: %x\n", &kernel_end);
	dump_kmem_tables();
	uintptr_t* value = (uintptr_t*)kmalloc((size_t)0x2000);
	dump_kmem_tables();
	*value = 10;
	int* value2 = (uintptr_t*)kmalloc(sizeof(int));
	dump_kmem_tables();
	*value2 = 5;
	kfree((void *)value);
	dump_kmem_tables();
	kfree((void *)value2);
	dump_kmem_tables();
	panic_splash();
	for(;;);
}
