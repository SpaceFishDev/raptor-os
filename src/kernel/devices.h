#ifndef DEVICES_H

#define DEVICES_H

enum DEVICES
{
	device_telletype,
	device_floppy_index,
	device_floppy_enable,
	device_keyboard,
};

void send_byte(int device, char b);
void send_bytes(int device, char* bytes, int size);
void read_byte(int device, char* buf);
void read_bytes(int device, char* buf, int size);

#endif
