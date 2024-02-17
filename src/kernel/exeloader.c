#include "exeloader.h"

#include "fs.h"
#include "printf.h"

void load_exe_from_file(disk_t far *disk, char far *file)
{
	char far *buffer = malloc(
		1024 *
		16);		  // 16K is enough RIGHT??? its more than the entire kernel...
	buffer[0] = 0x55; // MIGHT BE WRONG! but its push bp
	buffer[1] = 0x89; // mov bp, sp
	buffer[2] = 0xe5; // &^^^^

	size_t size = read_file(disk, buffer + 3, file);

	buffer[size + 3] = 0x89;
	buffer[size + 4] = 0xEC;
	buffer[size + 5] = 0x5d;
	buffer[size + 6] = 0xc3;

	char buf[1024 * 4] = {0}; // try a near pointer??
	for (int i = 0; i < size + 7; ++i)
	{
		buf[i] = buffer[i];
		printf("%x\n", buf[i]);
	}
	call(buf);
}
