#include "main.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int convert_di(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_b(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_u(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_o(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);

/**
 * convert_di - Converts an argument to a signed int and
 *              stores it to a buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_di(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	long int d, cpy;
	unsigned int rend = 0, count = 0;
	char pad, space = ' ', neg = '-', plus = '+';

	if (len == LONG)
		d = va_arg(args, long int);
	else
		d = va_arg(args, int);
	if (len == SHORT)
		d = (short)d;

	/* Handle space flag */
	if (SPACE_FLAG == 1 && d >= 0)
		rend += _memcpy(output, &space, 1);

	if (prec <= 0 && NEG_FLAG == 0) /* Handle width  */
	{
		if (d == LONG_MIN)
			count += 19;
		else
		{
			for (cpy = (d < 0) ? -d : d; cpy > 0; cpy /= 10)
				count++;
		}
		count += (d == 0) ? 1 : 0;
		count += (d < 0) ? 1 : 0;
		count += (PLUS_FLAG == 1 && d >= 0) ? 1 : 0;
		count += (SPACE_FLAG == 1 && d >= 0) ? 1 : 0;

		/* Handle plus flag when zero flag is active */
		if (ZERO_FLAG == 1 && PLUS_FLAG == 1 && d >= 0)
			rend += _memcpy(output, &plus, 1);
		/*Print negative sign when zero flag is active */
		if (ZERO_FLAG == 1 && d < 0)
			rend += _memcpy(output, &neg, 1);

		pad = (ZERO_FLAG == 1) ? '0' : ' ';
		for (width -= count; width > 0; width--)
			rend += _memcpy(output, &pad, 1);
	}

	/* Print negative sign when zero flag is not active */
	if (ZERO_FLAG == 0 && d < 0)
		rend += _memcpy(output, &neg, 1);
	/* Handle plus flag when zero flag is not active */
	if (ZERO_FLAG == 0 && (PLUS_FLAG == 1 && d >= 0))
		rend += _memcpy(output, &plus, 1);

	if (!(d == 0 && prec == 0))
		rend += convert_sbase(output, d, "0123456789",
				flags, 0, prec);

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_b - Converts an unsigned int argument to binary
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
unsigned int convert_b(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	unsigned int num;

	num = va_arg(args, unsigned int);

	(void)len;

	return (convert_ubase(output, num, "01", flags, width, prec));
}

/**
 * convert_o - Converts an unsigned int to octal and
 *             stores it to a buffer contained in a struct.
 * @args: A va_list poinitng to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_o(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	unsigned long int num;
	unsigned int rend = 0;
	char zero = '0';

	if (len == LONG)
		num = va_arg(args, unsigned long int);
	else
		num = va_arg(args, unsigned int);
	if (len == SHORT)
		num = (unsigned short)num;

	if (HASH_FLAG == 1 && num != 0)
		rend += _memcpy(output, &zero, 1);

	if (!(num == 0 && prec == 0))
		rend += convert_ubase(output, num, "01234567",
				flags, width, prec);

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_u - Converts an unsigned int argument to decimal and
 *               stores it to a buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_u(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	unsigned long int num;
	unsigned int rend = 0;

	if (len == LONG)
		num = va_arg(args, unsigned long int);
	else
		num = va_arg(args, unsigned int);
	if (len == SHORT)
		num = (unsigned short)num;

	if (!(num == 0 && prec == 0))
		rend += convert_ubase(output, num, "0123456789",
				flags, width, prec);

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}
