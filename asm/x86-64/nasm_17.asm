; loops

section .data
    list db 1,2,3,4

section .text
    global _start

_start:
    mov rax, 0
    mov cl, 0
loop:
    mov bl, [list + rax]  ; list address + add byte to fetch value 
    add cl, bl
    inc rax
    cmp rax, 4
    jne loop  ; if rax is not equal to 4 loop again

    mov rax, 1
    int 80h