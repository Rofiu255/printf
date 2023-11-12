#include "main.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char handle_flags(const char *flag, char *i);
unsigned char handle_length(const char *modifier, char *i);
int handle_width(va_list args, const char *modifier, char *i);
int handle_precision(va_list args, const char *modifier, char *i);
unsigned int (*handle_specifiers(const char *specifier))(va_list, buffer_t *,
		unsigned char, int, int, unsigned char);

/**
 * handle_flags - Matches flags with corresponding values.
 * @flag: A pointer to a potential string of flags.
 * @i: An index counter for the original format string.
 *
 * Return: If flag characters are matched - a corresponding value.
 *         Otherwise - 0.
 */
unsigned char handle_flags(const char *flag, char *i)
{
	int index, j;
	unsigned char rend = 0;
	flag_t flags[] = {
		{'+', PLUS},
		{' ', SPACE},
		{'#', HASH},
		{'0', ZERO},
		{'-', NEG},
		{0, 0}
	};

	for (index = 0; flag[index]; index++)
	{
		for (j = 0; flags[j].flag != 0; j++)
		{
			if (flag[index] == flags[j].flag)
			{
				(*i)++;
				if (rend == 0)
					rend = flags[j].value;
				else
					rend |= flags[j].value;
				break;
			}
		}
		if (flags[j].value == 0)
			break;
	}

	return (rend);
}

/**
 * handle_length - Matches length modifiers with their corresponding value.
 * @modifier: A pointer to a potential length modifier.
 * @i: An index counter for the original format string.
 *
 * Return: If a lenth modifier is matched - its corresponding value.
 *         Otherwise - 0.
 */
unsigned char handle_length(const char *modifier, char *i)
{
	if (*modifier == 'h')
	{
		(*i)++;
		return (SHORT);
	}

	else if (*modifier == 'l')
	{
		(*i)++;
		return (LONG);
	}

	return (0);
}

/**
 * handle_width - Matches a width modifier with its corresponding value.
 * @args: A va_list of arguments.
 * @modifier: A pointer to a potential width modifier.
 * @i: An index counter for the original format string.
 *
 * Return: If a width modifier is matched - its value.
 *         Otherwise - 0.
 */
int handle_width(va_list args, const char *modifier, char *i)
{
	int value = 0;

	while ((*modifier >= '0' && *modifier <= '9') || (*modifier == '*'))
	{
		(*i)++;

		if (*modifier == '*')
		{
			value = va_arg(args, int);
			if (value <= 0)
				return (0);
			return (value);
		}

		value *= 10;
		value += (*modifier - '0');
		modifier++;
	}

	return (value);
}

/**
 * handle_precision - Matches a precision modifier with
 *                    its corresponding value.
 * @args: A va_list of arguments.
 * @modifier: A pointer to a potential precision modifier.
 * @i: An index counter for the original format string.
 *
 * Return: If a precision modifier is matched - its value.
 *         If the precision modifier is empty, zero, or negative - 0.
 *         Otherwise - -1.
 */
int handle_precision(va_list args, const char *modifier, char *i)
{
	int value = 0;

	if (*modifier != '.')
		return (-1);

	modifier++;
	(*i)++;

	if ((*modifier <= '0' || *modifier > '9') &&
	     *modifier != '*')
	{
		if (*modifier == '0')
			(*i)++;
		return (0);
	}

	while ((*modifier >= '0' && *modifier <= '9') ||
	       (*modifier == '*'))
	{
		(*i)++;

		if (*modifier == '*')
		{
			value = va_arg(args, int);
			if (value <= 0)
				return (0);
			return (value);
		}

		value *= 10;
		value += (*modifier - '0');
		modifier++;
	}

	return (value);
}

/**
 * handle_specifiers - Matches a conversion specifier with
 *                     a corresponding conversion function.
 * @specifier: A pointer to a potential conversion specifier.
 *
 * Return: If a conversion function is matched - a pointer to the function.
 *         Otherwise - NULL.
 */
unsigned int (*handle_specifiers(const char *specifier))(va_list, buffer_t *,
		unsigned char, int, int, unsigned char)
{
	int index;
	converter_t converters[] = {
		{'c', convert_c},
		{'s', convert_s},
		{'d', convert_di},
		{'i', convert_di},
		{'%', convert_percent},
		{'b', convert_b},
		{'u', convert_u},
		{'o', convert_o},
		{'x', convert_x},
		{'X', convert_X},
		{'S', convert_S},
		{'p', convert_p},
		{'r', convert_r},
		{'R', convert_R},
		{0, NULL}
	};

	for (index = 0; converters[index].func; index++)
	{
		if (converters[index].specifier == *specifier)
			return (converters[index].func);
	}

	return (NULL);
}
