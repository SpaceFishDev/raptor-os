#ifndef FS_H

#define FS_H

#include "../stdlib/stdint.h"

typedef struct
{
	int cyllinder;
	int head;
	int sector;
} chs_t;

char write_bytes_to_disk(short drive, int addr, char* data, int size);
char read_bytes_from_disk(short drive, int addr, char* data, int size);
char _cdecl write_sect(short drive, short c, short s, short h, short sectors,
					   char* data);
char _cdecl read_sect(uint8_t drive, uint16_t cylinder, uint16_t sector,
					  uint16_t head, uint8_t count, void far* dataOut);

void _cdecl reset_disk(uint8_t disk);

#endif