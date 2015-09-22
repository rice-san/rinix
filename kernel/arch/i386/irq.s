#IRQs occupy spaces 32-47 because Intel ISRs are in spaces 0-31
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

irq0:
	cli
	push $0
	push $32
	jmp irq_common_stub

irq1:
	cli
	push $0
	push $33
	jmp irq_common_stub

irq2:
	cli
	push $0
	push $34
	jmp irq_common_stub

irq3:
	cli
	push $0
	push $35
	jmp irq_common_stub

irq4:
	cli
	push $0
	push $36
	jmp irq_common_stub

irq5:
	cli
	push $0
	push $37
	jmp irq_common_stub

irq6:
	cli
	push $0
	push $38
	jmp irq_common_stub

irq7:
	cli
	push $0
	push $39
	jmp irq_common_stub

irq8:
	cli
	push $0
	push $40
	jmp irq_common_stub

irq9:
	cli
	push $0
	push $41
	jmp irq_common_stub

irq10:
	cli
	push $0
	push $42
	jmp irq_common_stub

irq11:
	cli
	push $0
	push $43
	jmp irq_common_stub

irq12:
	cli
	push $0
	push $44
	jmp irq_common_stub

irq13:
	cli
	push $0
	push $45
	jmp irq_common_stub

irq14:
	cli
	push $0
	push $46
	jmp irq_common_stub

irq15:
	cli
	push $0
	push $47
	jmp irq_common_stub


#irq_handler() is found in idt.c
.extern irq_handler


#Common IRQ stub
irq_common_stub: 
        pusha
        push %ds
        push %es
        push %fs
        push %gs
        movw $0x10, %ax         # Load the Kernel Data Segment descriptor!
        movw %ax, %ds
        movw %ax, %es
        movw %ax, %fs
        movw %ax, %gs
        movl %esp, %eax         # Push us the stack
        pushl %eax
        movl $irq_handler, %eax
        call *%eax                # A Special call, preserves the 'eip' register
        popl %eax
        popl %gs
        popl %fs
        popl %es
        popl %ds
        popa
        addl $8, %esp           # Cleans up the pushed error code and ISR number
        iret                    # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
