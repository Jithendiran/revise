;nasm printChar.s -f bin -o boot.img 
;qemu-system-i386 ./boot.img

; int 10 is for video mode

[org 0x7C00]
[BITS 16]

section .text
	global _start
	jmp _start

_start:
    mov ah, 0x0E        ;Teletype (tty) output
    mov al, 'A'         ;character to print
    mov bh, 0x00        ;BH = Page Number   BH stick with 0
    mov bl, 0x02        ;BL = Color (work's in graphics mode) 0x02 means green
    int 10h

;-----------------------------------------------------------------------
    ;with loop
    ;setting parameter
    mov ah, 0x0E
    mov bh, 0x00

    ; pointing si to string address
    mov si, msg

loop:
    mov al, [si] ; move content of si to al
    inc si      ; increment si to point next address

    cmp al, 0   ; compare al is 0, if 0 reached end of string 
    je end      ; cmp will set ZF 0 to if equal 

    int 10h
    jmp loop

end:

    mov ah, 0x0E        ;Teletype (tty) output
    mov al, 'Z'         ;character to print
    mov bh, 0x00        ;BH = Page Number   BH stick with 0
    mov bl, 0x02        ;BL = Color (work's in graphics mode) 0x02 means green
    int 10h
;------------------------------------------------------------------------
    jmp $

msg db "jith",0
times 510-($-$$) db 0
dw 0xAA55