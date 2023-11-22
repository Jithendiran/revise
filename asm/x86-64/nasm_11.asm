; ADD -> add 2 number and set some flags like parity, zero, carry flags
; to see flag values in gdb use "info registers eflags"
; ADD has 2 values left need to be memory location and right can be momory or direct values
; result will be store in left location

section .text
global _start

_start:
    mov al,0b11111111          ; 0b means binary
    ; value of al is 255
    mov bl,0b0001
    ; value of bl is 1
    ; when 255 + 1 is add result will be 256 but we are doing 8bit operation
    ; so the 9th bit will be hoped
    ; that 9th bit is carry, when every we have carry in operation carry flag will be set
    ; ADC is an another operator, it will add carry value
    add al,bl
    adc ah,0
    ; after this al is 0, ah is 1, ax is 256
    ;
    ; adc will add content of ah with 0 and with cary flag 
    ; why ax is result 256 is ah is higher 8bit of eax. when see ah alone it gives 1
    ; when see all 16 bit it's value is 256

    mov rax, 1
    int 80h

; gdb commands
; info registers al
; info registers ah
; info registers ax
; info registers eax