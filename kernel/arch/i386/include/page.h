#ifndef _ARCH_PAGE_H
#define _ARCH_PAGE_H

#include <stdint.h>

// This file has been modified for: x86 Architechture

// Page Table Structure
// The sizes have to be put in manually for now.
// (Automatic would be too complicated for me right now)

// PGDT - Page Table Directory
// PGMT - Page Table Middle
// PGLT - Page Table Low
// PGOFFSET - Address Offset

typedef uint32_t uaddr_t; // Set address size for architecture.
typedef uint32_t pte_t; // Set size of an page table entry

#define PAGE_SIZE 4096
#define ADDR_BITS 32

#define _PGDT_BITS 10 		// Size in bits
#define _PGDT_MASK 0xFFC00000
#define _PGMT_BITS 0  		// Size in bits
#define _PGMT_MASK 0x00000000
#define _PGLT_BITS 10 		// Size in bits
#define _PGLT_MASK 0x003FF000
#define _PGOFFSET_BITS 12	// Size in bits
#define _PGOFFSET_MASK 0x00000FFF

#define _PGDT_SIZE 1024 // Number of Entries
#define	_PGMT_SIZE 0    // Number of Entries
#define _PGLT_SIZE 1024 // Number of Entries

// MAXIMUM MEMORY
#define _RINIX_MAX_VMEM 0x100000000 // 4 GB

// Define Paging Flags - OR them together

#define PAGE_P      0x00000001
#define PAGE_RO     0x00000000
#define PAGE_RW     0x00000002
#define PAGE_S      0x00000000
#define PAGE_U      0x00000004

#endif
