#include <stddef.h>
#include <arch/page.h>
#include <mm/mem.h>
#include <mm/kheap.h>

// A linked list bucket heap
// NOTE: This allocator is mutually exclusive to the generic linked list allocator found in kmem.c

uintptr_t kheap_limit;
kheap_bucket_header_t* kheap;

void kheap_init(size_t size){
  kheap_size = NULL;
  kheap = NULL;
}

uintptr_t create_bucket(size_t size) {
  uintptr_t this_page = 0xD0000000 + kheap_size; // The virtual page this bucket will be working with
  //TODO: replace with actual vmm access
  alloc_map_page(this_page, PAGE_P | PAGE_S | PAGE_RW);
  kheap_bucket_header_t* bucket = this_page + PAGE_SIZE - sizeof(kheap_bucket_header);
  bucket->obj_size = size;
  bucket->left = NULL;
  bucket->right = NULL;
  bucket->max_count = ((PAGE_SIZE-sizeof(kheap_bucket_header))/size);
  bucket->free_count = bucket->max_count;
  for(int i = 0; i < max_count; i++) {
    *((void*)this_page + (i*size)) = this_page + ((i+1)*size); // Point each free space to the next address
  }
  bucket->first_free = this_page;
  bucket->next = NULL;
  kheap_size += PAGE_SIZE;
  return bucket;
}

uintptr_t kheap_expand_bucket(kheap_bucket_header_t* bucket) {
  if (bucket->next || bu) {
    panic("Attempt to extend already expanded bucket at %x.\n", bucket);
  }
}

uintptr_t kheap_bucket_alloc(kheap_bucket_header_t* bucket) {
  if (bucket->free_count < 2 && kheap) {
    kheap_expand_bucket(bucket);
  }

}

uintptr_t kheap_alloc(size_t size){
  // Traverse tree
  kheap_bucket_header_t* parent = kheap;
  kheap_bucket_header_t* node = kheap;
  while (node != NULL) {
    if (node->size == size) {
      // Allocate here

    }else if(node->size > size) {
      // traverse tree to the left
      parent = node;
      node = node->left;
    }else {
      // traverse tree to the right
      parent = node;
      node = node->right;
    }
  }
  // There is no prexisting node here
  if(size > parent->size){
    // New bucket should be a right node
    parent->right = create_bucket(size);
  }else{
    // New bucket should be a left node
    parent->left = create_bucket(size);
  }
}

void kheap_dealloc(uintptr_t loc){

}
