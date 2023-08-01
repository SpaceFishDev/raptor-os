#include "printf.h"

#include "devices.h"

uint32_t strlen(char *str)
{
	uint32_t res;
	for (; *str; ++res)
	{
		++str;
	}
	return res;
}

void _putc(char c) { send_byte(0, c); }

void _puts(char *str)
{
	while (*str)
	{
		_putc(*str);
		++str;
	}
}

void puts_f(char far *str)
{
	while (*str)
	{
		_putc(*str);
		++str;
	}
}

void _cdecl printf(const char *fmt, ...)
{
	int *argp = (int *)&fmt;
	int state = PRINTF_STATE_NORMAL;
	int length = PRINTF_LENGTH_DEFAULT;
	int radix = 10;
	bool sign = false;

	argp++;

	while (*fmt)
	{
		switch (state)
		{
			case PRINTF_STATE_NORMAL:
				switch (*fmt)
				{
					case '%':
						state = PRINTF_STATE_LENGTH;
						break;
					default:
						_putc(*fmt);
						break;
				}
				break;

			case PRINTF_STATE_LENGTH:
				switch (*fmt)
				{
					case 'h':
						length = PRINTF_LENGTH_SHORT;
						state = PRINTF_STATE_LENGTH_SHORT;
						break;
					case 'l':
						length = PRINTF_LENGTH_LONG;
						state = PRINTF_STATE_LENGTH_LONG;
						break;
					default:
						goto PRINTF_STATE_SPEC_;
				}
				break;

			case PRINTF_STATE_LENGTH_SHORT:
				if (*fmt == 'h')
				{
					length = PRINTF_LENGTH_SHORT_SHORT;
					state = PRINTF_STATE_SPEC;
				}
				else
					goto PRINTF_STATE_SPEC_;
				break;

			case PRINTF_STATE_LENGTH_LONG:
				if (*fmt == 'l')
				{
					length = PRINTF_LENGTH_LONG_LONG;
					state = PRINTF_STATE_SPEC;
				}
				else
					goto PRINTF_STATE_SPEC_;
				break;

			case PRINTF_STATE_SPEC:
			PRINTF_STATE_SPEC_:
				switch (*fmt)
				{
					case 'c':
						_putc((char)*argp);
						argp++;
						break;

					case 's':
						if (length == PRINTF_LENGTH_LONG ||
							length == PRINTF_LENGTH_LONG_LONG)
						{
							puts_f(*(const char far **)argp);
							argp += 2;
						}
						else
						{
							_puts(*(const char **)argp);
							argp++;
						}
						break;

					case '%':
						_putc('%');
						break;

					case 'd':
					case 'i':
						radix = 10;
						sign = true;
						argp = printf_number(argp, length, sign, radix);
						break;

					case 'u':
						radix = 10;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;

					case 'X':
					case 'x':
					case 'p':
						radix = 16;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;

					case 'o':
						radix = 8;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;
					default:
						break;
				}

				state = PRINTF_STATE_NORMAL;
				length = PRINTF_LENGTH_DEFAULT;
				radix = 10;
				sign = false;
				break;
		}

		fmt++;
	}
}
void _cdecl fprintf(const char far *fmt, ...)
{
	int *argp = (int *)&fmt;
	int state = PRINTF_STATE_NORMAL;
	int length = PRINTF_LENGTH_DEFAULT;
	int radix = 10;
	bool sign = false;

	argp++;

	while (*fmt)
	{
		switch (state)
		{
			case PRINTF_STATE_NORMAL:
				switch (*fmt)
				{
					case '%':
						state = PRINTF_STATE_LENGTH;
						break;
					default:
						_putc(*fmt);
						break;
				}
				break;

			case PRINTF_STATE_LENGTH:
				switch (*fmt)
				{
					case 'h':
						length = PRINTF_LENGTH_SHORT;
						state = PRINTF_STATE_LENGTH_SHORT;
						break;
					case 'l':
						length = PRINTF_LENGTH_LONG;
						state = PRINTF_STATE_LENGTH_LONG;
						break;
					default:
						goto PRINTF_STATE_SPEC_;
				}
				break;

			case PRINTF_STATE_LENGTH_SHORT:
				if (*fmt == 'h')
				{
					length = PRINTF_LENGTH_SHORT_SHORT;
					state = PRINTF_STATE_SPEC;
				}
				else
					goto PRINTF_STATE_SPEC_;
				break;

			case PRINTF_STATE_LENGTH_LONG:
				if (*fmt == 'l')
				{
					length = PRINTF_LENGTH_LONG_LONG;
					state = PRINTF_STATE_SPEC;
				}
				else
					goto PRINTF_STATE_SPEC_;
				break;

			case PRINTF_STATE_SPEC:
			PRINTF_STATE_SPEC_:
				switch (*fmt)
				{
					case 'c':
						_putc((char)*argp);
						argp++;
						break;
					case 's':
						if (length == PRINTF_LENGTH_LONG ||
							length == PRINTF_LENGTH_LONG_LONG)
						{
							puts_f(*(const char far **)argp);
							argp += 2;
						}
						else
						{
							_puts(*(const char **)argp);
							argp++;
						}
						break;

					case '%':
						_putc('%');
						break;

					case 'd':
					case 'i':
						radix = 10;
						sign = true;
						argp = printf_number(argp, length, sign, radix);
						break;

					case 'u':
						radix = 10;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;

					case 'X':
					case 'x':
					case 'p':
						radix = 16;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;

					case 'o':
						radix = 8;
						sign = false;
						argp = printf_number(argp, length, sign, radix);
						break;
					default:
						break;
				}

				state = PRINTF_STATE_NORMAL;
				length = PRINTF_LENGTH_DEFAULT;
				radix = 10;
				sign = false;
				break;
		}

		fmt++;
	}
}

const char g_HexChars[] = "0123456789abcdef";
int *printf_number(int *argp, int length, bool sign, int radix)
{
	char buffer[32];
	unsigned long long number;
	int number_sign = 1;
	int pos = 0;

	switch (length)
	{
		case PRINTF_LENGTH_SHORT_SHORT:
		case PRINTF_LENGTH_SHORT:
		case PRINTF_LENGTH_DEFAULT:
			if (sign)
			{
				int n = *argp;
				if (n < 0)
				{
					n = -n;
					number_sign = -1;
				}
				number = (unsigned long long)n;
			}
			else
			{
				number = *(unsigned int *)argp;
			}
			argp++;
			break;

		case PRINTF_LENGTH_LONG:
			if (sign)
			{
				long int n = *(long int *)argp;
				if (n < 0)
				{
					n = -n;
					number_sign = -1;
				}
				number = (unsigned long long)n;
			}
			else
			{
				number = *(unsigned long int *)argp;
			}
			argp += 2;
			break;

		case PRINTF_LENGTH_LONG_LONG:
			if (sign)
			{
				long long int n = *(long long int *)argp;
				if (n < 0)
				{
					n = -n;
					number_sign = -1;
				}
				number = (unsigned long long)n;
			}
			else
			{
				number = *(unsigned long long int *)argp;
			}
			argp += 4;
			break;
	}

	do
	{
		uint32_t rem;
		div64(number, radix, &number, &rem);
		buffer[pos++] = g_HexChars[rem];
	} while (number > 0);

	if (sign && number_sign < 0) buffer[pos++] = '-';

	while (--pos >= 0) _putc(buffer[pos]);

	return argp;
}
