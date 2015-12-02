#ifndef _MM_KMEM_H
#define _MM_KMEM_H

#define KERNEL_HEAP_START 0xD0000000
#define KERNEL_HEAP_END 0xE0000000

void kmem_init(void);
void* kmalloc(size_t);
void kfree(void*);

#endif