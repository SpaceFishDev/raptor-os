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
	set_ivt(0, div0_handler);
	video_mode(0x13);
	init_heap();
}
