#ifndef PRINTF_H
#define PRINTF_H

#include "../stdlib/stdint.h"
#include "sys.h"
uint32_t strlen(char *str);

#define PRINTF_STATE_NORMAL 0
#define PRINTF_STATE_LENGTH 1
#define PRINTF_STATE_LENGTH_SHORT 2
#define PRINTF_STATE_LENGTH_LONG 3
#define PRINTF_STATE_SPEC 4
#define PRINTF_LENGTH_DEFAULT 0
#define PRINTF_LENGTH_SHORT_SHORT 1
#define PRINTF_LENGTH_SHORT 2
#define PRINTF_LENGTH_LONG 3
#define PRINTF_LENGTH_LONG_LONG 4

void _puts(char *str);
void _putc(char c);
void puts_f(char far *str);

int *printf_number(int *argp, int length, bool sign, int radix);

void _cdecl printf(const char *fmt, ...);
void _cdecl fprintf(const char far *fmt, ...);

#endif
