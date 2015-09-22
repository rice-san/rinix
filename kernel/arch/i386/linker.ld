/* The boot loader will look at the image and start exectuion at the entry point. */
ENTRY(loader)
OUTPUT(elf32-i386)

phys_addr = 0x100000;
offset = 0xC0000000;
virt_addr = phys_addr + offset;

/* This tells where the various sections of the object files will be put in the final kernel image. */
SECTIONS
{
	/* Start putting sections at 1 MiB, a conventional place for kernel to be loaded at by the bootloader. */
	. = phys_addr;

    .boot ALIGN(4K): {
        *(.multiboot)
        */boot.o (.text)
    }

    . += offset;
	/* First put the multiboot header, as we need to put it early in the image, or the bootloader won't recognize the format.
	Next we'll put the .text section. */
	.text ALIGN(4K) : AT(ADDR(.text) - offset)
	{
		core_start = .;
		*(EXCLUDE_FILE (*/boot.o) .text)
		*(.rodata*)
		core_end = .;
	}
	
	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - offset)
	{
		data_start = .;
        *(.boot_stack)
	    *(.data)
	    data_end = .;
	}
	
	/* Read-write data (uninitialized) */
	.bss ALIGN(4K) : AT(ADDR(.bss) - offset)
	{
		bss_start = .;
		*(COMMON)
		*(.bss)
		bss_end = .;
		kernel_end = .;
	}
}
