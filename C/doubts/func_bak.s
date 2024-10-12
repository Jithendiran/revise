	.file	"func.c"
	.intel_syntax noprefix
;=================================================================
	.text
	.local	b
	.comm	b,4,4
;--------------------------------------------------------------------
	.globl	func
	.type	func, @function
func:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	eax, DWORD PTR aa.2[rip]
	add	eax, 1
	mov	DWORD PTR aa.2[rip], eax
	mov	eax, DWORD PTR b[rip]
	add	eax, 1
	mov	DWORD PTR b[rip], eax
	mov	eax, DWORD PTR aa.2[rip]
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	func, .-func
;----------------------------------------------------------------------

	.globl	func2
	.type	func2, @function
func2:
.LFB1:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -4[rbp], edi		; function's arg variable c
	mov	eax, DWORD PTR aa.1[rip]
	add	eax, 1
	mov	DWORD PTR aa.1[rip], eax
	mov	edx, DWORD PTR aa.1[rip]
	mov	eax, DWORD PTR -4[rbp]
	add	eax, edx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	func2, .-func2
;-------------------------------------------------------------------------

	.type	fun3, @function
fun3:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	mov	DWORD PTR -20[rbp], edi		; function's arg variable a
	mov	eax, DWORD PTR aa.0[rip]
	add	eax, 1
	mov	DWORD PTR aa.0[rip], eax
	mov	DWORD PTR -4[rbp], 2		; local variable ab
	mov	edx, DWORD PTR aa.0[rip]
	mov	eax, DWORD PTR -20[rbp]
	add	edx, eax
	mov	eax, DWORD PTR -4[rbp]
	add	eax, edx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	fun3, .-fun3
;--------------------------------------------------------------------------

	.local	aa.2
	.comm	aa.2,4,4

	.data
	.align 4
	.type	aa.1, @object
	.size	aa.1, 4
aa.1:

	.long	1
	.local	aa.0
	.comm	aa.0,4,4
;----------------------------------------------------------------------------
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
