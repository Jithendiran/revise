; compile: nasm -f elf64 -o a.o nasm_10.asm && ld a.o -o a.out
section .data
    num DB 1                    ; DB -> define bytes. Each char one byte
    num2 DB 2
    char DB 'A'                 ; character stored as 65
    list DB 1,2,3,4             ;0x04030201 -> this is how values stored
    string DB "HEL",0
    string1 DB "LLO",0
    num3 DB 3 DUP(2)            ; initialize 2 values 3 times  0x00020202

section .bss
    numr RESB 3

; in above list what is problem is we exactly don't know where the data is end. we don't know size
; for the above problem we can use 0 in last or -1 in last or know the size to fetch

; for string it is easier to know the end of the the string by placing null(0) character

section .text
global _start

_start:
    mov bl, [num]              ; bl -> lower 8bit of bx, bx is 16 bit of ebx, ebx is 32 bit of rbx
    mov cl, [num2]             ; cl -> lower 8bit of cx
    mov dl, [list]
    mov bl, [string]           ; assume memory address is 0x402007
    mov cl, [string1] 
    ;mov [numr], 1             ; will not work because size of the value is don't know
    mov bl,1                   ; bl is 1 byte
    mov  [numr], bl            ; moving 1 byte of data to numr now size is know because of bl
    ; x86 use little endian encoding, least significiant bit is starting value
    ; numr is 0x402010 location will be 0x00000001
    mov bl,2
    mov  [numr+1], bl          ; numr points to the 1st memory location +1 means get 2nd memory location
    ; numr is 0x402011 location will be 0x00000201
    mov bl,3
    mov  [numr+2], bl          ; +1 means 1 byte, +2 means 2 byte
    ; numr is 0x402012 location will be 0x00030201
    mov bl,4
    mov  [numr+3], bl
    ; numr is 0x402013 location will be 0x04030201
    mov rax, 1
    INT 80h

; to see use gdb debugger run following commands
; gdb ./a.out
; layout asm
; break _start
; run
; stepi -> go to next step
; info register rbx -> to see content of rax
; x/x 0x402000 -> to see the content of the address 0x402000
;x/3x 0x402000 -> which means looking 3 memory slots from given address

; when moving values to lower bit (bl), on the time of reference using bl, bx, ebx all values will give same result
; when moving values to higher bit (bh), on the time of reference using bh, bx, ebx will vary because the higher most bit is set to 1. so result will vary

; do experiment