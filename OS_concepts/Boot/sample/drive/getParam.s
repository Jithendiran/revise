;nasm getParam.s -f bin -o boot.img 
;qemu-system-i386 ./boot.img

[org 0x7C00]
[BITS 16]

section .text
	global _start
	jmp _start

_start:
	mov	ah,0x08		; 08 read drive parameter
    mov	dl,0x00     ; drive index 00 => 1st floppy, 80 => 1st HDD
	int	0x13
    ; return CF => 1 if error, 0 if success
    ; AH => return code
    ; DL => number of hard disk drives
    ; DH => logical last index of head
    ; CX => [5:0] logical last index of sector, [15:8][7:6] logical last index of cylinder 
    ; BL => drive type ( only AT/PS2 floppies)
    ; ES:DI => pointer to drive parameter table (only for floppies)

    jnc ok
    jmp error

ok:
    mov ah, 0x0E
    mov al, 'O'
    mov bh, 0x00
    mov bl, 0x02
    int 10h
    jmp done

error:
    mov ah, 0x0E
    mov al, 'E'
    mov bh, 0x00
    mov bl, 0x02
    int 10h

done:
    jmp $

times 510-($-$$) db 0
dw 0xAA55