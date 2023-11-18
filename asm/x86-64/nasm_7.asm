; macros
; macros will differ for each assembler, this document is for NASM
; macros will expand in the code after compiled, it is like c macro


section .data
    digit db 0,10

section .text
    global _start

%macro exit 0                           ; exit -> name of macro, 0 -> The no.of arguments for macro
        mov rax, 60
        mov rdi, 0
        syscall
%endmacro

                                        ; arguments are taken from macros by using "%index" index start from 1

%macro printDigit 1
        mov rax, %1
        call printRAXDigit
%endmacro

%macro printDigitSum 2
        mov rax, %1
        add rax, %2
        call printRAXDigit
%endmacro


printRAXDigit:
    add rax, 48
    mov [digit], al
    mov rax, 1
    mov rdi, 1
    mov rsi, digit
    mov rdx, 2
    syscall
    ret


_start:
    printDigit 3                         ; calling macro by name and passing argument          
    printDigit 4
    
    printDigitSum 1,2                    ; passing more than 1 argument
    exit

; if we specify labels in macroc when it expand in code after compile the label will consider as defined twice
; so it will raise redefined symbol error
; to solve this issue use "%%" befor defining and call the labels 

;eg without %%

; %macro stopexe 0
; _loop:
;       jmp _loop
; %endmacro

; lets see how the above code will expand


; stopexe
; stopexe

; the code is calling macro it is before compile, the below code is after compile

; _loop:
;       jmp _loop
; _loop:
;       jmp _loop
;---------------------------------------------------------------

;eg with %%

; %macro stopexe 0
; %%_loop:
;       jmp %%_loop
; %endmacro

; stopexe
; stopexe

; the code is calling macro it is before compile, the below code is after compile

; _loop:
;       jmp _loop
;       jmp _loop