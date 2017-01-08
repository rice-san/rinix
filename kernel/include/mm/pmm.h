#ifndef __MM_PMM_H
#define __MM_PMM_H

#ifdef __cplusplus
extern "C" {
#endif

struct physical_mem {
  uint64_t last_checked[PMM_BLOCK_ORDER]; // The last checked search for each search level
  uint64_t len; // Length of frame bitmap in bits
  uintptr_t ptr; // Beginning of Bitmap Array
}

typedef struct pmm_block{
  long pfn; // The number of the first page frame this block represents
  long len; // The length in page frames of this block
}pmm_block_t;

void __pmm_init(void);  // Initialize the pmm.
void _pmm_alloc(void);  // Allocate a block of page frames
pmm_block_t* pmm_alloc(int size); // Same as the previous but also allocates the pmm_block structure
void pmm_free(pmm_block_t* blk); // Free a block of page frames

#ifdef __cplusplus
}
#endif

#endif
