#include "main.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int convert_s(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_S(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_r(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);
unsigned int convert_R(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len);

/**
 * convert_s - Converts an argument to a string and
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
unsigned int convert_s(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	char *str, *null = "(null)";
	int size;
	unsigned int rend = 0;

	(void)flags;
	(void)len;

	str = va_arg(args, char *);
	if (str == NULL)
		return (_memcpy(output, null, 6));

	for (size = 0; *(str + size);)
		size++;

	rend += print_string_width(output, flags, width, prec, size);

	prec = (prec == -1) ? size : prec;
	while (*str != '\0' && prec > 0)
	{
		rend += _memcpy(output, str, 1);
		prec--;
		str++;
	}

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_S - Converts an argument to a string and
 *             stores it to a buffer contained in a struct.
 * @args: A va_list pointing to the argument to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 *
 * Description: Non-printable characteres (ASCII values < 32 or >= 127)
 *              are stored as \x followed by the ASCII code value in hex.
 */
unsigned int convert_S(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	char *str, *null = "(null)", *hex = "\\x", zero = '0';
	int size, i;
	unsigned int rend = 0;

	(void)len;
	str = va_arg(args, char *);
	if (str == NULL)
		return (_memcpy(output, null, 6));

	for (size = 0; str[size];)
		size++;

	rend += print_string_width(output, flags, width, prec, size);

	prec = (prec == -1) ? size : prec;
	for (i = 0; *(str + i) != '\0' && i < prec; i++)
	{
		if (*(str + i) < 32 || *(str + i) >= 127)
		{
			rend += _memcpy(output, hex, 2);
			if (*(str + i) < 16)
				rend += _memcpy(output, &zero, 1);
			rend += convert_ubase(output, *(str + i),
					     "0123456789ABCDEF", flags, 0, 0);
			continue;
		}
		rend += _memcpy(output, (str + i), 1);
	}

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_r - Reverses a string and stores it
 *             to a buffer contained in a struct.
 * @args: A va_list pointing to the string to be reversed.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A length modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_r(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	char *str, *null = "(null)";
	int size, end, i;
	unsigned int rend = 0;

	(void)flags;
	(void)len;

	str = va_arg(args, char *);
	if (str == NULL)
		return (_memcpy(output, null, 6));

	for (size = 0; *(str + size);)
		size++;

	rend += print_string_width(output, flags, width, prec, size);

	end = size - 1;
	prec = (prec == -1) ? size : prec;
	for (i = 0; end >= 0 && i < prec; i++)
	{
		rend += _memcpy(output, (str + end), 1);
		end--;
	}

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}

/**
 * convert_R - Converts a string to ROT13 and stores
 *             it to a buffer contained in a struct.
 * @args: A va_list pointing to the string to be converted.
 * @flags: Flag modifiers.
 * @width: A width modifier.
 * @prec: A precision modifier.
 * @len: A lenth modifier.
 * @output: A buffer_t struct containing a character array.
 *
 * Return: The number of bytes stored to the buffer.
 */
unsigned int convert_R(va_list args, buffer_t *output,
		unsigned char flags, int width, int prec, unsigned char len)
{
	char *alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *rot13 = "nopqrstuvwxyzabcdefghijklmNOPQRSTUVWXYZABCDEFGHIJKLM";
	char *str, *null = "(null)";
	int i, j, size;
	unsigned int rend = 0;

	(void)flags;
	(void)len;

	str = va_arg(args, char *);
	if (str == NULL)
		return (_memcpy(output, null, 6));

	for (size = 0; *(str + size);)
		size++;

	rend += print_string_width(output, flags, width, prec, size);

	prec = (prec == -1) ? size : prec;
	for (i = 0; *(str + i) != '\0' && i < prec; i++)
	{
		for (j = 0; j < 52; j++)
		{
			if (*(str + i) == *(alpha + j))
			{
				rend += _memcpy(output, (rot13 + j), 1);
				break;
			}
		}
		if (j == 52)
			rend += _memcpy(output, (str + i), 1);
	}

	rend += print_neg_width(output, rend, flags, width);

	return (rend);
}
