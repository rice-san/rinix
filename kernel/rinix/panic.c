#include <rinix/rinix.h>
#include <rinix/timer.h>
#include <rinix/tty.h>
#include <rinix/vga.h>
#include <rinix/version.h>
#include <rinix/config.h>

#include <arch/info.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// panic.c - Panics for everyone!


// Borrowing some things from the tty.c file!
extern size_t term_x;
extern size_t term_y;
extern uint8_t term_color;
extern uint16_t* term_buf;

// Here's the Best Part!

#define NUM_OF_PANIC_STRINGS 3

const char *panic_messages[] =
{
	"You feel like you're going to have a bad time.",
	"Remember! ebx != ebp!",
	"Generic Message",
};

static unsigned int message_select()
{
	return (unsigned int)(get_ticks() % NUM_OF_PANIC_STRINGS);
}

void panic_splash(void)
{
	/*
	unsigned int i = 0;
	for(i = 0; i < VGA_WIDTH*VGA_HEIGHT; i++)
	{
		term_buf[i] = make_vgaentry(' ', make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
	}
	term_color = make_color(COLOR_BLACK, COLOR_LIGHT_GREY);
	
	term_x = 0;
	term_y = 0;
	term_writestr("==================================Kernel Panic");
	
	term_x = 33;
	term_y = 0;
	term_writestr("Kernel Panic");
	term_x = 80 - (strlen(_RINIX_VERSION_STRING) + 6 + strlen(ARCH_NAME) + 1);
	unsigned int fill_space = term_x - 46;
	printf("rinix-%s %s", ARCH_NAME,_RINIX_VERSION_STRING);
	term_x = 46;
	
	for (i = 0; i<fill_space; i++)
	{
		term_putentry('=', make_color(COLOR_BLACK, COLOR_LIGHT_GREY), i+46, 0);
	}
	
	for (i=0; i<VGA_WIDTH; i++)
	{
		term_putat('=', i, 1);
	}
	term_x = 0;
	term_y = 2;
	term_setcolor(make_color(COLOR_RED, COLOR_LIGHT_GREY));
	char* selected_str = panic_messages[message_select()];
	term_x = ((VGA_WIDTH - strlen(selected_str)) / 2);
	printf("%s\n", selected_str); // Later we'll randomize these messages!
	term_setcolor(make_color(COLOR_BLACK, COLOR_LIGHT_GREY));
	*/
	printf("\n");
	printf("panic: ");
};


static void print_panic_joke(void)
{
	#ifdef PANIC_JOKE
	//term_setcolor(make_color(COLOR_RED, COLOR_BLACK));
	char* selected_str = panic_messages[message_select()];
	term_x = ((VGA_WIDTH - strlen(selected_str)) / 2);
	printf("\n%s", selected_str); // Later we'll randomize these messages!
	term_setcolor(make_color(COLOR_LIGHT_GREY, COLOR_BLACK));
	printf("\n");
	#endif
}

void panic(const char *str, ...)
{
	panic_splash();
	va_list args;
	va_start(args, str);
	int i = vprintf(str, args);
	va_end(args);
	print_panic_joke();
	for(;;);
}

