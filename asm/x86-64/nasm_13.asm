; mul -> multiply 2 number 
; mul has 2 values 1 must be rax and another one may be constant, memory or registers 
; result will be store in rax
; mul for unsigned and imul for signed values
section .text
global _start

_start:
    mov al, 5
    mov bl, 3
    mul bl           ; result 15 is stored in al

    mov al, 0xff     ; 255 ; info registers al -> -1 
    ; mul is consider as unsigned one so it will ne 255
    mov bl, 2           
    mul bl           ; 255 * 2 = 
    ; al is not sufficient to store 510
    ; info registers al -> -2 
    ; info registers ah -> 1
    ; info registers ax -> 510
    ; multiply will automatically use higher order bits to store result

    mov rax, 0      ;reset
    mov rbx, 0

    mov al, 0xff    ; -1
    mov bl, 2           
    imul bl         ; signed multiply -1 * 2 = -2
    ; al is consider as -ve value
    ; info registers al -> -2 
    ; info registers ah -> -1
    ; info registers ax -> -2

    mov rax, 1
    int 80h