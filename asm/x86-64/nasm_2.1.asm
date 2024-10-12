; print
; nasm -f elf64 -o a.o nasm_2.asm && ld a.o -o a

section .data
    text db "Hello world",10
    text2 db "end",10

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
    mov rdx, 12         
    syscall ; this label is not ended with ret meaning call will execute unitl ret, but it must needed ret
printEnd:   ; when ever print is call printEnd content is also execute because it is continuation
    mov rax, 1          
    mov rdi, 1         
    mov rsi, text2      
    mov rdx, 14         
    syscall   
    ret       
