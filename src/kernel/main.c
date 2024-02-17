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
	printf("Welcome to raptor-os!\n");
	disk_t far *disk = init_disk(0x01);
	char far *buffer = malloc(32);
	buffer[0] = 0xb4;
	buffer[1] = 0xBF;
	buffer[2] = 0xcd;
	buffer[3] = 0x21;
	write_file(disk, "main.o", buffer, 4);
	load_exe_from_file(disk, "main.o");
}
