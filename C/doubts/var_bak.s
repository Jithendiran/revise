	.file	"var.c"
	.intel_syntax noprefix
	.text
;=============================================		SEGMENT
	.globl	bb
	.bss
	.align 4
	.type	bb, @object
	.size	bb, 4
bb:
;----------------------------------------------		bss
	.zero	4
	.globl	ga
	.data
	.type	ga, @object
	.size	ga, 1
ga:

	.byte	97
	.globl	gi
	.align 4
	.type	gi, @object
	.size	gi, 4
gi:

	.long	3
	.align 4
	.type	sgi, @object
	.size	sgi, 4
sgi:		 													; non global 
;------------------------------------------------ 	data
	.long	2
	.globl	gci
	.section	.rodata
	.align 4
	.type	gci, @object
	.size	gci, 4
gci:
;-----------------------------------------------   rodata
	.long	27
	.local	sibs
	.comm	sibs,4,4
;------------------------------------------------- .local?, belongs to which section
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -4[rbp], 25
	mov	DWORD PTR -8[rbp], 34
	mov	BYTE PTR ga[rip], 98
	mov	DWORD PTR gi[rip], 2000
	mov	DWORD PTR sgi[rip], 12
	mov	DWORD PTR -4[rbp], 27
	mov	DWORD PTR si.0[rip], 369
	nop
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
;-----------------------------------------------  text
	.size	main, .-main
	.data
	.align 4
	.type	si.0, @object
	.size	si.0, 4
si.0:
;-----------------------------------------------  data 		; non global
	.long	12
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
