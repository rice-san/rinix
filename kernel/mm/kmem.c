// mm/kmem.c - The rinix kernel heap manager.

#include <stddef.h>
#include <arch/page.h>
#include <mm/kmem.h>
#include <mm/mem.h>

#define KMEM_ALIGN_SIZE 0x8

#define KMEM_FREE 0x00
#define KMEM_USED 0x01
#define KMEM_TAIL 0x81

typedef struct kmemptr{
    struct kmemptr* prev;
    struct kmemptr* next;
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
        return scratch->next; 
    }
    else
    {
        return NULL;
    }
}

inline static size_t align_block(unsigned int addr){
	return (addr % KMEM_ALIGN_SIZE ? (addr + (KMEM_ALIGN_SIZE - (addr % KMEM_ALIGN_SIZE))) : addr);
}

inline static void dump_kmemptr_t(kmemptr_t* ptr)
{
	#ifdef DEBUG
	if (ptr != NULL)
		printd("This Block: %x size: %x prev: %x flags: %x Next Block: %x\n", ptr, ptr->size, ptr->prev, ptr->flags, kmemptr_next(ptr));
	#endif
}

void dump_kmem_tables(void)
{
	#ifdef DEBUG
    printd("kmem_tables dump\n");
    kmemptr_t* scratch = kmem_info->head;
    printd("heap size: %x\n", kmem_info->total_length);
    do
    {
        pause();
        dump_kmemptr_t(scratch);
        scratch = kmemptr_next(scratch);
    }while(scratch->flags != KMEM_TAIL);
    #endif
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
    kmem_info->head->next = kmem_info->tail;
    kmem_info->tail->prev = kmem_info->head;
    kmem_info->tail->size = 0;
    kmem_info->tail->flags = KMEM_TAIL;
    kmem_info->tail->next = kmem_info->tail;
    kmem_info->first_free = kmem_info->head;
    kmem_info->total_length = 0x1000;
}

void kmem_morecore(unsigned int block_count)
{
    printd("kmem_morecore() called\n");
    printd("block_count: %i\n", block_count);
    for(int i = 0; i<block_count; i++)
    {
        printd("Cycles remaining: %i\n", block_count - i);
        alloc_map_page(0xD0000000 + kmem_info->total_length, 0x3);
        kmem_info->total_length += PAGE_SIZE;
        kmemptr_t* kmem_tail_prev = kmem_info->tail->prev;
        printd("Last Memory Block: ");
        dump_kmemptr_t(kmem_tail_prev);
        if (kmem_tail_prev->flags == KMEM_FREE)
        {
            kmem_tail_prev->size += (unsigned int)PAGE_SIZE;
            kmem_info->tail = kmemptr_next(kmem_tail_prev);
            kmem_info->tail->prev = kmem_tail_prev;
            kmem_info->tail->size = 0;
            kmem_info->tail->flags = KMEM_TAIL;
        }
        else if(kmem_tail_prev->flags == KMEM_USED)
        {
            kmemptr_t* scratch = kmem_info->tail;
            scratch->size = PAGE_SIZE - sizeof(kmemptr_t);
            kmem_info->tail = scratch + (unsigned int)PAGE_SIZE;
            kmem_info->tail->flags = KMEM_TAIL;
        }
    }
}


void* kmalloc(size_t bsize)
{
    printd("kmalloc called\n");
    int found = 0;
    printd("Request for block of %x bytes\n", bsize);
    bsize = align_block(bsize);
    kmemptr_t* scratch = kmem_info->first_free;
    while (!found && scratch < kmem_info->head + kmem_info->total_length && scratch != NULL)
    {
        if (scratch->flags == KMEM_TAIL)
        {
            // Let's decide how many block to allocate (so we don't scan the list a million times.)
            uint32_t size_req;
            if (scratch->prev->flags == KMEM_FREE)
            {
                size_req = (bsize + sizeof(kmemptr_t)) - scratch->prev->size; 
            }
            else
            {
                size_req = bsize + sizeof(kmemptr_t);
            }
            kmem_morecore( ((size_req/0x1000) + ( (size_req % 0x1000) ? 1 : 0 )) );
            scratch = kmem_info->first_free;
        }
        
        if ((scratch->flags == KMEM_FREE) && (scratch->size > (bsize + sizeof(kmemptr_t))))
        {
            // Let's use and chop this block
            kmemptr_t* new_ptr = ((unsigned int)scratch + sizeof(kmemptr_t) + bsize);
            kmemptr_next(scratch)->prev = new_ptr;
            new_ptr->size = scratch->size - (bsize + sizeof(kmemptr_t));
            new_ptr->prev = (unsigned int)scratch;
            new_ptr->next = scratch->next;
            scratch->size = bsize;
            scratch->next = (unsigned int)new_ptr;
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
	// As a rule of thumb, A block being freed eats the one after it.
	// If a free block preceeds it, then the previous block will eat this one
	// (Including the block that this one ate).
	
    printd("kfree called\n");
    kmemptr_t* scratch = (kmemptr_t *)(ptr - sizeof(kmemptr_t)); // Get the location of the kmemptr that corresponds to this block
    kmemptr_t* prev = scratch->prev; // The previous ptr
    if (scratch->flags == KMEM_USED)
    {
        if (kmemptr_next(scratch)->flags == KMEM_FREE && kmemptr_next(scratch)->flags != KMEM_TAIL)
        {
            // If the next block is free...
            kmemptr_next(kmemptr_next(scratch))->prev = scratch; // The set the prev ptr of the block after that to this one
            scratch->next = kmemptr_next(kmemptr_next(scratch)); // Set the next ptr of this block to the one after that.
            scratch->size += kmemptr_next(scratch)->size + sizeof(kmemptr_t); // Add the next block to the allocator
            scratch->flags = KMEM_FREE; // set this block to free
        }
        if (prev->flags == KMEM_FREE && prev != scratch)
        {
            // If the previous block is free...
            prev->size += (scratch->size + sizeof(kmemptr_t)); // Add the size of this block to the size of the previous one
            kmemptr_next(scratch)->prev = prev; // Set the previous ptr of the next block to the one before us.
            prev->next = kmemptr_next(scratch); //Set the next ptr of the previous block to the one after this one.
        }
        if (scratch->flags == KMEM_USED)
        {
			// If this block is in solitary space, and STILL isn't freed...
			scratch->flags = KMEM_FREE; // We'll have to leave it free and alone.
		}
    }
    else
    {
        return;
    }
}
