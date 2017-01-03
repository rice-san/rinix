#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <asm/io.h>
#include <rinix/vga.h>

size_t term_y;
size_t term_x;
uint8_t term_color;
uint16_t* term_buf;

static void update_cursor(void)
{
		size_t csr_pos = term_y * VGA_WIDTH + term_x;
		outb(0x3D4, 14);
		outb(0x3D5, csr_pos >> 8);
		outb(0x3D4, 15);
		outb(0x3D5, csr_pos);
}

static void term_scroll(void)
{
	if (term_y >= 25)
	{
		int i;
		for(i = 0*80; i < 24*80; i++) {
			term_buf[i] = term_buf[i+80]; 
		}
		
		for(i = 24*80; i < 25*80; i++) {
			term_buf[i] = make_vgaentry(' ', term_color);
		}
		term_x = 0;
		term_y = 24;
	}
}

void term_init(void)
{
	term_y = 0;
	term_x = 0;
	term_color = make_color(COLOR_WHITE, COLOR_WHITE);
	term_buf = VGA_MEMORY;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
			for(size_t x = 0; x < VGA_WIDTH; x++)
			{
					const size_t index = y * VGA_WIDTH + x;
					term_buf[index] = make_vgaentry(' ', term_color);
			}
	}
	term_color = make_color(COLOR_WHITE, COLOR_BLACK);
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
			for(size_t x = 0; x < VGA_WIDTH; x++)
			{
					const size_t index = y * VGA_WIDTH + x;
					term_buf[index] = make_vgaentry(' ', term_color);
			}
	}
	term_y = 0;
	term_x = 0;
}

void term_setcolor(uint8_t color)
{
		term_color = color;
}

void term_putat(char c, size_t x, size_t y)
{
	if((x >= VGA_WIDTH) || (y >= VGA_HEIGHT))
	{
		return; // Ignore anything that goes off the screen
	}
	const size_t index = y * VGA_WIDTH + x;
	term_buf[index] = make_vgaentry(c, term_color);
}

void term_putentry(char c, uint8_t color, size_t x, size_t y)
{
		const size_t index = y * VGA_WIDTH + x;
		term_buf[index] = make_vgaentry(c, color);
}

void term_putchar(char c)
{	
	if(c == '\n')
	{
			++term_y;
			term_x = 0;
	}
	
	else if(c == '\r')
	{
			term_x = 0;
	}
	
	// TODO: Handle tab and backspace characters
	
	else if(c >= ' ')
	{
		term_putentry(c, term_color, term_x, term_y);
		if (++term_x == VGA_WIDTH)
		{
			term_x = 0;
			++term_y;
		}
	}
	
	term_scroll();
	update_cursor();
}

void term_write(const char* data, size_t size)
{
		for (size_t i = 0; i < size; i++)
				term_putchar(data[i]);
}

void term_writestr(const char* data)
{
		term_write(data, strlen(data));
}

