#include "devices.h"
#include "fs.h"
#include "interrupts.h"
#include "io/io.h"
#include "malloc.h"
#include "printf.h"
#include "sys.h"
#define WIDTH 320
#define HEIGHT 200

void _cdecl main_(char boot_disk)
{
	install_interrupt(0x0, div0_handler);
	install_interrupt(0x24, critical_error_handler);
	video_mode(0x13);
	init_heap();
	printf("Welcome to raptor-kernel!\n");
	char far* buf = malloc(512);
	memset((char far*)buf, 0, 1024);
	((short far*)buf)[0] = 1;
	*((size_t far*)(buf + 2)) = 2;
	*(buf + 6) = 'a';
	*(buf + 7) = 'b';
	*(buf + 8) = 0;
	*(buf + 9) = 'c';
	*(buf + 10) = 'd';
	write_sect(0x01, 0, 1, 0, 1, (char far*)buf);
	disk_t far* disk;
	disk = init_disk(0x01);
	printf("NUM OF FILES=%d\n", disk->n_file);
}
