#include "main.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int convert_x(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_X(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);

/**
 * convert_x - Converts an unsigned int argument to hex using abcdef
 *             and stores it to a buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_x(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	unsigned long int num;
	unsigned int rend = 0;
	char *lead = "0x";

	if (len == LONG)
		num = va_arg(args, unsigned long int);
	else
		num = va_arg(args, unsigned int);
	if (len == SHORT)
		num = (unsigned short)num;

	if (HASH_FLAG == 1 && num != 0)
		rend += _memcpy(output, lead, 2);

	if (!(num == 0 && prec == 0))
		rend += convert_ubase(output, num, "0123456789abcdef",
				flags, width, prec);

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_X - Converts an unsigned int argument to hex using ABCDEF
 *             and stores it to a buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_X(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	unsigned long int num;
	unsigned int rend = 0;
	char *lead = "0X";

	if (len == LONG)
		num = va_arg(args, unsigned long);
	else
		num = va_arg(args, unsigned int);
	if (len == SHORT)
		num = (unsigned short)num;

	if (HASH_FLAG == 1 && num != 0)
		rend += _memcpy(output, lead, 2);

	if (!(num == 0 && prec == 0))
		rend += convert_ubase(output, num, "0123456789ABCDEF",
				flags, width, prec);

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}
