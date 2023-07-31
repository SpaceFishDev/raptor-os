#ifndef SYS_H
#define SYS_H

#include "../stdlib/stdint.h"

void sti();
void cli();
void hlt();
void reboot();
void far *_cdecl get_heap();

typedef unsigned long long uint64_t;

void _cdecl div64(uint64_t dividend, uint32_t divisor, uint64_t *quotientOut,
				  uint32_t *remainderOut);

enum REGISTERS
{
	AX,
	DX,
	CX,
	BX,
	AH,
	CH,
};
int get_register(short reg);

void _cdecl outb(uint8_t byte, uint16_t port);
uint8_t _cdecl inb(uint16_t port);

#endif
