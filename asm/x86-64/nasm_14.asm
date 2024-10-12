; div -> divide 2 number 
; div has 2 values 1 must be rax and another one may be constant, memory or registers 
; result will be store in rax
; remainder will store in rdx
; div for unsigned and idiv for signed values
section .text
global _start

_start:
    mov rax, 11
    mov rbx, 2
    div rbx
    ; info register rax -> 5 (quotient)
    ; info register rdx -> 1 (remainder)

    mov rdx, 0
    mov rax, 0xff                       ; 255
    mov rbx, 2
    div rbx                             ; 255/2 rax->127 rdx -> 1

    mov rdx, 0
    mov rax, 0xff                       
    mov rbx, 2
    idiv rbx

    mov rax, 1
    int 80h