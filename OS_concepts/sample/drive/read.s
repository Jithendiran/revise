;nasm read.s -f bin -o boot.img 
;qemu-system-i386 ./boot.img

[org 0x7C00]
[BITS 16]

section .text
	global _start
	jmp _start

_start:
    mov ah, 0x02 ; ah 02 is read sector from drive
	mov	al, 0x04 ; al is no.of sectors to read 

    mov dl, 0x00 ; dl is which drive, 00 => 1st floppy, 80 => 1st hard disk
	mov dh, 0x00 ; dh is which head number

	mov	ch, 0x00 ; ch is which cylinder number
    mov cl, 0x02 ; cl is wgich sector number
    ; register CX will be both cylinder and sectors
    ; CX =       ---CH--- ---CL---
    ; cylinder : 76543210 98
    ; sector   :            543210
    
	mov	bx, 0x0200 ; ES:BX buffer address pointer where the readed result will store
	int	0x13
    ;return CF => 1 if error, 0 if success
    ; AH => return code, AL => Actual sectors read count

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