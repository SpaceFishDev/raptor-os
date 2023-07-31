#include "interrupts.h"

#include "devices.h"

void _cdecl set_safe_point()
{
	_asm
	{
		push bx
		mov bx, safe_point
		push [bp]
		call set_safepoint
		pop bx
	}
}

void panic(char* msg)
{
	char str[] = "KERNEL PANIC:\n";
	send_bytes(0, str, sizeof(str));
	while (*msg)
	{
		send_byte(0, *msg);
		msg++;
	}
	panic_has_happened = true;
	TO_SAFETY;
}

void div0_handler() { panic("DIVIDE BY 0 ERROR."); }
