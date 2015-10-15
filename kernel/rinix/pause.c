#include <rinix/pause.h>

volatile int pause_flag;

void pause_init(void)
{
	pause_flag = 0;
}

void pause (void)
{
	pause_flag = 1;
	while(pause_flag == 1);
}

void unpause (void)
{
	if (pause_flag == 1)
	{
		pause_flag = 0;
	}
}
