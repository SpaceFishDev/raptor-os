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
	install_interrupt(0x21, int21_handler_stage0);
	install_interrupt(0x24, critical_error_handler);
	video_mode(0x13);
	init_heap();
	printf("Welcome to raptor-os!\n");
	char c = waitk();
	_asm
		{
			mov ah, 0x01
			int 0x21
			mov dl, al
			mov ah, 0x02
			int 0x21
		}
	;
}
