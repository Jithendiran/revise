; AND, OR, NOT, XOR
; SHL(shift left), SHR(shift right), SAL(shift airthmetic left), SAR(shift airthmetic right)
; shift airthmetic will preserve sign bit
; the shifted bit will be placed in carry flag
section .text
global _start

_start:
    mov rax, 0b1010
    mov rbx, 0b1101
    and rax, rbx

    mov rax, 0b1010
    mov rbx, 0b1101
    or rax, rbx

    not rax

    mov rax, 0b1010
    mov rbx, 0b1100
    xor rax, rbx

    mov rax, 2
    shr rax,1                           ; shift one bit to right 0010 -> 0001  the sfifted 0 will be placed in carry flag
    shr rax,1                           ; shift one bit to right 0001 -> 0001  the sfifted 1 will be placed in carry flag

    mov rax, 2
    shr rax,2                           ; it means shift 2 times same as above
                                        ; shift right is same as divide by 2 operation

    mov rax, 2
    shl rax,1                           ; shift one bit to left 0010 -> 0100  the sfifted 0 will be placed in carry flag
    shl rax,1                           ; shift one bit to left 0100 -> 1000  the sfifted 0 will be placed in carry flag

    mov rax, 2
    shl rax,2                           ; it means shift 2 times same as above
                                        ; shift left is same as multiply by 2 operation

    mov rax, 2
    sar rax,2



    mov rax, 60         
    mov rdi, 0        
    syscall 