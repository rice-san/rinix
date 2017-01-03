#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void print(const char* data, size_t data_length)
{
		for( size_t i = 0; i < data_length; i++ )
			putchar((int) ((const unsigned char*) data)[i]);
}

// Taken from "The C Programming Language"
static void reverse(char s[])
{
	int i, j;
	char c;
	
	for(i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

// Adapted from "The C Programming Language"
static void itoa(int num, char *buffer, int base)
{
	if(base <= 0 || base > 35)
		return;
	
	int i, sign = 0;
	
	char *digits = "0123456789abcdefghijklmnopqrstuvwxyz"; //Base can be from 1-35;
	
	if ((sign = num) < 0)	// Get the sign
		num = -num;	// Make it positive
	i = 0;
	// Go to work
	do {
		buffer[i++] = digits[num % base];
	}while ((num /= base) > 0);
	if (sign < 0)
		buffer[i++] = '-';
	buffer[i] = '\0';
	reverse(buffer);
}

// Unsigned version of itoa
static void utoa(unsigned int num, char *buffer, int base)
{
	if(base <= 0 || base > 35)
		return;
	
	int i;
	
	char *digits = "0123456789abcdefghijklmnopqrstuvwxyz"; //Base can be from 1-35;
	
	i = 0;
	// Go to work
	do {
		buffer[i++] = digits[num % base];
	}while ((num /= base) > 0);
	buffer[i] = '\0';
	reverse(buffer);
}

static void ltoa(long num, char *buffer, int base)
{
	if(base <= 0 || base > 35)
		return;
	
	int i, sign;
	
	char *digits = "0123456789abcdefghijklmnopqrstuvwxyz"; //Base can be from 1-35;
	
	if ((sign = num) < 0)	// Get the sign
		num = -num;	// Make it positive
	i = 0;
	// Go to work
	do {
		buffer[i++] = digits[num % base];
	}while ((num /= base) > 0);
	buffer[i] = '\0';
	reverse(buffer);
}

static void ultoa(unsigned long num, char *buffer, int base)
{
	if(base <= 0 || base > 35)
		return;
	
	int i;
	
	char *digits = "0123456789abcdefghijklmnopqrstuvwxyz"; //Base can be from 1-35;
	
	i = 0;
	// Go to work
	do {
		buffer[i++] = digits[num % base];
	}while ((num /= base) > 0);
	buffer[i] = '\0';
	reverse(buffer);
}

int vprintf(const char* restrict format, va_list param)
{
		int written = 0;
		size_t amount;
		bool rejected_bad_specifier = false;
		
		while ( *format != '\0' )
		{
				if( *format != '%' )
				{
						print_c:
							amount = 1;
							while (format[amount] && format[amount] != '%' )
								amount++;
							print(format, amount);
							format += amount;
							written += amount;
							continue;
				}
				
				const char* format_begun_at = format;
				
				if( *(++format) == '%')
					goto print_c;
					
				if ( rejected_bad_specifier )
				{
				incomprehensible_conversion:
					rejected_bad_specifier = true;
					format = format_begun_at;
					goto print_c;
				}
				
				if ( *format == 'c' )
				{
						format++;
						char c = (char) va_arg(param, int /* char promotes to int */);
						print(&c, sizeof(c));
				}
				else if ( *format == 's' )
				{
						format++;
						const char* s = va_arg(param, const char*);
						print(s, strlen(s));
				}
				else if ( *format == 'd' || *format == 'i' || *format == 'o') {
						int i = va_arg(param, int);
						int radius;
						if (*format == 'd' || *format == 'i')
						{
							radius = 10;
						}
						else if (*format == 'o')
						{
							radius = 8;
						}
						else
						{
							radius = 10;
						}
						format++;
						char s[64];
						if (radius == 10)
						{
							itoa(i, s, radius);
						}
						else
						{
							utoa(i, s, radius);
						}
						print(s, strlen(s));
				}
				else if ( *format == 'x' || *format == 'l' )
				{
						long l = va_arg(param, long);
						int radius;
						if (*format == 'x')
						{
							radius = 16;
							print("0x", (size_t)2);
						}
						else
						{
							radius = 10;
						}
						format++;
						char s[64];
						if (radius == 10)
						{
							ltoa(l, s, radius);
						}
						else
						{
							ultoa(l, s, radius);
						}
						print(s, strlen(s));
				}
				else
				{
						goto incomprehensible_conversion;
				}
				
		}
		
		return written;
}
