#include "interrupts.h"

#include "devices.h"
#include "keyboard.h"
#include "printf.h"
#include "sys.h"

#define TO_SAFETY jump(safe_point)
void _cdecl set_safe_point()
{
	_asm
	{
		push bx
		mov bx, [bp]
		mov [safe_point],bx
		pop bx
	}
}

void panic(char* msg)
{
	printf("%s\n%s", "KERNEL PANIC!:", msg);
	panic_has_happened = true;
	TO_SAFETY;
}
void div0_handler()
{
	cli();
	panic("DIVIDE BY 0 ERROR.");
}

void critical_error_handler()
{
	cli();
	printf("%p\n", safe_point);
	panic("CRITICAL ERROR.");
}

void install_interrupt(short int_, void* handler)
{
	set_ivt(int_ * 4, handler);
}

void _cdecl int21H(uint16_t ax)
{
	uint8_t ah = ax >> 8;
	switch (ah)
	{
		case 0x00:
		{
			// exit!!!
		}
		break;
		case 0x01:
		{
			char k = waitk();
			_asm
				{
					mov al, [k]
				}
			;
		}
		break;
		case 0x02:
		{
			uint8_t dl = (char)get_register(DX);
			printf("%c", dl);
		}
		break;
	}
}

