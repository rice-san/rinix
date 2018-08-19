#ifndef _MM_KHEAP_H
#define _MM_KHEAP_H

#define MIN_BUCKET_SIZE 8
#define MAX_BUCKET_SIZE 512

extern uintptr_t kheap_size;

typedef struct kheap_bucket_header{
  size_t obj_size;
  struct kheap_bucket_header *left; // Lesser size tree node
  struct kheap_bucket_header *right; // Greater size tree node
  unsigned int max_count;
  unsigned int free_count;
  void* first_free;
  struct kheap_bucket_header *next; // Next bucket of same size
} kheap_bucket_header_t;

void kheap_init(void);

uintptr_t kheap_alloc(size_t);
void kheap_dealloc(uintptr_t);



#endif /* _MM_KHEAP_H */
