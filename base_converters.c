#include "main.h"

unsigned int convert_sbase(buffer_t *output, long int num, char *base,
		unsigned char flags, int width, int prec);
unsigned int convert_ubase(buffer_t *output,
		unsigned long int num, char *base,
		unsigned char flags, int width, int prec);

/**
 * convert_sbase - Converts a signed long to an inputted base and stores
 *                 the result to a buffer contained in a struct.
 * @output: A buffer_t struct containing a character array.
 * @num: A signed long to be converted.
 * @base: A pointer to a string containing the base to convert to.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_sbase(buffer_t *output, long int num, char *base,
		unsigned char flags, int width, int prec)
{
	int size;
	char digit, pad = '0';
	unsigned int rend = 1;

	for (size = 0; *(base + size);)
		size++;

	if (num >= size || num <= -size)
		rend += convert_sbase(output, num / size, base,
				flags, width - 1, prec - 1);

	else
	{
		for (; prec > 1; prec--, width--) /* Handle precision */
			rend += _memcpy(output, &pad, 1);

		if (NEG_FLAG == 0) /* Handle width */
		{
			pad = (ZERO_FLAG == 1) ? '0' : ' ';
			for (; width > 1; width--)
				rend += _memcpy(output, &pad, 1);
		}
	}

	digit = base[(num < 0 ? -1 : 1) * (num % size)];
	_memcpy(output, &digit, 1);

	return (rend);
}

/**
 * convert_ubase - Converts an unsigned long to an inputted base and
 *                 stores the result to a buffer contained in a struct.
 * @output: A buffer_t struct containing a character array.
 * @num: An unsigned long to be converted.
 * @base: A pointer to a string containing the base to convert to.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_ubase(buffer_t *output, unsigned long int num, char *base,
		unsigned char flags, int width, int prec)
{
	unsigned int size, rend = 1;
	char digit, pad = '0', *lead = "0x";

	for (size = 0; *(base + size);)
		size++;

	if (num >= size)
		rend += convert_ubase(output, num / size, base,
				flags, width - 1, prec - 1);

	else
	{
		if (((flags >> 5) & 1) == 1) /* Printing a ptr address */
		{
			width -= 2;
			prec -= 2;
		}
		for (; prec > 1; prec--, width--) /* Handle precision */
			rend += _memcpy(output, &pad, 1);

		if (NEG_FLAG == 0) /* Handle width */
		{
			pad = (ZERO_FLAG == 1) ? '0' : ' ';
			for (; width > 1; width--)
				rend += _memcpy(output, &pad, 1);
		}
		if (((flags >> 5) & 1) == 1) /* Print 0x for ptr address */
			rend += _memcpy(output, lead, 2);
	}

	digit = base[(num % size)];
	_memcpy(output, &digit, 1);

	return (rend);
}
