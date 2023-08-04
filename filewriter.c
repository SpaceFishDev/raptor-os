#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	size_t size_of_content;
	char* name;
	char* content;
} file_t;
typedef struct
{
	size_t n_file;
	size_t used_sectors;
	file_t* files;
} disk_t;

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		printf("Please provide atleast two arguments.\n");
		return 0;
	}
	else
	{
		FILE* fp = fopen(argv[1], "rb");
		fseek(fp, 0, SEEK_END);
		size_t len = ftell(fp);
		char* buffer = malloc(len + 1);
		memset(buffer, 0, len + 1);
		fseek(fp, 0, SEEK_SET);
		fread(buffer, len, 1, fp);
	}
}

