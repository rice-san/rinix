#ifndef _MM_MMAP_H
#define _MM_MMAP_H

#include <stdint.h>

#define MMAP_REGION_NUM 16

#define MMAP_USABLE 0
#define MMAP_UNUSABLE 1

typedef struct mmap{
  unsigned long mem_len;
  struct mmap_chunk* regions;
}mmap_t;


typedef struct mmap_chunk{
  uintptr_t start;
  unsigned long size;
  struct mmap_chunk* next;
  unsigned int type;
}mmap_chunk_t;

int create_mmap(void);

mmap_t mmap;

#endif /* _MM_MMAP_H */
