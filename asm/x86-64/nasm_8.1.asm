; nasm_8.asm can write like this
; nasm -f elf64 -o a.o nasm_8.1.asm && ld a.o -o a.out && ./a.out

%include "nasm_8.1-header.asm"
%include "nasm_8.1-footer.asm"

section .data
    text db "Hello",10

section .text
    global _start

_start:
    call print
    jmp exit

