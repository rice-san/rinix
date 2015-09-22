.global loader

# Declare the constants for the multiboot header
.set ALIGN,			1<<0						# align loaded modules on page boundaries
.set MEMINFO,		1<<1						# provide memory map
.set FLAGS,			ALIGN | MEMINFO				# this is the Multiboot 'flag' field
.set MAGIC, 		0x1BADB002					# 'magic number' lets bootloader find the header
.set CHECKSUM,		-(MAGIC + FLAGS)			# checksum of the above, to prove we are multiboot

.set KERNEL_VIRTUAL_BASE, 0xC0000000                   # 3GB
.set KERNEL_PAGE_NUMBER, (KERNEL_VIRTUAL_BASE >> 22)   # Page directory index of kernel's 4MB PTE.

# Declare a header as in the Multiboot Standard
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


# Reserve a stack for initial thread.
.section .boot_stack
stack_bottom:
	.space 16384 # 16 KiB
stack_top:

.section .data
.align 0x1000
# Initial Page Directory
_initialPD:
    #Blank Page Directory
    .space 0x1000, 0x00

# Enable Paging Time!
.section .text
.align 4

# The original entry point
loader:
    # Initialize Stack
    movl $stack_top, %esp
    subl $KERNEL_VIRTUAL_BASE, %esp

    #Init paging
    call init_paging

    #Move stack to virtual address
    addl $KERNEL_VIRTUAL_BASE, %esp
    
    call _start


init_paging:

    #Save the registers
    push %eax
    push %ebx
    push %ecx
    push %edx

	movl $_initialPD, %eax
    subl $KERNEL_VIRTUAL_BASE, %eax


    #Insert Identity Mapped Low memory 4MB Page
    movl $0x083, (%eax)
    #Insert High Memory Page
    movl $KERNEL_PAGE_NUMBER, %edx
    movl $0x083, (%eax, %edx, 4)
    
    
    #Load PD
    movl %eax, %cr3

    #Enable PSE
    movl %cr4, %eax
    or $0x010, %eax
    movl %eax, %cr4

    #Enable Paging
    movl %cr0, %eax
    or $(1 << 31), %eax
    movl %eax, %cr0

    #Restore register values
    pop %edx
    pop %ecx
    pop %ebx
    pop %eax
    ret

# The kernel higher-half entry point
.type _start, @function
_start:
	# Pass the address of the initial page table on the stack
	push $_initialPD
	# Pass the multiboot information onto the stack.
	push %ebp
	
	# Initialize the core kernel before running global constructors
	call initk
	
	# Call the global constructors.
	call _init
	
	# Transfer control to the main kernel.
	call kmain
	
	# Hang if kmain returns unexpectedly (it shouldn't do that!)
	cli
	hlt
.Lhang:
		jmp .Lhang
.size _start, . - _start
