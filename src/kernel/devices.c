#include "devices.h"

#include "io/io.h"
#include "sys.h"

void send_byte(int device, char b)
{
	switch (device)
	{
		case device_telletype:
		{
			if (b == '\n')
			{
				putc(15, '\r');
			}
			putc(15, b);
		}
		break;
		case device_floppy_index:
		{
			outb(8, b);
		}
		break;
		case device_floppy_enable:
		{
			outb(24, b);
		}
		break;
	}
}
void send_bytes(int device, char* b, int size)
{
	switch (device)
	{
		case device_telletype:
		{
			if (b[0] != 0xFF)
			{
				for (int i = 0; i < size; i++)
				{
					send_byte(device, b[i]);
				}
				return;
			}
			if (size < 3)
			{
				return;
			}
			for (int i = 2; i != size; ++i)
			{
				putc(b[1], b[i]);
			}
		}
		break;
	}
}
