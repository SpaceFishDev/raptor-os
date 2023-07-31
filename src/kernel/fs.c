#include "fs.h"

#include "printf.h"

void get_hpc() {}

char write_bytes_to_disk(short drive, int addr, char *data, int size)
{
	int sectors = (size + 511) / 512;
	chs_t chs;
	chs.cyllinder = addr / (512 * 64);
	int temp = addr % (512 * 64);
	chs.head = temp / 64;
	chs.sector = temp % 64 + 1;
	char dat[512 * 8] = {'a'};
	for (int i = 0; i < size; i++)
	{
		dat[i] = data[i];
	}
	return write_sect(drive, chs.cyllinder, chs.sector, chs.head, sectors,
					  data);
}
char read_bytes_from_disk(short drive, int addr, char *data, int size)
{
	int sectors = (size + 511) / 512;
	chs_t chs;
	chs.cyllinder = addr / (512 * 64);
	int temp = addr % (512 * 64);
	chs.head = temp / 64;
	chs.sector = temp % 64 + 1;
	printf("%p\n", data);
	return read_sect(drive, chs.cyllinder, chs.sector, chs.head, sectors, data);
}
