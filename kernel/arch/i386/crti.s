.section .init
.global _init
.type _init, @function
_init:
	push %ebp
	movl %esp, %ebp
	/* The rest of crtbegin.o's .init section will be placed here */

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %ebp
	movl %esp, %ebp
	/* gcc will place crtgeni.o's .fini section here*/
