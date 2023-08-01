#include "fs.h"

#include "io/io.h"
#include "printf.h"

void get_hpc() {}

chs_t LBA_TO_CHS(int lba)
{
	chs_t chs;
	chs.cyllinder = lba / (16 * 63);
	chs.head = (lba / 63) % 16;
	chs.sector = (lba % 63) + 1;
}

char write_bytes_to_disk(short drive, int addr, char far *data, int size)
{
	int sectors = (size + 511) / 512;
	chs_t chs;
	chs = LBA_TO_CHS(addr);
	return write_sect(drive, chs.cyllinder, chs.sector, chs.head, sectors,
					  data);
}
char read_bytes_from_disk(short drive, int addr, char far *data, int size)
{
	int sectors = (size + 511) / 512;
	chs_t chs;
	chs = LBA_TO_CHS(addr);
	return read_sect(drive, chs.cyllinder, chs.sector, chs.head, sectors, data);
}

#include "malloc.h"

file_t far *parse_files(size_t *num_files, char far *buffer, int used_sectors)
{
	char far *real_buffer = buffer + 2;
	file_t far *files = malloc(sizeof(file_t));
	size_t num_file = 0;
	for (int i = 0; i < (used_sectors * 512) - 2;)
	{
		size_t size_of_file = *((size_t far *)real_buffer);
		if (size_of_file == 0)
		{
			break;
		}
		printf("SIZE OF FILE=%d\n", size_of_file);
		real_buffer += 4;
		size_t len_of_name = 0;
		while (*(real_buffer))
		{
			++len_of_name;
			++real_buffer;
		}
		real_buffer -= len_of_name;	 // I really dont know lmao
		char far *name = malloc(len_of_name + 1);
		int n = 0;
		while (*(real_buffer))
		{
			name[n] = *real_buffer;
			++n;
			++real_buffer;
		}
		++real_buffer;
		printf("LENGTH OF NAME=%d\n", len_of_name);
		printf("FILENAME=");
		putsf(15, name);
		printf("\n");
		char far *data = malloc(size_of_file);
		int j = 0;
		while (j < size_of_file)
		{
			data[j] = *real_buffer;
			++j;
			++real_buffer;
		}
		printf("FILEDATA=");
		i += len_of_name + 4 + size_of_file;
		for (int x = 0; x < size_of_file; x++)
		{
			printf("%c", data[x]);
		}
		printf("\n");
		file_t file;
		file.size_of_content = size_of_file;
		file.title = name;
		file.content = data;
		files[num_file] = file;
		++num_file;
		realloc(&((char far *)files), num_file - 1, num_file);
	}
	*num_files = num_file;
	return files;
}

disk_t far *init_disk(int disk_id)
{
	short used_sectors;
	short far *tbuffer = malloc(1024);
	read_sect(disk_id, 0, 1, 0, 1, tbuffer);
	used_sectors = tbuffer[0];

	free(tbuffer);
	disk_t far *disk = malloc(sizeof(disk_t));
	disk->disk_id = disk_id;
	disk->used_sectors = used_sectors;
	char far *buffer = malloc(512 * used_sectors);
	read_sect(disk_id, 0, 1, 0, used_sectors, buffer);
	size_t num_file;
	disk->files = parse_files(&num_file, buffer, disk->used_sectors);
	disk->n_file = num_file;
	return disk;
}
