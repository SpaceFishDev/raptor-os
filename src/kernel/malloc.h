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
void memset(char far* ptr, uint8_t value, size_t num);
void memcpy(char far* ptr, char far* value, size_t num);
void reverse(char far* ptr, size_t num);
void realloc(char far** ptr, size_t original_size, size_t new_size);

#endif
