#include "devices.h"
#include "exeloader.h"
#include "fs.h"
#include "interrupts.h"
#include "io/io.h"
#include "keyboard.h"
#include "malloc.h"
#include "printf.h"
#include "sys.h"
#define WIDTH 320
#define HEIGHT 200

void _cdecl main_(char boot_disk)
{
	install_interrupt(0x0, div0_handler);
	install_interrupt(0x21, int21_handler_stage0);
	install_interrupt(0x24, critical_error_handler);
	video_mode(0x13);
	init_heap();
	// b4 02 b2 61 cd 21
	printf("Welcome to raptor-os!\n");
	disk_t far* disk = init_disk(0x01);
	init_keyboard();
	char far* buffer = malloc(6);
	buffer[0] = 0xb4;
	buffer[1] = 0x02;
	buffer[2] = 0xb2;
	buffer[3] = 0x61;
	buffer[4] = 0xcd;
	buffer[5] = 0x21;
	write_file(disk, mkstr("test.o"), buffer, 7);
	load_exe_from_file(disk, mkstr("test.o"));
	flush_disk(disk);
}
