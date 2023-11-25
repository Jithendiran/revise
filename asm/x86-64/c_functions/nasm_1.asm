; to call standard c
; nasm -f elf64 -o a.o nasm_1.asm
; gcc -z noexecstack -m64 -no-pie a.o -o a.out
;./a.out

; to call custom c
; nasm -f elf64 -o a.o nasm_1.asm
; gcc -z noexecstack -m64 -no-pie a.o function.c -o a.out

; calling c standard functions
; for this compiling from nasm and running with gcc
; gcc starting point is main so start is changed to main


extern printf  ;extern keyword from c; printf function is defined outside
extern custom_function
section .text
    global main

main:
; c function arguments are expected to be in stack
; 1st arugument of printf is fmt then msg
; stack is LIFO, so 1st push msg and format
   
    ;push msg
    ;PUSH fmt
    ;call printf         ; return value auto stored in rax

    push  2
    push  3
    call custom_function
    add esp, 8

    mov rbx, rax
    mov rax, 1     
    int 80h


section .data
    msg DB "Hello c",0              ; 0 -> null terminate for string, 10 -> new line
    fmt DB "OP : %s",10,0