#include "keyboard.h"
#include "malloc.h"
// char kbd_US[128] = {
// 	0,
// 	ESCAPE,
// 	'1',
// 	'2',
// 	'3',
// 	'4',
// 	'5',
// 	'6',
// 	'7',
// 	'8',
// 	'9',
// 	'0',
// 	'-',
// 	'=',
// 	BACKSPACE,
// 	'\t',
// 	'q',
// 	'w',
// 	'e',
// 	'r',
// 	't',
// 	'y',
// 	'u',
// 	'i',
// 	'o',
// 	'p',
// 	'[',
// 	']',
// 	'\n',
// 	LEFT_CNTRL,
// 	'a',
// 	's',
// 	'd',
// 	'f',
// 	'g',
// 	'h',
// 	'j',
// 	'k',
// 	'l',
// 	';',
// 	'\'',
// 	'`',
// 	LEFT_SHIFT,
// 	'\\',
// 	'z',
// 	'x',
// 	'c',
// 	'v',
// 	'b',
// 	'n',
// 	'm',
// 	',',
// 	'.',
// 	'/',
// 	RIGHT_SHIFT,
// 	'*',
// 	LEFT_ALT, /* Alt */
// 	' ',	  /* Space bar */
// 	0,		  /* Caps lock */
// 	0,		  /* 59 - F1 key ... > */
// 	0,
// 	0,
// 	0,
// 	0,
// 	0,
// 	0,
// 	0,
// 	0,
// 	0, /* < ... F10 */
// 	0, /* 69 - Num lock*/
// 	0, /* Scroll Lock */
// 	0, /* Home key */
// 	0, /* Up Arrow */
// 	0, /* Page Up */
// 	'-',
// 	0, /* Left Arrow */
// 	0,
// 	0, /* Right Arrow */
// 	'+',
// 	0, /* 79 - End key*/
// 	0, /* Down Arrow */
// 	0, /* Page Down */
// 	0, /* Insert Key */
// 	0, /* Delete Key */
// 	0,
// 	0,
// 	0,
// 	0, /* F11 Key */
// 	0, /* F12 Key */
// 	0,
// };
char far *KBD_US = (char far *)0;
char far *kbd_US = (char far *)0;
char scan_code_as_ascii(char c) { return kbd_US[c]; }
bool getk(char *key)
{
	char c = inb(0x60);
	if ((c) > 128)
	{
		*key = scan_code_as_ascii(c & ~0x80);
		return false;
	}
	*key = scan_code_as_ascii(c);
	return true;
}

char waitk()
{
	char c;
	while (!getk(&c))
		;
	return c;
}

void update_keyboard()
{
	if (KBD_US == 0)
	{
		KBD_US = malloc(128);
		KBD_US[0] = 0;
		KBD_US[1] = ESCAPE;
		KBD_US[2] = '1';
		KBD_US[3] = '2';
		KBD_US[4] = '3';
		KBD_US[5] = '4';
		KBD_US[6] = '5';
		KBD_US[7] = '6';
		KBD_US[8] = '7';
		KBD_US[9] = '8';
		KBD_US[10] = '9';
		KBD_US[11] = '0';
		KBD_US[12] = '-';
		KBD_US[13] = '=';
		KBD_US[14] = BACKSPACE;
		KBD_US[15] = '\t';
		KBD_US[16] = 'q';
		KBD_US[17] = 'w';
		KBD_US[18] = 'e';
		KBD_US[19] = 'r';
		KBD_US[20] = 't';
		KBD_US[21] = 'y';
		KBD_US[22] = 'u';
		KBD_US[23] = 'i';
		KBD_US[24] = 'o';
		KBD_US[25] = 'p';
		KBD_US[26] = '[';
		KBD_US[27] = ']';
		KBD_US[28] = '\n';
		KBD_US[29] = LEFT_CNTRL;
		KBD_US[30] = 'a';
		KBD_US[31] = 's';
		KBD_US[32] = 'd';
		KBD_US[33] = 'f';
		KBD_US[34] = 'g';
		KBD_US[35] = 'h';
		KBD_US[36] = 'j';
		KBD_US[37] = 'k';
		KBD_US[38] = 'l';
		KBD_US[39] = ';';
		KBD_US[40] = '\'';
		KBD_US[41] = '`';
		KBD_US[42] = LEFT_SHIFT;
		KBD_US[43] = '\\';
		KBD_US[44] = 'z';
		KBD_US[45] = 'x';
		KBD_US[46] = 'c';
		KBD_US[47] = 'v';
		KBD_US[48] = 'b';
		KBD_US[49] = 'n';
		KBD_US[50] = 'm';
		KBD_US[51] = ',';
		KBD_US[52] = '.';
		KBD_US[53] = '/';
		KBD_US[54] = RIGHT_SHIFT;
		KBD_US[55] = '*';
		KBD_US[56] = LEFT_ALT;
		KBD_US[57] = ' ';
		kbd_US = KBD_US;
	}
	char c = inb(0x60);
	if (c > 128)
	{
		keyboard_state[kbd_US[c & ~0x80]] = false;
		return;
	}
	keyboard_state[kbd_US[c]] = true;
}

void init_keyboard()
{
	for (int i = 0; i < 128; ++i)
	{
		keyboard_state[i] = false;
	}
}

bool getc(char key) { return keyboard_state[key]; }
