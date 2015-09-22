.section .init
	/* gcc will put the .init section of crtend.o here */
	popl %ebp
	ret

.section .fini
	/* gcc will put the .fini section of crtend.o  here */
	popl %ebp
	ret
