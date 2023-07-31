#include "io.h"

void puts(char color, char* str)
{
	for (; *str; str++)
	{
		putc(color, *str);
	}
}
void putsf(char color, char far* str)
{
	for (; *str; str++)
	{
		putc(color, *str);
	}
}
