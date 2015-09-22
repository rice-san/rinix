#include <stdio.h>

#include <rinix/timer.h>

#include "irq.h"

/* Keep track of how many ticks this has been running for */
volatile unsigned int timer_ticks = 0;
static float timer_hz = 18.222;

/* Function to change timer speed */
void timer_phase(int hz)
{
  int divisor = 1193180/hz;      /* Calculate the divisor */
  outb(0x43, 0x36);           /* Send the command byte 0x36 */
  outb(0x40, divisor & 0xFF); /* Set low byte of divisor */
  outb(0x40, divisor >> 8);   /* Set high byte of divisor*/
  timer_hz = hz;
}

/* Handle the timer. Increment the time variable each time the timer fires */
void timer_handler(struct regs *r)
{
  /* Increment the tick count */
  timer_ticks++;

  /* By defualt every 18 clocks (approximately 1 second), we will
   * display a message on the screen... 
   * FOR NOW! */
}

void timer_wait(int ticks)
{
	unsigned long end;
	
	end = timer_ticks + ticks;
	while(timer_ticks < end);
}

/* Set up the system clock by installing the timer handler into IRQ0 */
void timer_install(void)
{
  /* Installs 'timer_handler' to IRQ0 */
  install_irq_handler(0, timer_handler);
}
