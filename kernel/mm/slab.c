#include <mm/slab.h>

// Slab Allocator

slab_alloc_t slb_alloc_alloc; // A slab allocator for slab allocators

slab_alloc_t *slab_init(size_t obj_size, int min_free){

}

void* slab_alloc(slab_alloc_t* alloc){

}

void slab_free(void*){

}

// Find location of slab structure for a given ptr
static uintptr_t slab_of(uintptr_t obj_loc){

}
