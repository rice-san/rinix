// Make a GDT segment descriptors in 64-bit integer form

#include <stdio.h>
#include <stdint.h>

typedef uint64_t gdt_entry;

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));


gdt_entry gdt[5];
struct gdt_ptr gp;

extern void gdt_load();

// Defines for each specific flag descriptor
// Intel documentation provides a description of what each one does.
#define GDT_DESCTYPE(x)		((x) << 0x04) // Descriptor type (0 for system, 1 for code/data)
#define GDT_PRES(x)			((x) << 0x07) // Present
#define GDT_SAVL(x)			((x) << 0x0C) // Available for system use
#define GDT_LONG(x)			((x) << 0x0D) // Long mode
#define GDT_SIZE(x)			((x) << 0x0E) // Size (0 for 16-bit, 1 for 32)
#define GDT_GRAN(x)			((x) << 0x0F) // Granularity (0 for 1B - 1MB, 1 for 4KB - 4GB)
#define GDT_PRIV(x)			(((x) & 0x03) << 0x05)	// Set privilege level (0 - 3)


// The following can be bitwise OR'ed together to make a proper flag

// DATA SEGMENT FLAGS
#define GDT_DATA_RD			0x00 // Read-Only
#define GDT_DATA_RDWR		0x02 // Read/Write
#define GDT_DATA_ACC		0x01 // Accessed (Needs a RD or RDWR flag)
#define GDT_DATA_EXPD		0x04 // Expand-Down (Needs a RD or RDWR flag)

// CODE SEGMENT FLAGS
#define GDT_CODE_EX			0x08 // Execute-Only
#define GDT_CODE_EXRD		0x0A // Execute/Read
#define GDT_CODE_ACC		0x01 // Accessed (Needs a EX or EXRD flag)
#define GDT_CODE_CONF		0x04 // Conforming (Needs a EX or EXRD flag)


// SEGMENTS
#define GDT_CODE_PL0	GDT_DESCTYPE(1)	| GDT_PRES(1)	| GDT_SAVL(0)	| \
						GDT_LONG(0)		| GDT_SIZE(1)	| GDT_GRAN(1)	| \
						GDT_PRIV(0)		| GDT_CODE_EXRD

#define GDT_DATA_PL0	GDT_DESCTYPE(1)	| GDT_PRES(1)	| GDT_SAVL(0)	| \
						GDT_LONG(0)		| GDT_SIZE(1)	| GDT_GRAN(1)	| \
						GDT_PRIV(0)		| GDT_DATA_RDWR


#define GDT_CODE_PL3	GDT_DESCTYPE(1)	| GDT_PRES(1)	| GDT_SAVL(0)	| \
						GDT_LONG(0)		| GDT_SIZE(1)	| GDT_GRAN(1)	| \
						GDT_PRIV(3)		| GDT_CODE_EXRD


#define GDT_DATA_PL3	GDT_DESCTYPE(1)	| GDT_PRES(1)	| GDT_SAVL(0)	| \
						GDT_LONG(0)		| GDT_SIZE(1)	| GDT_GRAN(1)	| \
						GDT_PRIV(3)		| GDT_DATA_RDWR


void
create_gdt_descriptor(const int num, uint32_t base, uint32_t limit, uint16_t flag)
{
		gdt_entry descriptor;
		
		descriptor  = limit			& 0x000F0000;	// set limit bits 19:16
		descriptor |= (flag << 8)	& 0x00F0FF00;	// set type, p, dpl, s, g, d/b, l and avl fields
		descriptor |= (base >> 16)	& 0x000000FF;	// set base bits 23:16
		descriptor |= base			& 0xFF000000;	// set base bits 31:24
		
		// Shift descriptor by 32 to allow for low part of segment
		descriptor <<= 32;
		
		// Create the low 32 bit segment
		descriptor |= base << 16;					// set base bits 15:0
		descriptor |= limit & 0x0000FFFF;			// set limit bits 15:0
		
		gdt[num] = descriptor;
}

void gdt_init(void)
{
	gp.limit = sizeof(gdt);
	gp.base = &gdt;
		
	create_gdt_descriptor(0, 0, 0, 0);
	create_gdt_descriptor(1, 0, 0x000FFFFF, (GDT_CODE_PL0));
	create_gdt_descriptor(2, 0, 0x000FFFFF, (GDT_DATA_PL0));
	create_gdt_descriptor(3, 0, 0x000FFFFF, (GDT_CODE_PL3));
	create_gdt_descriptor(4, 0, 0x000FFFFF, (GDT_DATA_PL3));
	
	gdt_load();
}
