.global idt_load
.extern ip
idt_load:
		lidt ip
		ret
