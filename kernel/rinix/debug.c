#include <rinix/debug.h>
#include <rinix/vga.h>
#include <rinix/tty.h>
#include <rinix/timer.h>

#include <string.h>

extern size_t term_y;
extern size_t term_x;
extern uint16_t* term_buf;

uint16_t temp_area[80*25];

void flash_screen(void){
#ifdef DEBUG
	memcpy(temp_area, term_buf, VGA_HEIGHT*VGA_WIDTH*sizeof(uint16_t)); // Save screen
	uint8_t flash_color = make_color(COLOR_WHITE, COLOR_WHITE);
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
			for(size_t x = 0; x < VGA_WIDTH; x++)
			{
					term_putentry(' ',flash_color, x,y);
			}
	}
	timer_wait(3);
	memcpy(term_buf, temp_area, VGA_HEIGHT*VGA_WIDTH*sizeof(uint16_t)); // Restore screen
	/*
	flash_color = make_color(COLOR_WHITE, COLOR_BLACK);
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
			for(size_t x = 0; x < VGA_WIDTH; x++)
			{
					term_putentry(' ',flash_color, x,y);
			}
	}
	*/
#endif
}