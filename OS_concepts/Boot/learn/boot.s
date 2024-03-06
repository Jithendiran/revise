;! boot.s program
;! First use BIOS interrupt to load head code into memory 0x10000, and then move it to memory 0.
;! Finally enter protected mode, jump to the beginning of head code at 0 to continue running.

BOOTSEG = 0x07c0                ;! This program is loaded into memory at 0x7c00 by BIOS.
SYSSEG = 0x1000                 ;! The head is first loaded to 0x10000 and moved to 0x0.
SYSLEN = 17                     ;! Max num of disk sectors occupied by the kernel.
entry start
start:
    jmpi go,#BOOTSEG            ;! Jump between segments to 0x7c0:go. All segment reg
go: mov ax,cs                   ;! are 0 when runs. This jump ins load CS with 0x7c0.
    mov ds,ax                   ;! Both DS and SS point to the 0x7c0 segment.
    mov ss,ax
    mov sp,#0x400               ;! Set temp stack pointer. Its value needs to be larger
                                ;! than this program and has a certain space.
;! Load the kernel code to the beginning of memory at address 0x10000.
load_system:
    mov dx,#0x0000              ;! Use BIOS int 0x13 func2 to load head code from bootdisk.
    mov cx,#0x0002              ;! DH-head no; DL-drive no; CH-10 bit track no low 8 bits
    mov ax,#SYSSEG              ;! CL-Bits7,6 track num high 2 bits ,bit 5-0 start secter
    mov es,ax                   ;! ES:BX - Read in buffer location (0x1000:0x0000).
    xor bx,bx                   ;! AH-read sector func num; AL- num of sectors read (17).
    mov ax,#0x200+SYSLEN
    int 0x13
    jnc ok_load                 ;! If no error occurs, then continues, otherwise dead.
die: jmp die
;! Move kernel code to memory location 0. Total of 8KB bytes are moved (kernel code <8kb).
ok_load:
    cli ;! Disable interrupts.
    mov ax, #SYSSEG             ;! Move from DS:SI(0x1000:0) to ES:DI(0:0).
    mov ds, ax
    xor ax, ax
    mov es, ax
    mov cx, #0x1000             ;! Set the move 4K times, one word at a time.
    sub si,si
    sub di,di
    rep 
    movw                    ;! Execute the repeat move instruction.
;! Load IDT and GDT base address registers IDTR and GDTR.
    mov ax, #BOOTSEG
    mov ds, ax                  ;! Let DS point to 0x7c0 segment again.
    lidt idt_48                 ;! Load IDTR. 2 byte table limit, 4 byte linear base addr.
    lgdt gdt_48                 ;! Load GDTR. 2 byte table limit, 4 byte linear base addr.

;! Set CR0 to enter protection mode. The seg selector value 8 refers to 2nd descriptor in GDT.
    mov ax,#0x0001              ;! Set the protection mode flag PE (bit 0) in CR0.
    lmsw ax                     ;! Jump to segment specified by the selector, offset 0.
    jmpi 0,8                    ;! Seg value is now a selector. The linear base addr is 0.

;! The following is the content of GDT. It has 3 seg descriptors. The first one is not used,
;! the other two are code and data segment descriptors
gdt: .word 0,0,0,0              ;! First descriptor not used. Occupies 8 bytes.

    .word 0x07FF                ;! Descriptor 1. 8Mb - limit=2047 (2048*4096=8MB).
    .word 0x0000                ;! Segment base address = 0x00000.
    .word 0x9A00                ;! Code segment, readable/executable.
    .word 0x00C0                ;! Segment attribute granularity = 4KB, 80386.

    .word 0x07FF                ;! Descriptor 1. 8Mb - limit=2047 (2048*4096=8MB).
    .word 0x0000                ;! Segment base address = 0x00000.
    .word 0x9200                ;! Data segment, readable and writable.
    .word 0x00C0                ;! Segment attribute granularity = 4KB, 80386.
;! The following are the 6-byte operands of the LIDT and LGDT instructions, respectively.
idt_48: .word 0                 ;! The IDT table length is 0.
    .word 0,0                   ;! The linear base address of IDT table is also zero.
gdt_48: .word 0x7ff             ;! GDT limit is 2048 bytes, can hold 256 descriptors.
    .word 0x7c00+gdt,0          ;! Linear base of GDT is at offset gdt of seg 0x7c0.
.org 510
    .word 0xAA55                ;! Boot sector flag. Must be at last 2 bytes of boot sector.