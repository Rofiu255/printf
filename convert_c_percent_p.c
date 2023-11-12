#include "main.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int convert_c(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_percent(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_p(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);

/**
 * convert_c - Converts an argument to an unsigned char and
 *             stores it to a buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_c(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	char c;
	unsigned int rend = 0;

	(void)prec;
	(void)len;

	c = va_arg(args, int);

	rend += print_width(output, rend, flags, width);
	rend += _memcpy(output, &c, 1);
	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_percent - Stores a percent sign to a
 *                   buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer (always 1).
 */
unsigned int convert_percent(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	char percent = '%';
	unsigned int rend = 0;

	(void)args;
	(void)prec;
	(void)len;

	rend += print_width(output, rend, flags, width);
	rend += _memcpy(output, &percent, 1);
	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_p - Converts the address of an argument to hex and
 *             stores it to a buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_p(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	char *null = "(nil)";
	unsigned long int addr;
	unsigned int rend = 0;

	(void)len;

	addr = va_arg(args, unsigned long int);
	if (addr == '\0')
		return (_memcpy(output, null, 5));

	flags |= 32;
	rend += convert_ubase(output, addr, "0123456789abcdef",
			flags, width, prec);
	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}
