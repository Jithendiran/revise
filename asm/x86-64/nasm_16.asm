; compare will compare the operand the set flags
; based on the flag values jumps will be performed

section .text
global _start

_start:
    mov rax, 3
    mov rbx, 2
    cmp rax, rbx
    ; cmp will subract the operands and set eflag registers
    ; op may be -ve, 0 , +ve
    ; +ve rax is bigger, 0 both are equal, -ve rbx is bigger
    jl lesser                   ; if rax is lesser then jump to lesser
    jmp alwaysrun               ; jmp is independent of compare flags. it will jump always
    ; more jump varient are available je, jne, jg, jge, jle, jz, jnz

lesser:
    mov rcx, 1

alwaysrun:
    mov rax, 1
    int 80h

; see in gdb debugger and swap 3,2 values and see