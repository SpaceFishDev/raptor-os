#include "interrupts.h"

#include "devices.h"
#include "printf.h"
#include "sys.h"

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
