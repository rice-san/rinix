#ifndef _IRQ_H
#define _IRQ_H

void install_irq_handler(int irq, void (*handler)(struct regs *r));

#endif
