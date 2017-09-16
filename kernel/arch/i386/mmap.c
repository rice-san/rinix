#include <stdint.h>

#include <arch/info.h>
#include <arch/page.h>

#include <mm/bitmap.h>
#include <mm/mem.h>

#include <rinix/debug.h>
#include <mm/mmap.h>
#include <arch/multiboot.h>
#include <arch/multiboot_stub.h>

// TODO: make mmap sections number dynamic rather than static
mmap_t mmap;
mmap_chunk_t mmap_regions[MMAP_REGION_NUM];

int create_mmap(void){
  mmap.regions = &mmap_regions;
  int mmap_region_nr = 0;
  if(!(multiboot_info->flags & (MULTIBOOT_MEMORY_INFO))){
    panic("Multiboot Memory is invalid. All hope is lost");
    return 1; // Multiboot is invalid; all hope is lost
  }
  for (int i = 0; i < MMAP_REGION_NUM; i = i + 1){
    mmap.regions[mmap_region_nr].start = (uintptr_t)0;
    mmap.regions[mmap_region_nr].size = (unsigned long)0;
    mmap.regions[mmap_region_nr].next = (mmap_chunk_t*)0; // TODO: change if linked lists are later employed
    mmap.regions[mmap_region_nr].type = 0;
  }
  multiboot_memory_map_t* mbt_map = multiboot_info->mmap_addr;
  while(mbt_map < multiboot_info->mmap_addr + multiboot_info->mmap_length){
    if(mmap_region_nr >= MMAP_REGION_NUM) {
      panic("Memory divided into too many regions. (Modify code to support more divisions)");
    }
    if(mbt_map->type == 1 ){
      mmap.regions[mmap_region_nr].start = (uintptr_t)mbt_map->addr;
      mmap.regions[mmap_region_nr].size = (unsigned long)mbt_map->len;
      mmap.regions[mmap_region_nr].next = (mmap_chunk_t*)0; // TODO: change if linked lists are later employed
      mmap.regions[mmap_region_nr].type = MMAP_USABLE;
    }else{
      mmap.regions[mmap_region_nr].start = (uintptr_t)mbt_map->addr;
      mmap.regions[mmap_region_nr].size = (unsigned long)mbt_map->len;
      mmap.regions[mmap_region_nr].next = (mmap_chunk_t*)0; // TODO: change if linked lists are later employed
      mmap.regions[mmap_region_nr].type = MMAP_UNUSABLE;
    }
    printd("Memory Map: start=%x length=%x size=%x value=%x\n", (unsigned long)mmap.regions[mmap_region_nr].start, ((unsigned long)mmap.regions[mmap_region_nr].size), (unsigned long)mmap.regions[mmap_region_nr].size, mmap.regions[mmap_region_nr].type);
    mbt_map = (multiboot_memory_map_t*) ( (unsigned int)mbt_map + mbt_map->size + sizeof(unsigned int) );
    mmap_region_nr = mmap_region_nr + 1;
  }
  return 0;
}
