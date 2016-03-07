#include <rinix/rinix.h>
#include <rinix/tty.h>
#include <rinix/vga.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

// panic.c - Panics for everyone!


// Borrowing some things from the tty.c file!
extern size_t term_x;
extern size_t term_y;
extern uint8_t term_color;
extern uint16_t* term_buf;

// Here's the Best Part!

const char *panic_messages[] =
{
	"You feel like you're going to have a bad time.",
	"Remember! ebx != ebp!",
	"Generic Message",
};



void panic_splash(void)
{
	unsigned int i = 0;
	for(i = 0; i < VGA_WIDTH*VGA_HEIGHT; i++)
	{
	//	term_buf[i] = make_vgaentry(' ', make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
	}
	//term_color = make_color(COLOR_BLACK, COLOR_LIGHT_GREY);
	//term_putat('P', 2, 2);
	//term_putat('A', 3, 2);
	//term_putat('N', 4, 2);
	//term_putat('I', 5, 2);
	//term_putat('C', 6, 2);
	for (i = 0; i<VGA_WIDTH; i++)
	{
	//	term_putentry('=', make_color(COLOR_BLACK, COLOR_LIGHT_GREY), i, 3);
	}
	//term_x = 0;
	//term_y = 4;
	//term_setcolor(make_color(COLOR_RED, COLOR_LIGHT_GREY));
	//printf("%s\n", panic_messages[0]); // Later we'll randomize these messages!
	//term_setcolor(make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
};


void panic(const char *str, ...)
{
	panic_splash();
	//va_list args;
	//va_start(args, str);
	//int i = vprintf(str, args);
	//va_end(args);
	for(;;);
}

