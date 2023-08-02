#include "malloc.h"

#include "../stdlib/stdint.h"
#include "printf.h"

size_t num_free;
size_t num_block;

char far* mkstr(char* str)
{
	char far* new_str = malloc(strlen(str) + 1);
	memset(new_str, 0, strlen(str) + 1);
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
		(free_block_t far*)((int)get_heap() + 512 * sizeof(block_t));
	heap_addr = (void far*)((int)get_heap() + sizeof(block_t) * 512 +
							sizeof(free_block_t) * 512);
	free_heap_blocks[0].ptr = heap_addr;
	free_heap_blocks[0].size = 60 * 1024;
}

void far* malloc(size_t size)
{
	if (num_block > 512)
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

void memset(char far* ptr, uint8_t value, size_t num)
{
	for (size_t i = 0; i < num; ++i)
	{
		ptr[i] = value;
	}
}

void reverse(char far* ptr, size_t num)
{
	int i = num;
	for (int j = 0; j < num; ++j)
	{
		char temp = ptr[j];
		ptr[j] = ptr[i];
		ptr[i] = temp;
		--i;
	}
}

void realloc(char far** ptr, size_t original_size, size_t new_size)
{
	char far* ptr_new = malloc(new_size);
	memcpy(ptr_new, *ptr, original_size);
	free(*ptr);
	*ptr = ptr_new;
}

void memcpy(char far* ptr, char far* ptr2, size_t num)
{
	for (int i = 0; i < num; ++i)
	{
		ptr[i] = ptr2[i];
	}
}

bool memcmp(char far* ptr, char far* data, size_t size)
{
	for (int i = 0; i < size; ++i)
	{
		if (ptr[i] != data[i])
		{
			return false;
		}
	}
	return true;
}

bool strcmp(char far* a, char far* b)
{
	while (*a)
	{
		if (*a != *b)
		{
			return false;
		}
		++a;
		++b;
	}
	return true;
}
