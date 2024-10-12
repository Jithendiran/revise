; math
; operation register, value/register

; add rax, 5 ;what this will do is increment rax value by 5 and store in rax

; sub rax, rbx ;what this will do is decrement rax value by the value of rbx and store in rax

; for mul and div 1st argument alwasy in rax and do operatio like this

; mov rax, 5
; mul rbx

; result of mul will store in rax, like wise div

; mul and div for Unsingned
; imul and idiv for signed

section .data
    digit db 0,10 ; digit is 2byte long

section .text
    global _start

_start:
    mov rax, 0
    call printRAXDigit

    mov rax, 1
    call printRAXDigit

    mov rax, 25         ;it prints I confused
    call printRAXDigit

    mov rax, 2
    add rax, 2
    call printRAXDigit

    mov rax, 6
    mov rbx, 2
    div rbx
    call printRAXDigit

    jmp exit

exit:
    mov rax, 60         
    mov rdi, 0          
    syscall  

; extended ascii is 8 bit long

printRAXDigit:
    add rax, 48       ;ascii value of 48 is 0 
    mov [digit], al   ; al is the lower 8 bit of rax register, it will move lower 8 bit of rax register directly to ram memory; it will alter only lower 8bit of digit and 10(new line) is unaffected
    mov rax, 1          
    mov rdi, 1
    mov rsi, digit      
    mov rdx, 2         
    syscall 
    ret   