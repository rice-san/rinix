#ifndef _RINIX_DEBUG_H
#define _RINIX_DEBUG_H

#include <stdio.h>

// Uncomment this for debug
#define DEBUG 1

#define nullAction( ) \
do { ; } \
while(0)

#ifdef DEBUG

	#define printd(str, arg...) ( printf( (str) , ##arg ) )
	#define _debug(action) \
	do { action; } \
	while(0)

#else

	#define printd(str, arg...) nullAction( )
	#define _debug(action) nullAction( )

#endif

//void flash_screen(void);

#endif
