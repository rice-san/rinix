#ifndef _MM_KHEAP_H
#define _MM_KHEAP_H

struct kheap_bucket_header{
  size_t obj_size;
  struct kheap_bucket_header *next_header;
  void* first_free;
};

void kheap_init(void);

uintptr_t kheap_alloc(size_t);
void kheap_dealloc(uintptr_t);



#endif /* _MM_KHEAP_H */
