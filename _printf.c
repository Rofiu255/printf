#include "main.h"
#include <stdio.h>
#include <stdlib.h>

void clean_up(va_list args, buffer_t *output);
int printf_run(const char *format, va_list args, buffer_t *output);
int _printf(const char *format, ...);

/**
 * clean_up - Peforms cleanup operations for _printf.
 * @args: A va_list of arguments provided to _printf.
 * @output: A buffer_t struct.
 */
void clean_up(va_list args, buffer_t *output)
{
	va_end(args);
	write(1, output->start, output->len);
	free_buffer(output);
}

/**
 * printf_run - Reads through the format string for _printf.
 * @format: Character string to print - may contain directives.
 * @output: A buffer_t struct containing a buffer.
 * @args: A va_list of arguments.
 *
 * Return: The number of characters stored to output.
 */
int printf_run(const char *format, va_list args, buffer_t *output)
{
	int index, width, prec, rend = 0;
	char tmp;
	unsigned char flags, len;
	unsigned int (*f)(va_list, buffer_t *,
			unsigned char, int, int, unsigned char);

	for (index = 0; *(format + index); index++)
	{
		len = 0;
		if (*(format + index) == '%')
		{
			tmp = 0;
			flags = handle_flags(format + index + 1, &tmp);
			width = handle_width(args, format + index + tmp + 1, &tmp);
			prec = handle_precision(args, format + index + tmp + 1,
					&tmp);
			len = handle_length(format + index + tmp + 1, &tmp);

			f = handle_specifiers(format + index + tmp + 1);
			if (f != NULL)
			{
				index += tmp + 1;
				rend += f(args, output, flags, width, prec, len);
				continue;
			}
			else if (*(format + index + tmp + 1) == '\0')
			{
				rend = -1;
				break;
			}
		}
		rend += _memcpy(output, (format + index), 1);
		index += (len != 0) ? 1 : 0;
	}
	clean_up(args, output);
	return (rend);
}

/**
 * _printf - Outputs a formatted string.
 * @format: Character string to print - may contain directives.
 *
 * Return: The number of characters printed.
 */
int _printf(const char *format, ...)
{
	buffer_t *output;
	va_list args;
	int rend;

	if (format == NULL)
		return (-1);
	output = init_buffer();
	if (output == NULL)
		return (-1);

	va_start(args, format);

	rend = printf_run(format, args, output);

	return (rend);
}
