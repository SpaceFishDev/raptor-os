#include "interrupts.h"

#include "devices.h"
#include "io/io.h"
#include "keyboard.h"
#include "malloc.h"
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

void panic(char *msg)
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

void install_interrupt(short int_, void *handler)
{
	set_ivt(int_ * 4, handler);
}

char _cdecl int21H(uint16_t ax)
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
			printf("%c", k);
			return k;
		}
		break;
		case 0x02:
		{
			uint8_t dl = (char)get_register(DX);
			printf("%c", dl);
		}
		break;
		case 0x08:
		{
			char k = waitk();
			return k;
		}
		case 0x09:
		{
			uint16_t ds = get_register(DS);
			uint16_t bx = get_register(BX);
			int str = (str | ds) >> 8;
			str = (str | bx);
			char far *str_ptr = str;
			printf("%d\n", (int)str);
		}
		break;
		case 0x25:	// CUSTOM!!! (check if key is currently pressed)
		{
			// DL = KEY
			// RETURNS IF THE KEY IS PRESSED
			// RETURN IS IN AL.
			uint8_t dl = get_register(DX);
			uint8_t buf;
			bool p = getk(&buf);
			if (dl == buf)
			{
				return p;
			}
			return false;  // p saved in al
		}
		case 0xBF:	// brainfuck interpreter!!! it takes in input until '$' is
					// pressed. (ALSO CUSTOM!)
		{
			BF_Interpreter();
			printf("\n--------------------\nPRESS A KEY TO EXIT.");
			char a;
			waitk(&a);
			video_mode(0x13);
			printf("WELCOME TO RAPTOR-OS!");
		}
	}
}

void interpret_bf(char far *buffer)
{
	int far *stack = malloc(1024 * sizeof(int));
	memset((char far *)stack, 0, sizeof(int) * 1024);
	int stack_ptr = 0;
	int pos = 0;
	int mempos = 0;
	char far *mem = malloc(1000);
	memset(mem, 0, 1000);
	while (buffer[pos])
	{
		switch (buffer[pos])
		{
			case '+':
			{
				mem[mempos]++;
			}
			break;
			case '-':
			{
				mem[mempos]--;
			}
			break;
			case '.':
			{
				printf("%c", mem[mempos]);
			}
			break;
			case ',':
			{
				char k = waitk();
				mem[mempos] = k;
			}
			break;
			case '>':
			{
				++mempos;
			}
			break;
			case '<':
			{
				--mempos;
			}
			break;
			case '[':
			{
				stack_ptr++;
				stack[stack_ptr] = pos;
			}
			break;
			case ']':
			{
				if (mem[mempos] != 0)
				{
					pos = stack[stack_ptr] - 1;
					--stack_ptr;
				}
			}
			break;
		}
		++pos;
	}
	free(stack);
	free(buffer);
	free(mem);
}

void BF_Interpreter()
{
	video_mode(0x13);
	char far *buffer = malloc(1024);  // 1KB is MORE THAN ENOUGH!
	memset(buffer, 0, 1024);
	int buffer_idx = 0;
	char row = 1;
	char column = 0;
	bool shift_pressed = false;
	printf("RAPTOR-OS BF INTERPRETER. '$' TO EXECUTE");

	while (true)
	{
		char key = 0;
		bool key_press = getk(&key);
		update_keyboard();
		if (keyboard_state['4'] && keyboard_state[LEFT_SHIFT])
		{
			while (keyboard_state['4'] && keyboard_state[LEFT_SHIFT])
			{
				update_keyboard();
			}
			break;
		}
		if (key_press)
		{
			key_press = getk(&key);
			update_keyboard();
			if (column >= 40)
			{
				++row;
				column = 0;
			}
			if (shift_pressed)
			{
				shift_pressed = keyboard_state[LEFT_SHIFT];
			}
			switch (key)
			{
				case '\n':
				{
					column = 0;
					row++;
				}
				break;
				case LEFT_SHIFT:
				{
					continue;
				}
				case BACKSPACE:
				{
					--buffer_idx;
					if (column == 0)
					{
						row--;
						column = 40;
					}
					else
					{
						--column;
					}
					buffer[buffer_idx] = ' ';

					_asm
						{
								mov dl, [column]
								mov dh, [row]
								mov ah, 0x02
								mov bh, 0
								int 10h
						}
					printf("%c", ' ');
					_asm
					{
								mov dl, [column]
								mov dh, [row]
								mov ah, 0x02
								mov bh, 0
								int 10h
					}
				}
				break;
				case '.':
				{
					update_keyboard();
					if (keyboard_state[LEFT_SHIFT])
					{
						key = '>';
					}
					buffer[buffer_idx++] = key;
					printf("%c", key);
					++column;
					while (keyboard_state['.'] && keyboard_state[LEFT_SHIFT])
					{
						update_keyboard();
					}
				}
				break;
				case ',':
				{
					update_keyboard();
					if (keyboard_state[LEFT_SHIFT])
					{
						key = '<';
					}
					buffer[buffer_idx++] = key;
					printf("%c", key);
					++column;
					while (keyboard_state[','] && keyboard_state[LEFT_SHIFT])
					{
						update_keyboard();
					}
				}
				break;
				case '=':
				{
					update_keyboard();
					if (keyboard_state[LEFT_SHIFT])
					{
						key = '+';
					}
					buffer[buffer_idx++] = key;
					printf("%c", key);
					++column;
					while (keyboard_state['='] && keyboard_state[LEFT_SHIFT])
					{
						update_keyboard();
					}
				}
				break;
				default:
				{
					buffer[buffer_idx++] = key;
					printf("%c", key);
					++column;
				}
				break;
			}
			char k = key;
			while (key_press && k == key)
			{
				key_press = getk(&k);
			}
		}
	}
	printf("\n\n-------\nOUTPUT:\n");
	interpret_bf(buffer);
}
