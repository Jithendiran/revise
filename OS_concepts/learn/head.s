# Head.s contains code for 32-bit protected mode init, clock & system call interrupts, and two
# tasks code. After initialization, the program moves to task 0 to start execution, and the  switching operation between tasks 0 and 1 is performed under the clock interrupt.
LATCH = 11930                                                   # Timer count, interrupt is sent every 10 ms.
SCRN_SEL = 0x18                                                 # The segment selector for the screen display memory.
TSS0_SEL = 0x20                                                 # TSS segment selector for task 0.
LDT0_SEL = 0x28                                                 # LDT segment selector for task 0.
TSS1_SEL = 0X30                                                 # TSS segment selector for task 1.
LDT1_SEL = 0x38                                                 # LDT segment selector for task 1.
.text
startup_32:
# First load DS, SS, and ESP. The linear base address of all segments is 0.
    movl $0x10,%eax                                             # 0x10 is the data segment selector in the GDT.
    mov %ax,%ds
    lss init_stack,%esp
    
# Re-set the IDT and GDT tables at new location.
    call setup_idt                                              # Setup IDT.
    call setup_gdt                                              # Setup GDT.
    movl $0x10,%eax                                             # Reload all segment registers after changing GDT.
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    lss init_stack,%esp

# Set 8253 timing chip. Channel 0 is set to generate an interrupt request every 10 ms.
    movb $0x36, %al                                             # Control word: Channel 0 in mode 3, Count in binary.
    movl $0x43, %edx                                            # 0x43 is write port of control word register.
    outb %al, %dx
    movl $LATCH, %eax                                           # Init count set to LATCH (1193180/100), freq. 100HZ.
    movl $0x40, %edx                                            # The port of channel 0.
    outb %al, %dx                                               # Write initial count value to channel 0 in two steps.
    movb %ah, %al
    outb %al, %dx

# Set the timer interrupt gate descriptor at item 8 of the IDT table.
    movl $0x00080000, %eax                                      # EAX high word set to kernel code seg selector 0x0008.
    movw $timer_interrupt, %ax                                  # Set timer int gate descriptor. Get handler address.
    movw $0x8E00, %dx                                           # Interrupt gate type is 14, plevel is 0 or hardware used.
    movl $0x08, %ecx                                            # Clock interrupt vector no. set by BIOS is 8.
    lea idt(,%ecx,8), %esi                                      # Put IDT Descriptor 0x08 address into ESI and set it.
    movl %eax,(%esi)
    movl %edx,4(%esi)                                           # Set the system call trap gate descriptor at item 128 (0x80) of the IDT table.
    movw $system_interrupt, %ax                                 # Set system call gate descriptor. Get handler address.
    movw $0xef00, %dx                                           # Trap gate type is 15, code of plevel 3 is executable.
    movl $0x80, %ecx                                            # System call vector no. is 0x80.
    lea idt(,%ecx,8), %esi                                      # Put IDT Descriptor 0x80 address into ESI and set it.
    movl %eax,(%esi)
    movl %edx,4(%esi)
    # Now, to use IRET to move to task 0 (A), we manually prepare to setup the stack contents. See Figure4-29 for the stack contents we need to setup. Refer to include/asm/system.h.
    pushfl                                                      # Reset NT flag in EFLAGS to disable task switch when
    andl $0xffffbfff, (%esp)                                    # execute IRET instruction.
    popfl
    movl $TSS0_SEL, %eax                                        # Load task0's TSS seg selector into task register TR.
    ltr %ax
    movl $LDT0_SEL, %eax                                        # Load task0's LDT seg selector into LDTR.
    lldt %ax                                                    # TR and LDTR need only be manually loaded once.
    movl $0, current                                            # Save current task num 0 into current variable.
    sti                                                         # Enable int, build a scene on stack for int returns.
    pushl $0x17                                                 # Push task 0 data (stack) seg selector onto stack.
    pushl $init_stack                                           # Push the stack pointer (same as push ESP).
    pushfl                                                      # Push the EFLAGS.
    pushl $0x0f                                                 # Push current local space code seg selector.
    pushl $task0                                                # Push task 0 code pointer onto stack.
    iret                                                        # This causes execution moves to task0 in plevel 3.
    # The following are the subroutines for setting descriptor items in GDT and IDT.
setup_gdt:                                                      # GDT table position & limit are set using
    lgdt lgdt_opcode                                            # 6-byte operand lgdt_opcode.
    ret
    # The following code is used to temporarily set all 256 interrupt gate descriptors in the
    # IDT table to the same default value. All use the default interrupt handler ignore_int.# The specific method of setting is: first set the contents of 0-3 bytes and 4-7 bytes of            
    # the default interrupt gate descriptor into the eax and edx register pairs. Then, using
    # this register pair, the interrupt descriptor is cyclically filled into the IDT table.
setup_idt:                                                      # Set all 256 int gate descriptors to use default handler.
    lea ignore_int,%edx                                         # The same way as setting timer int gate descriptor.
    movl $0x00080000,%eax                                       # The selector is 0x0008.
    movw %dx,%ax
    movw $0x8E00,%dx                                            # Interrupt gate type is 14, plevel is 0.
    lea idt,%edi
    mov $256,%ecx                                               # Loop through all 256 gate descriptor entries.
rp_idt: 
    movl %eax,(%edi)
    movl %edx,4(%edi)
    addl $8,%edi
    dec %ecx
    jne rp_idt
    lidt lidt_opcode                                            # IDTR register is loaded with a 6-byte operand.
    ret
# Display characters subroutine. Get current cursor position & display char in AL.
# The entire screen can display 80 X 25 (2000) characters.
 write_char:
    push %gs                                                    # First save the register to be used, EAX is
    pushl %ebx                                                  # saved by the caller.
    mov $SCRN_SEL, %ebx                                         # Then let GS point to display mem seg (0xb8000).
    mov %bx, %gs
    movl scr_loc, %bx                                           # Get current char display position from scr_loc.
    shl $1, %ebx                                                # Since each char has one attribute byte, so actual
    movb %al, %gs:(%ebx)                                        # display memory offset should multiplied by 2.
    shr $1, %ebx                                                # After putting char into display memory, divide the
    incl %ebx                                                   # position value by 2 plus 1 to get the next position.
    cmpl $2000, %ebx                                            # If position is greater than 2000, it is reset to 0.
    jb 1f
    movl $0, %ebx
1: movl %ebx, scr_loc                                           # Finally save this position value (scr_loc),
    popl %ebx                                                   # and pop up the contents of saved register, return.
    pop %gs
    ret
# The following are 3 interrupt handlers: default, timer, and system call interrupt.
# Ignore_int is default handler. If system generates other interrupts, it display char 'C'.
.align 2
ignore_int:
    push %ds
    pushl %eax                                                  # Let DS point to the kernel data segment because
    movl $0x10, %eax                                            # the interrupt handler belongs to the kernel.
    mov %ax, %ds
    movl $67, %eax                                              # Put 'C' in AL, call write_char to display on screen.
    call write_char
    popl %eax
    pop %ds
    iret

# This is the timer interrupt handler. The main function is to perform task switching operations.
.align 2
timer_interrupt:
    push %ds
    pushl %eax
    movl $0x10, %eax                                            # First let DS point to the kernel data segment.
    mov %ax, %ds
    movb $0x20, %al                                             # Then send EOI to 8259A to allow other interrupts.
    outb %al, $0x20
    movl $1, %eax                                               # Then check current task to switch task 0 and 1.
    cmpl %eax, current
    je 1f
    movl %eax, current                                          # If current task is 0, save 1 in current and jump to
    ljmp $TSS1_SEL, $0                                          # task 1 to execute. The offset of jump is useless.
    jmp 2f
    1: movl $0, current                                         # If current task is 1, save 0 in current and jump to
    ljmp $TSS0_SEL, $0                                          # task 0 to execute.
    2: popl %eax
    pop %ds
    iret
    
# The system call int 0x80 handler. This example has only one display char function.
.align 2
system_interrupt:
    push %ds
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl $0x10, %edx                                            # First let DS point to the kernel data segment.
    mov %dx, %ds
    call write_char                                             # Then call routine write_char to display char in AL.
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    pop %ds
    iret

/*********************************************/
current:.long 0                                                 # Store current task number (0 or 1).
scr_loc:.long 0                                                 # Store screen current display position.

.align 2
lidt_opcode:
    .word 256*8-1                                               # 6-byte operand for set IDTR : table size & base.
    .long idt
lgdt_opcode:
    .word (end_gdt-gdt)-1                                       # 6-byte operand for set IDTR : table size & base.
    .long gdt

.align 3
idt: .fill 256,8,0                                              # IDT. 256 gate descriptors, each 8 bytes, total 2KB.
# The following is GDT table contents (of descriptors).
gdt: .quad 0x0000000000000000                                   # [0] The first segment descriptor is not used.
    .quad 0x00c09a00000007ff                                    # [1] Kernel code descriptor. Its selector is 0x08
    .quad 0x00c09200000007ff                                    # [2] Kernel data descriptor. Its selector is 0x10
    .quad 0x00c0920b80000002                                    # [3] Display mem descriptor. Its selector is 0x18
    .word 0x68, tss0, 0xe900, 0x0                               # [4] TSS0 descriptor. Its selector is 0x20.
    .word 0x40, ldt0, 0xe200, 0x0                               # [5] LDT0 descriptor. Its selector is 0x28
    .word 0x68, tss1, 0xe900, 0x0                               # [6] TSS1 descriptor. Its selector is 0x30
    .word 0x40, ldt1, 0xe200, 0x0                               # [7] LDT1 descriptor. Its selector is 0x38
end_gdt:
    .fill 128,4,0                                               # Initial kernel stack space.
init_stack:                                                     # Stack pointer when first enter protected mode.
    .long init_stack                                            # Stack segment offset position.
    .word 0x10                                                  # Stack segment, same as kernel data seg (0x10).

# Below is the local segment descriptor in the LDT table segment of task 0.
.align 3
ldt0: .quad 0x0000000000000000                                  # [0] The first descriptor is not used.
    .quad 0x00c0fa00000003ff                                    # [1] The local code descriptor, its selector is 0x0f
    .quad 0x00c0f200000003ff                                    # [2] The local data descriptor, its selector is 0x17
# Content of TSS seg for task 0. Note fields such as labels do not change when task switches.
tss0: .long 0 /* back link */
    .long krn_stk0, 0x10 /* esp0, ss0 */
    .long 0, 0, 0, 0, 0 /* esp1, ss1, esp2, ss2, cr3 */
    .long 0, 0, 0, 0, 0 /* eip, eflags, eax, ecx, edx */
    .long 0, 0, 0, 0, 0 /* ebx esp, ebp, esi, edi */
    .long 0, 0, 0, 0, 0, 0 /* es, cs, ss, ds, fs, gs */
    .long LDT0_SEL, 0x8000000 /* ldt, trace bitmap */
 
    .fill 128,4,0                                               # This is the kernel stack space for task 0.
krn_stk0:
 
# Task 1 LDT table content and TSS segment content.
.align 3
ldt1: .quad 0x0000000000000000                                  # [0] The first descriptor is not used.
    .quad 0x00c0fa00000003ff                                    # [1] The selector is 0x0f, base = 0x00000。
    .quad 0x00c0f200000003ff                                    # [2] The selector is 0x17, base = 0x00000。

tss1: .long 0 /* back link */
    .long krn_stk1, 0x10 /* esp0, ss0 */
    .long 0, 0, 0, 0, 0 /* esp1, ss1, esp2, ss2, cr3 */
    .long task1, 0x200 /* eip, eflags */
    .long 0, 0, 0, 0 /* eax, ecx, edx, ebx */
    .long usr_stk1, 0, 0, 0 /* esp, ebp, esi, edi */
    .long 0x17,0x0f,0x17,0x17,0x17,0x17 /* es, cs, ss, ds, fs, gs */
    .long LDT1_SEL, 0x8000000 /* ldt, trace bitmap */

    .fill 128,4,0 # This is the kernel stack space for task 1. Its user
krn_stk1:                                                       # stack uses the initial kernel stack space directly.
 
# The programs of tasks 0 and 1, which cyclically display chars 'A' and 'B', respectively.
task0:
    movl $0x17, %eax                                            # DS point to the local data segment of the task.
    movw %ax, %ds                                               # No local data, these 2 instructions can be omitted.
    movl $65, %al                                               # Put 'A' into the AL register.
    int $0x80                                                   # Execute system call to display it.
    movl $0xfff, %ecx                                           # Execute a loop, act as a delay.
    1: loop 1b
    jmp task0                                                   # Jump to start of task 0 to continue displaying.
task1:
    movl $66, %al                                               # Put 'B' into the AL register.
    int $0x80                                                   # Execute system call to display it.
    movl $0xfff, %ecx                                           # Execute a loop, act as a delay.
1: loop 1b
   jmp task1

   .fill 128,4,0                                                # This is user stack space for task 1.
usr_stk1: