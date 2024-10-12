section .data
    num db 1
;    num2 resb 1 ; warning: uninitialized space declared in non-BSS section `.data': zeroing [-w+zeroing]

section .bss
;    num2 db 2 ; nasm_19.asm:6: warning: attempt to initialize memory in BSS section `.bss': ignored [-w+other]
    num1 resb 10

section .rodata
    num2 db 2

section .text
global _start

_start:
    mov al, 1 
    mov [num], al 
;   mov [num2], al ; error
    mov [num1], al
    jmp exit

exit:
    mov rax, 60         
    mov rdi, 0          
    syscall