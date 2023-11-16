; nasm -f elf64 -o a.o nasm_3.asm && ld a.o -o a.out

section .data
    text1 db "Enter name",10
    text2 db "Hello "

section .bss
; this section is used to reserve data
    name resb 16 ; reserve 16  bytes of data for name memory


section .text
    global _start

_start:
    call print1
    call input
    call print2
    call print3
    jmp exit

exit:
    mov rax, 60         
    mov rdi, 0          
    syscall            

print:
    mov rax, 1          
    mov rdi, 1
    ret

print1:
    call print         
    mov rsi, text1      
    mov rdx, 11         
    syscall     
    ret      

print2:
    call print          
    mov rsi, text2      
    mov rdx, 6         
    syscall     
    ret  

print3:
    call print          
    mov rsi, name      
    mov rdx, 20       ; actual size is 16, just to see what it will print given 20 bytes. i expect garbage but printed empty  
    syscall     
    ret

input:
    mov rax, 0 ; 0 is sys_input
    mov rdi, 0  ; 0 is standard input
    mov rsi, name
    mov rdx, 16
    syscall
    ret

; when the size exceed 16 byte it will ignore after character 16 byte
