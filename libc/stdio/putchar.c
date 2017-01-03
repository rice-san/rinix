#include <stdio.h>

#if defined(__is_libk)
#include <rinix/tty.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	term_write(&c, sizeof(c));
#else
	// TODO: Put a write system call.
#endif
	return ic;
}
