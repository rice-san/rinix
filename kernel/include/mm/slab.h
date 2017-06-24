#ifndef _MM_SLAB_H
#define _MM_SLAB_H

#ifdef __cplusplus
extern "C" {
#endif

// Slab Free List Item
typedef struct slab_list{
  slab_list_t* next;
}slab_list_t;


// Page sized slab region table
struct slab{
  struct slab* next;
  struct slab* next_free_slab;
  int n_free;
  slab_list_t* first_free;
};

// Table for a whole slab allocation system
typedef struct slab_allocator{
  int n_pages;
  int n_free;
  int min_free;
  size_t obj_size;
  struct slab* first_slab;
  struct slab* first_free_slab;
}slab_alloc_t;


// Allocation functions

void __slab_init(void);
slab_alloc_t* slab_init(size_t, int);
void* slab_alloc(slab_alloc_t*);
void slab_free(void*);

#ifdef __cplusplus
}
#endif

#endif
