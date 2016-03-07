#ifndef _RINIX_TIMER_H
#define _RINIX_TIMER_H

void timer_install(void);
void timer_wait(int ticks);
unsigned int get_ticks(void);

#endif
