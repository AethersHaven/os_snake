#ifndef MEM_H
#define MEM_H

typedef unsigned int size_t;
typedef unsigned char bool;

#define NULL ((void *)0)
#define true ((bool)1)
#define false ((bool)0)

/*
all sizes are in blocks (block = 4 bytes)

Header layout (2 blocks):
1 bit: free(1) / allocated(0)
31 bit: chunk size
32 bit: previous chunk size
*/

#define HEAP_START ((void *)0x00FFFFFF) // 16MB
#define HEAP_SIZE ((size_t)0x00FFFFFF)    // 16MB

#define low_16(address) ((unsighed short)((address) & 0xFFFF))
#define high_16(address) ((unsigned short)(((address) >> 16) & 0xFFFF))

void init_heap();

void *malloc(size_t nbytes);
void free(void *address);

void *memcpy(void *dest, void *src, size_t nbytes);
void *memset(void *dest, int value, size_t nbytes);
void *memset8(void *dest, char value, size_t count);
void *memset16(void *dest, short value, size_t count);
void *memset32(void *dest, int value, size_t count);

#endif