#ifndef MALLOC_H
#define MALLOC_H

#include "../stdlib/stdint.h"
#include "sys.h"

typedef struct
{
	char far* ptr;
	size_t size;
} free_block_t;

typedef struct
{
	char far* ptr;
	size_t size;
} block_t;

free_block_t far* free_heap_blocks;
block_t far* heap_blocks;

char far* heap_addr;
void init_heap();
void print_blocks();
void far* malloc(size_t size);
void free(void far* ptr);
char far* mkstr(char* str);

#endif
