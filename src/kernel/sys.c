#include "sys.h"

#include "printf.h"

void cli()
{
	_asm
		{
		cli
		}
	;
}

void sti()
{
	_asm
		{
		sti
		}
	;
}

void hlt()
{
	cli();
	_asm
		{
		hlt
		}
	;
}

void reboot()
{
	cli();
	_asm {
		mov ax, 0x00
		mov es, ax
		mov ds, ax
		jmp es:0xFFFF
	}
	;
}

int get_register(short reg)
{
	switch (reg)
	{
		case AX:
		{
			_asm
				{
					mov [reg], ax
				}
			;
		}
		break;
		case DX:
		{
			_asm
				{
					mov [reg], dx
				}
			;
		}
		break;
		case CX:
		{
			_asm
				{
				mov [reg], cx
				}
			;
		}
		break;
		case AH:
		{
			char r2 = 0;
			_asm
				{
				mov [r2], ah
				}
			;
			return r2;
		}
		break;
	}
	return reg;
}
