#ifndef FS_H

#define FS_H

#include "../stdlib/stdint.h"

typedef struct
{
	int cyllinder;
	int head;
	int sector;
} chs_t;

char write_bytes_to_disk(short drive, int addr, char far *data, int size);
char read_bytes_from_disk(short drive, int addr, char far *data, int size);
char _cdecl write_sect(short drive, short c, short s, short h, short sectors,
					   char far *data);
char _cdecl read_sect(uint8_t drive, uint16_t cylinder, uint16_t sector,
					  uint16_t head, uint8_t count, void far *dataOut);

void _cdecl reset_disk(uint8_t disk);

typedef struct
{
	size_t size_of_content;
	char far *title;
	char far *content;
} file_t;

typedef struct
{
	uint16_t used_sectors;
	size_t used_bytes;
	file_t far *files;
	size_t n_file;
	int disk_id;
} disk_t;

disk_t far *init_disk(int disk_id);
size_t read_file(disk_t far *disk, char far *buffer, char far *name);
char far *read_null_terminated_file(disk_t far *disk, char far *name);
void write_file(disk_t far *disk, char far *name, char far *data,
				size_t size_of_file);

void flush_disk(disk_t far **disk);

void remove_file(disk_t far* disk, char far* name);

#endif
