; floating point number

section .data
    x DD 3.14
    y DD 2.1

section .text
    global _start

; to store floating point cpu have a special registers 
; xmm0 - xmm15

_start:

    MOVSS  xmm0, [x]                     ; Move scalar single precision
    ; single value moved at a time (one decimal value into a register)
                                         ; single percision 32 bit floating point number 
    movss  xmm1, [y]

    ; any operation can perform like add, sub,..
    ADDSS xmm0, xmm1

    MOVSS xmm0, [x]
    ; compare
    UCOMISS xmm0, XMM1
    ; will sets eflags
    ; JUMP syntax also differ JB(jump below) is like JL, JA(jump above) is like JG, JBE(jump below equal),..
    ; to compare equal JE
    JA greater
    jmp end

greater:
    mov ecx, 1

end:
; exit interup
    mov eax, 1
    mov ebx, 27
    int 80h

; to print values in GDB
; p $xmm0.v4_float[0]