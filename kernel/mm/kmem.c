// mm/kmem.c - The rinix kernel heap manager.

#include <stddef.h>
#include <mm/kmem.h>
#include <mm/mem.h>

#define KMEM_FREE 0x00
#define KMEM_USED 0x01
#define KMEM_TAIL 0x81

typedef struct kmemptr{
    struct kmemptr* prev;
    size_t size;
    uint8_t flags;
}kmemptr_t;

typedef struct kmem{
    struct kmemptr* head;
    struct kmemptr* first_free;
    struct kmemptr* tail;
    size_t total_length;
}kmem_t;

kmem_t _kmem;

kmem_t* kmem_info = &_kmem;

inline static kmemptr_t* kmemptr_next(kmemptr_t* scratch)
{
    if (scratch->flags != KMEM_TAIL)
    {
        return (kmemptr_t*)((unsigned int)scratch + (unsigned int)scratch->size + sizeof(kmemptr_t)); 
    }
    else
    {
        return NULL;
    }
}

inline static void dump_kmemptr_t(kmemptr_t* ptr)
{
	if (ptr != NULL)
		printd("This Block: %x size: %x prev: %x flags: %x Next Block: %x\n", ptr, ptr->size, ptr->prev, ptr->flags, kmemptr_next(ptr));
}

void dump_kmem_tables(void)
{
    printd("kmem_tables dump\n");
    kmemptr_t* scratch = kmem_info->head;
    printd("heap size: %x\n", kmem_info->total_length);
    do
    {
        pause();
        dump_kmemptr_t(scratch);
        scratch = kmemptr_next(scratch);
    }while(scratch->flags != KMEM_TAIL);
}

void kmem_init(void)
{
	alloc_map_page(0xD0000000, PAGE_P | PAGE_RW | PAGE_S);
	printd("Size of kmemptr_t: %x\n", sizeof(kmemptr_t));
    kmem_info->head = (kmemptr_t *)0xD0000000;
    kmem_info->head->prev = kmem_info->head;
    kmem_info->head->size = (0x1000 - (2*sizeof(kmemptr_t)));
    kmem_info->head->flags = 0;
    kmem_info->tail = (kmemptr_t*)(0xD0001000 - sizeof(kmemptr_t));
    kmem_info->tail->prev = kmem_info->head;
    kmem_info->tail->size = 0;
    kmem_info->tail->flags = KMEM_TAIL;
    kmem_info->first_free = kmem_info->head;
    kmem_info->total_length = 0x1000;
}

void kmem_morecore()
{
    printd("kmem_morecore() called\n");
    alloc_map_page(0xD0000000 + kmem_info->total_length, 0x3);
    kmem_info->total_length += 0x1000;
    kmemptr_t* kmem_tail_prev = kmem_info->tail->prev;
    printd("Before kmemptr_t");
    dump_kmemptr_t(kmem_tail_prev);
    if (kmem_tail_prev->flags == KMEM_FREE)
    {
        kmem_tail_prev->size += (unsigned int)0x1000;
        kmem_info->tail = kmemptr_next(kmem_tail_prev);
        kmem_info->tail->prev = kmem_tail_prev;
        kmem_info->tail->size = 0;
        kmem_info->tail->flags = KMEM_TAIL;
    }
    else if(kmem_tail_prev->flags == KMEM_USED)
    {
        kmemptr_t* scratch = kmem_info->tail;
        scratch->size = 0x1000 - sizeof(kmemptr_t);
        kmem_info->tail = scratch + (unsigned int)0x1000;
        kmem_info->tail->flags = KMEM_TAIL;
    }
}


void* kmalloc(size_t bsize)
{
    printd("kmalloc called\n");
    int found = 0;
    printd("Request for block of %x bytes\n", bsize);
    kmemptr_t* scratch = kmem_info->first_free;
    while (!found && scratch < kmem_info->head + kmem_info->total_length && scratch != NULL)
    {
        if (scratch->flags == KMEM_TAIL)
        {
            kmem_morecore();
            scratch = kmem_info->first_free;
        }
        
        if ((scratch->flags == KMEM_FREE) && (scratch->size > bsize))
        {
            // Let's use and chop this block
            kmemptr_t* new_ptr = ((unsigned int)scratch + sizeof(kmemptr_t) + bsize);
            new_ptr->size = scratch->size - (bsize + sizeof(kmemptr_t));
            new_ptr->prev = (unsigned int)scratch;
            scratch->size = bsize;
            scratch->flags = 0x01;
            new_ptr->flags = 0x00;
            return (kmemptr_t*)((unsigned int)scratch + sizeof(kmemptr_t));
        }
        else
        {
            // Let's try the next block instead
            scratch = kmemptr_next(scratch);
            continue;
        }
    }
    return NULL;
}

void kfree(void* ptr)
{
    printd("kfree called\n");
    kmemptr_t* scratch = (kmemptr_t *)(ptr - sizeof(kmemptr_t)); // Get the location of the kmemptr that corresponds to this block
    kmemptr_t* prev = scratch->prev; // The previous ptr
    if (scratch->flags & 0x01)
    {
        if (kmemptr_next(scratch)->flags == KMEM_FREE && kmemptr_next(scratch)->flags != KMEM_TAIL)
        {
            // If the next block is free...
            kmemptr_next(kmemptr_next(scratch))->prev = scratch;
            scratch->size += kmemptr_next(scratch)->size + sizeof(kmemptr_t); // Add the next block to the allocator
            scratch->flags = KMEM_FREE;
        }
        if (prev->flags == KMEM_FREE)
        {
            // If the previous block is free...
            prev->size += (scratch->size + sizeof(kmemptr_t)); // Add the size of this block to the size of the previous one
            kmemptr_next(scratch)->prev = prev; // Set the previous ptr of the next block to this one.
        }
    }
    else
    {
        return;
    }
}
