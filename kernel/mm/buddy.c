// Possible buddy.c implementation

#include <stdint.h>

#include <mm/buddy.h>


// Make a buddy list for sizes from PAGE_SIZE to PAGE_SIZE*2^7
buddy_t* buddy_list[BUDDY_MAX_ORDER + 1];

static unsigned int bsize(uint32_t size)
{
	for(unsigned int shift = 0; shift <=BUDDY_MAX_ORDER; shift++)
	{
		if(size < (PAGE_SIZE << shift))
		{
			return shift;
		}
	}
}


void buddy_build(uintptr_t b)
{
	
}

void get_page()
{

}
