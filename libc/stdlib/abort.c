#include <stdlib.h>
#include <stdio.h>

__attribute__((__noreturn__))
void abort(void)
{
		// Add Kernel Panic
		printf("Kernel Panic: abort()\n");
		while (1) { }
		__builtin_unreachable();
}
