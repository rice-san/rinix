.global gdt_load
.extern gp
gdt_load:
		cli
        lgdt gp
        # Reload data segment registers:
        mov $0x10, %ax # 0x10 points at the new data selector
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss
        ljmp $0x8,$reload_CS     # Reload CS register 
reload_CS:
		ret
