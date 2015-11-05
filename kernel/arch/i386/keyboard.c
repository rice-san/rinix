#include <stdio.h>
#include <stdint.h>

#include <asm/io.h>

#include <rinix/pause.h>

#include "irq.h"
#include "regs.h"

#define SHIFT(flag) (flag & 0x0C) // 00|00|11|00
#define CAPSLOCK(flag) (flag & 0x40) // 01|00|00|00
#define NUMLOCK(flag) (flag & 0x20) // 00|10|00|00
#define SCROLLLOCK(flag) (flag & 0x10) // 00|01|00|00

extern int pause_flag;

/* TODO: Add Flag Bits for CAPS LOCK, NUMS LOCK, and SCROLL LOCK */

uint8_t flags = 0;	/* Flags: scroll|nums|caps|lshift|rshift|ctrl|alt */

unsigned char kbdus[256] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};
    // SHIFT CODES
unsigned char kbdus_shift[128] =
{    
      0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', '\b',	/* Backspace */
  '\t',			/* Tab */
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',		/* Enter key */
    0,			/* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '~',   0,		/* Left shift */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', '<', '>', '?',   0,					/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};


/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r)
{
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);
	
	
    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80)
    {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
      if((scancode & 0x7F) == 0x2A) /* Left Shift */
      {
			  flags &= 0xF7; /* 01|11 */
		  }
		  else if((scancode & 0x7F) == 0x36) /* Right Shift */
		  {
			  flags &= 0xFA; /* 10|11 */
		  }
		  else if((scancode & 0x7F) == 0x1D) /* Control */
		  {
			  flags &= 0xFD; /* 11|01 */
		  }
		  else if((scancode & 0x7F) == 0x38) /* Alt */
		  {
			  flags &= 0xFE; /* 11|10 */
		  }
    }
    else
    {
		  if((scancode & 0x7F) == 0x2A)
		  {
		  	flags |= 0x08; /* Left Shift */
		  }
		  else if((scancode & 0x7F) == 0x36)
		  {
		  	flags |= 0x04; /* Right Shift */
		  }
		  else if((scancode & 0x7F) == 0x1D) /* Control */
		  {
		  	flags |= 0x02;
		  }
		  else if((scancode & 0x7F) == 0x38) /* Alt */
		  {
		  	flags |= 0x01;
		  }
		  else if((scancode & 0x7F) == 0x3A) /* Caps Lock */
		  {
		    flags ^= 0x40;
		  }
		  else if((scancode & 0x7F) == 0x45) /* Num Lock */
		  {
		    flags ^= 0x20;
		  }
		  else if((scancode & 0x7F) == 0x46) /* Scroll Lock */
		  {
		    flags ^= 0x10;
		  }
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */
        
        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */
        
        // Since we don't want the key press to go to screen,
        // we don't let it print a charachter when paused
        if (pause_flag == 1)
        {
          unpause();
        }
        else
        {
          if((SHIFT(flags) && !CAPSLOCK(flags)) || (CAPSLOCK(flags) && !SHIFT(flags)))
          {
			      printf("%c", kbdus_shift[scancode]);
          }
          else
          {
		        printf("%c", kbdus[scancode]);
          }
        }
    }
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install(void)
{
    install_irq_handler(1, keyboard_handler);
}

