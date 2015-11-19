// arch_init.c - Perform machine dependent initialization functions
// This file is for x86 architectures


extern void gdt_init(void);	// Found in gdt.c
extern void idt_init(void); // Found in idt.c
extern void timer_install(void); // Found in timer.c
extern void keyboard_install(void); // Found in keyboard.c

void arch_init(void) {
	gdt_init();
	idt_init();
	timer_install();
	keyboard_install();
}

void arch_finish_init(void){
	
}