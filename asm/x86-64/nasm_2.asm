; nasm -f elf64 -o a.o nasm_2.asm && ld a.o -o a

section .data
    text db "Hello world",10

section .text
    global _start

_start:
    call print
    jmp exit

exit:
    mov rax, 60         
    mov rdi, 0          
    syscall            

print:
    mov rax, 1          
    mov rdi, 1         
    mov rsi, text      
    mov rdx, 14         
    syscall     
    ret       