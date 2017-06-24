#include <mm/slab.h>

// Slab Allocator

slab_alloc_t slb_alloc_alloc; // A slab allocator for slab allocators

// Initialize the whole slab system
void __slab_init(void){
  slb_alloc_alloc->n_pages = 0;
  slb_alloc_alloc->n_free = 0;
  slb_alloc_alloc->min_free = 2;
  slb_alloc_alloc->obj_size = sizeof(struct slab)
  slb_alloc_alloc->first_slab = (struct slab*)0;
  slb_alloc_alloc->first_free_slab = (struct slab*)0;
}

// Create a slab system
slab_alloc_t *slab_init(size_t obj_size, int min_free){

}

// Allocate an object from the slab
void* slab_alloc(slab_alloc_t* alloc){

}

// Free an object from the slab
void slab_free(void*){

}

// Find location of slab structure for a given ptr
static uintptr_t slab_of(uintptr_t obj_loc){

}
