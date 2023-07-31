#include "malloc.h"

#include "../stdlib/stdint.h"
#include "printf.h"

size_t num_free;
size_t num_block;

char far* mkstr(char* str)
{
	char far* new_str = malloc(strlen(str));
	int i = 0;
	while (*str)
	{
		new_str[i++] = *str;
		++str;
	}
	return new_str;
}

void init_heap()
{
	num_free = 1;
	num_block = 0;
	heap_blocks = (block_t far*)get_heap();
	free_heap_blocks =
		(free_block_t far*)((int)get_heap() + 256 * sizeof(block_t));
	heap_addr = (void far*)((int)get_heap() + sizeof(block_t) * 256 +
							sizeof(free_block_t) * 256);
	free_heap_blocks[0].ptr = heap_addr;
	free_heap_blocks[0].size = 60 * 1024;
}

void far* malloc(size_t size)
{
	if (num_block > 256)
	{
		return 0;
	}
	for (int i = 0; i < num_free; ++i)
	{
		if (free_heap_blocks[i].size >= size - 1)
		{
			for (int j = i; j < num_free; ++j)
			{
				if (free_heap_blocks[j].size >= size - 1 &&
					free_heap_blocks[j].size < free_heap_blocks[i].size)
				{
					i = j;
					break;
				}
			}
			void far* ptr = free_heap_blocks[i].ptr;
			free_heap_blocks[i].size -= size;
			if (free_heap_blocks[i].size == 0)
			{
				for (int x = i; x < num_free; ++x)
				{
					free_heap_blocks[x] = free_heap_blocks[x + 1];
				}
				--num_free;
			}
			else
			{
				free_heap_blocks[i].ptr += size;
			}
			heap_blocks[num_block].ptr = ptr;
			heap_blocks[num_block].size = size;
			++num_block;
			return ptr;
		}
	}
	return 0;
}

void free(void far* ptr)
{
	for (int i = 0; i < num_block; ++i)
	{
		if ((int)heap_blocks[i].ptr == (int)ptr)
		{
			free_block_t free_b;
			free_b.ptr = ptr;
			free_b.size = heap_blocks[i].size;
			for (int j = i; j < num_block; ++j)
			{
				heap_blocks[j] = heap_blocks[j + 1];
			}
			--num_block;
			free_heap_blocks[num_free] = free_b;
			num_free++;
		}
	}
}

void print_blocks()
{
	printf("USED BLOCKS:\n");
	for (int i = 0; i < num_block; ++i)
	{
		printf("BLOCK = %d\n", i);
		printf("PTR  = %p\n", heap_blocks[i].ptr);
		printf("SIZE = %d\n\n", heap_blocks[i].size);
	}
}
