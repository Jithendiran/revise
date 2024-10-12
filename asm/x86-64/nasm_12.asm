; sub -> sub 2 number and set some flags like parity, zero, carry, sign flags
; sub has 2 values left need to be memory location and right can be momory or direct values
; result will be store in left location and set flags
; when signed flag is set. it indicated that result is negative

section .text
global _start

_start:
    mov rax, 5
    mov rbx, 3
    sub rax, rbx            ; result 2 is stored in rax

    mov rax,3
    mov rbx, 5
    sub rax, rbx            ; result -2 is stored in rax. sign flag is set to 1

    mov rax, 1
    int 80h