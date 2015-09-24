#include <rinix/pause.h>

void pause_init(void)
{
	pause_flag = 0;
}

void pause (void)
{
	pause_flag = 1;
	while(pause_flag == 1)
	{
		;
	}
}
