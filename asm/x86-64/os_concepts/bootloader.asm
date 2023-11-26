; compile
; nasm ./bootloader.asm -f bin -o bootloader.bin
; cp bootloader.bin bootloader.img
; truncate -s 1440k bootloader.img

; BIOS is looking for a signatute 0xAA55 to bootup in each disk
; BIOS is going to load each disk's memory address 0x7C00 and look for signature.
; if signature found it start bootup process, else scan next disk
; Once the bootup program found it loads to memory and start executing from data segment

; Commands
; ORG address -> means tell the assembler do addressing relative to the given address
; BITS 16 -> set's in 16 bit asm mode (16 bit is enough for boot loaders)
; HLT -> pause the CPU execution until CPU receives some type of Interupts 
; TIMES num -> going to repeat the operation for specific number of times

ORG 0x7C00
BITS 16

main:
    HLT

; the loop below is because need to pgm to run
halt:
    jmp halt

; write the signature at the end of the pgm
; assuming every block of data in disk is 512 byte
; write the signature in last 2 bytes of block

TIMES 510-($-$$) DB 0
DW 0AA55h

; $$ -> this will tell how many bytes the current proram is taking 
; Times code will navigate the code to 510th memory location


; to execute
; $qemu-system-1386 -fda bootloader.img