#include <mm/pmm.h>
//#include <mm/slab.h>

#include <stdint.h>
#include <arch/multiboot_stub.h>

// Physical Memory Manager

// TODO: Implement actual new algorithms

// Remove once new algorithms are implemented
extern void kernel_bitmap_init();
extern uintptr_t get_frame(void);


void __pmm_init(void){
  kernel_bitmap_init();
}

void _pmm_alloc(pmm_block_t* blk, unsigned int size){

}

uintptr_t pmm_alloc(unsigned int size){
  // Remove once new algorithms are defined
  if (size != 0) {
    return (uintptr_t)0;
  }

  return get_frame();

}

int pmm_free(uintptr_t pf){
  return free_frame(pf);
}
