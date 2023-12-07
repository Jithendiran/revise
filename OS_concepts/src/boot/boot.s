;nasm boot.s -f bin -o boot.bin
;cp boot.bin boot.img
;truncate -s 1440k boot.img 

org 0x7C00
BITS 16

message db "Ji OS Booting...........",0


section .text
    global _start



_start:

	; print string
	; mov	ah,0x03		
	; xor	bh,bh
	; int	0x10

	; mov ax, 0x7C00
	; mov es, ax

	; mov cx, msglength
	; mov bx, 0x0007
	; mov bp, message
	; mov bl, 0x02
	; mov	ax, 0x1300
	; int 0x10
	;---------------------------
	; print char
	; mov ah, 0x0E
	; mov al, 'A'
	; mov bh, 0x00
	; int 10h

	mov si, message

	mov ah, 0x0E
	mov bh, 0x00

	mov al, [si]
	; lodsb

	int 10h
	; call printStr
	cld
	jmp $


printStr:
	mov ah, 0x0E
	mov bh, 0x00
loop:
	lodsb
	; Load a byte from SI into AL
	; and then increase SI.

	; If AL contains a null-terminating
	; character, then stop printing.
	cmp al, 0x00
	je done
	int 10h
	jmp loop
done:
	ret

times 510-($-$$) db 0
dw 0xAA55