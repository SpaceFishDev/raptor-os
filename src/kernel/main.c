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
	printf("Welcome to raptor-os!\n");
	disk_t far *disk = init_disk(0x01);
	write_file(disk, mkstr("hi.txt"), mkstr("test!"), 6);
	char far *buffer = malloc(7);
	memset(buffer, 0, 7);
	size_t len = read_file(disk, buffer, mkstr("hi.txt"));
	remove_file(disk, mkstr("hi.txt"));
	flush_disk(&disk);
}
