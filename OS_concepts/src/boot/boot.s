;nasm boot.s -f bin -o boot.bin
;cp boot.bin boot.img
;truncate -s 1440k boot.img 

[org 0x7C00]
[BITS 16]

SETUPLEN equ 4				; no.of setup sectors
BOOTSEG  equ 0x07c0			; BIOS address of boot-sector
INITSEG  equ 0x9000			; new boot-sector address

section .text
	global _start
	jmp _start

_start:
	mov ax, BOOTSEG
	mov ds, ax
	mov ax, INITSEG
	mov es, ax
	mov cx, 256
	mov si, 0
	mov di, 0
	rep movsw		; copy ds:si to es:di then increment si, di
	jmp INITSEG:go	; once copied jump to new code segment with offset
; from below code Code segment is changed to 9000 and instruction pointer offset address remains same

go: mov ax, cs		
	mov ds, ax		; point ds and es to new code segment
	mov es, ax
	mov ss, ax 		; set stack segment at INITSEG
	mov sp, 0xFF00	; place stack pointer to FF00
	; now address of stack is 0x9FF00

	; Get page no, row no, column no
    mov	ah,0x03
	int	0x10

load_setup:
	mov dx, 0x0000 
	mov	cx, 0x0002		
	mov	bx, 0x0200		
	mov	ax, 0x0200+SETUPLEN	
	int	0x13			
	jnc	ok_load_setup		

	mov	dx,#0x0000
	mov	ax,#0x0000		
	int	0x13
	j	load_setup

ok_load_setup:

	; Printing OS info
	mov	ax, 0x1301      
    mov bp, msg         
	mov cx, msglen      
	mov bl, 0x02 		
	int 0x10
	jmp $

msg db "ji OS Loading........",10,0
msglen equ $-msg
times 510-($-$$) db 0
dw 0xAA55