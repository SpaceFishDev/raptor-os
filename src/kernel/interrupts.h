#ifndef INTERRUPTS_H

#define INTERRUPTS_H

#define bool unsigned char
#define true 1
#define false 0

bool panic_has_happened = false;
void* safe_point = 0x00;
void _cdecl set_safepoint();
void _cdecl set_safe_point();
void _cdecl jump(void* ptr);
void div0_handler();
void critical_error_handler();
void _cdecl set_ivt(short interrupt, void* handler);
void panic(char* msg);
void install_interrupt(short interrupt, void* handler);

#endif
