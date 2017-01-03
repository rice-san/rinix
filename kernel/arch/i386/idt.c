#include <asm/io.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <rinix/debug.h>

#include "regs.h"
#include "irq.h"

/* Intel ISRs */

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* End Intel ISRs*/



/* IRQs */

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* End ISRs */

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

uint64_t idt[256];
struct idt_ptr ip;

extern void idt_load();

void create_idt_descriptor(const uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
	idt[num]  = (uint64_t)0;
	idt[num] |=  (((uint64_t)base) & 0xFFFF);					// bits 0..15
	idt[num] |=  ((((uint64_t)selector) & 0xFFFF) << 16);		// bits 16..31
	// bits 32..39
	idt[num] |= ((((uint64_t)flags) & 0xFF) << 40);				// bits 40..47
	idt[num] |= (((((uint64_t)base) >> 16) & 0xFFFF) << 48);	// bits 48..63
}

/* Install Intel ISRs */
static void intel_isr_install(void)
{
	create_idt_descriptor(0, (unsigned)isr0, 0x08, 0x8E);
	create_idt_descriptor(1, (unsigned)isr1, 0x08, 0x8E);
	create_idt_descriptor(2, (unsigned)isr2, 0x08, 0x8E);
	create_idt_descriptor(3, (unsigned)isr3, 0x08, 0x8E);
	create_idt_descriptor(4, (unsigned)isr4, 0x08, 0x8E);
	create_idt_descriptor(5, (unsigned)isr5, 0x08, 0x8E);
	create_idt_descriptor(6, (unsigned)isr6, 0x08, 0x8E);
	create_idt_descriptor(7, (unsigned)isr7, 0x08, 0x8E);
	create_idt_descriptor(8, (unsigned)isr8, 0x08, 0x8E);
	create_idt_descriptor(9, (unsigned)isr9, 0x08, 0x8E);
	create_idt_descriptor(10, (unsigned)isr10, 0x08, 0x8E);
	create_idt_descriptor(11, (unsigned)isr11, 0x08, 0x8E);
	create_idt_descriptor(12, (unsigned)isr12, 0x08, 0x8E);
	create_idt_descriptor(13, (unsigned)isr13, 0x08, 0x8E);
	create_idt_descriptor(14, (unsigned)isr14, 0x08, 0x8E);
	create_idt_descriptor(15, (unsigned)isr15, 0x08, 0x8E);
	create_idt_descriptor(16, (unsigned)isr16, 0x08, 0x8E);
	create_idt_descriptor(17, (unsigned)isr17, 0x08, 0x8E);
	create_idt_descriptor(18, (unsigned)isr18, 0x08, 0x8E);
	create_idt_descriptor(19, (unsigned)isr19, 0x08, 0x8E);
	create_idt_descriptor(20, (unsigned)isr20, 0x08, 0x8E);
	create_idt_descriptor(21, (unsigned)isr21, 0x08, 0x8E);
	create_idt_descriptor(22, (unsigned)isr22, 0x08, 0x8E);
	create_idt_descriptor(23, (unsigned)isr23, 0x08, 0x8E);
	create_idt_descriptor(24, (unsigned)isr24, 0x08, 0x8E);
	create_idt_descriptor(25, (unsigned)isr25, 0x08, 0x8E);
	create_idt_descriptor(26, (unsigned)isr26, 0x08, 0x8E);
	create_idt_descriptor(27, (unsigned)isr27, 0x08, 0x8E);
	create_idt_descriptor(28, (unsigned)isr28, 0x08, 0x8E);
	create_idt_descriptor(29, (unsigned)isr29, 0x08, 0x8E);
	create_idt_descriptor(30, (unsigned)isr30, 0x08, 0x8E);
	create_idt_descriptor(31, (unsigned)isr31, 0x08, 0x8E);
}


/* Install IRQs */
/* We need to remap IRQ Controllers, so they don't 
 * overlap with Intel ISRs like the
 * Double Fault Exception */
static void irq_remap(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

/* IRQ Installer */
static void irq_install(void) {
	irq_remap();
	
	create_idt_descriptor(32, (unsigned)irq0, 0x08, 0x8E);
	create_idt_descriptor(33, (unsigned)irq1, 0x08, 0x8E);
	create_idt_descriptor(34, (unsigned)irq2, 0x08, 0x8E);
	create_idt_descriptor(35, (unsigned)irq3, 0x08, 0x8E);
	create_idt_descriptor(36, (unsigned)irq4, 0x08, 0x8E);
	create_idt_descriptor(37, (unsigned)irq5, 0x08, 0x8E);
	create_idt_descriptor(38, (unsigned)irq6, 0x08, 0x8E);
	create_idt_descriptor(39, (unsigned)irq7, 0x08, 0x8E);
	create_idt_descriptor(40, (unsigned)irq8, 0x08, 0x8E);
	create_idt_descriptor(41, (unsigned)irq9, 0x08, 0x8E);
	create_idt_descriptor(42, (unsigned)irq10, 0x08, 0x8E);
	create_idt_descriptor(43, (unsigned)irq11, 0x08, 0x8E);
	create_idt_descriptor(44, (unsigned)irq12, 0x08, 0x8E);
	create_idt_descriptor(45, (unsigned)irq13, 0x08, 0x8E);
	create_idt_descriptor(46, (unsigned)irq14, 0x08, 0x8E);
	create_idt_descriptor(47, (unsigned)irq15, 0x08, 0x8E);
}

void idt_init(void)
{
		// Set up IDT pointer
		ip.limit = sizeof(idt);
		ip.base = &idt;
		
		// Clear out the IDT
		memset(&idt, 0, (sizeof(idt)));
		
		// Disable Interrupts
		__asm__ __volatile__ ("cli");
		
		// Load ISRs and IRQs here
		intel_isr_install();
		printd("ISRs Installed\n");
		irq_install();
		printd("IRQs Installed\n");
		
		// Load IDT
		idt_load();

		// Enable Interrupts
		__asm__ __volatile__ ("sti");
		printd("Interrupts Enabled\n");
}

/* ISR Related Things*/

const char *exception_messages[] =
{
	"Division by Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
};


void isr_handler(struct regs *r) {
	
	if (r->int_no < 32)
	{
		printf("%s",exception_messages[r->int_no]);
		printf(" Exception. System Halted!\n");
		printf("Error Code: %i\n", r->err_code);
		printf("eip: %x\n", r->eip);
		//TODO: Create seperate ISR handlers in another file
		if (r->int_no == 14 ){
			intptr_t cr2;
			asm ("movl %%esp, %0" : "=r" (cr2) );
			printf("cr2 value: %x\n", cr2);
		}
		for(;;);
	}
};

/* IRQ related things */

void *irq_routines[16] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void install_irq_handler(int irq, void (*handler)(struct regs *r)) {
	irq_routines[irq] = handler;
}

void uninstall_irq_handler(int irq) {
	irq_routines[irq] = 0;
}

/* The IRQ Handler must tell IRQ Controllers when they are finished being served, 
 * Send and End of Interrupt (0x20) */
void irq_handler(struct regs *r) {
	void (*handler)(struct regs *r); // Template pointer-to-func
	
	handler = irq_routines[r->int_no - 32];
	
	if(handler) {
		handler(r);
	}
	
	/* Send to slave controller if needed */
	if(r->int_no >= 40) {
		outb(0xA0, 0x20);
	}
	
	outb(0x20, 0x20);
}
