	.file	"locality.c"
	.text
	.globl	row_sums
	.type	row_sums, @function
row_sums:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	%rdx, -56(%rbp)
	movq	%rcx, -64(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L2
.L5:
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	movq	$0, -24(%rbp)
	jmp	.L3
.L4:
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-24(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movsd	-16(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	addq	$1, -24(%rbp)
.L3:
	movq	-24(%rbp), %rax
	cmpq	-64(%rbp), %rax
	jb	.L4
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movsd	-16(%rbp), %xmm0
	movsd	%xmm0, (%rax)
	addq	$1, -8(%rbp)
.L2:
	movq	-8(%rbp), %rax
	cmpq	-56(%rbp), %rax
	jb	.L5
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	row_sums, .-row_sums
	.globl	col_sums
	.type	col_sums, @function
col_sums:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -56(%rbp)
	movq	%rsi, -64(%rbp)
	movq	%rdx, -72(%rbp)
	movq	%rcx, -80(%rbp)
	movq	$0, -8(%rbp)
	jmp	.L7
.L10:
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -24(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -32(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -40(%rbp)
	movq	$0, -48(%rbp)
	jmp	.L8
.L9:
	movq	-48(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movsd	-16(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	movq	-48(%rbp), %rax
	addq	$1, %rax
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movsd	-24(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -24(%rbp)
	movq	-48(%rbp), %rax
	addq	$2, %rax
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movsd	-32(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -32(%rbp)
	movq	-48(%rbp), %rax
	addq	$3, %rax
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-8(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movsd	(%rax), %xmm0
	movsd	-40(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -40(%rbp)
	addq	$4, -48(%rbp)
.L8:
	movq	-48(%rbp), %rax
	cmpq	-80(%rbp), %rax
	jb	.L9
	movsd	-16(%rbp), %xmm0
	addsd	-24(%rbp), %xmm0
	addsd	-32(%rbp), %xmm0
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-56(%rbp), %rax
	addq	%rdx, %rax
	addsd	-40(%rbp), %xmm0
	movsd	%xmm0, (%rax)
	addq	$1, -8(%rbp)
.L7:
	movq	-8(%rbp), %rax
	cmpq	-72(%rbp), %rax
	jb	.L10
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	col_sums, .-col_sums
	.section	.rodata
	.align 8
.LC3:
	.string	"Row summation ---------------------------"
	.align 8
.LC4:
	.string	"Elapsed time of %d iterations: %f s. Average time for one loop: %f\n"
.LC5:
	.string	"Random element of sums: %f\n\n"
	.align 8
.LC6:
	.string	"Column summation ---------------------------"
.LC7:
	.string	"Random element of sums: %f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$8112, %rsp
	movq	$5000, -48(%rbp)
	movl	$8000000, %edi
	call	malloc
	movq	%rax, -56(%rbp)
	movl	$8000, %edi
	call	malloc
	movq	%rax, -64(%rbp)
	movq	$0, -8(%rbp)
	movq	$0, -16(%rbp)
	jmp	.L12
.L13:
	movq	-16(%rbp), %rax
	leaq	0(,%rax,8), %rcx
	movq	-8(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	-56(%rbp), %rdx
	addq	%rcx, %rdx
	movq	%rdx, (%rax)
	addq	$1, -8(%rbp)
	addq	$1000, -16(%rbp)
.L12:
	cmpq	$999, -8(%rbp)
	jbe	.L13
	movq	$0, -24(%rbp)
	jmp	.L14
.L17:
	movq	$0, -32(%rbp)
	jmp	.L15
.L16:
	movq	-24(%rbp), %rax
	leaq	0(,%rax,8), %rdx
	movq	-64(%rbp), %rax
	addq	%rdx, %rax
	movq	(%rax), %rax
	movq	-32(%rbp), %rdx
	salq	$3, %rdx
	addq	%rdx, %rax
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, (%rax)
	addq	$1, -32(%rbp)
.L15:
	cmpq	$999, -32(%rbp)
	jbe	.L16
	addq	$1, -24(%rbp)
.L14:
	cmpq	$999, -24(%rbp)
	jbe	.L17
	call	clock
	movq	%rax, -72(%rbp)
	movl	$0, -36(%rbp)
	jmp	.L18
.L19:
	movq	-64(%rbp), %rsi
	leaq	-8112(%rbp), %rax
	movl	$1000, %ecx
	movl	$1000, %edx
	movq	%rax, %rdi
	call	row_sums
	addl	$1, -36(%rbp)
.L18:
	movl	-36(%rbp), %eax
	cltq
	cmpq	-48(%rbp), %rax
	jb	.L19
	call	clock
	movq	%rax, -80(%rbp)
	movq	-80(%rbp), %rax
	subq	-72(%rbp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC2(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -88(%rbp)
	movl	$.LC3, %edi
	call	puts
	movq	-48(%rbp), %rax
	testq	%rax, %rax
	js	.L20
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	jmp	.L21
.L20:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L21:
	movsd	-88(%rbp), %xmm1
	divsd	%xmm0, %xmm1
	movq	-88(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %xmm0
	movq	%rax, %rsi
	movl	$.LC4, %edi
	movl	$2, %eax
	call	printf
	movq	-8112(%rbp), %rax
	movq	%rax, %xmm0
	movl	$.LC5, %edi
	movl	$1, %eax
	call	printf
	call	clock
	movq	%rax, -96(%rbp)
	movl	$0, -40(%rbp)
	jmp	.L22
.L23:
	movq	-64(%rbp), %rsi
	leaq	-8112(%rbp), %rax
	movl	$1000, %ecx
	movl	$1000, %edx
	movq	%rax, %rdi
	call	col_sums
	addl	$1, -40(%rbp)
.L22:
	movl	-40(%rbp), %eax
	cltq
	cmpq	-48(%rbp), %rax
	jb	.L23
	call	clock
	movq	%rax, -104(%rbp)
	movq	-104(%rbp), %rax
	subq	-96(%rbp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC2(%rip), %xmm1
	divsd	%xmm1, %xmm0
	movsd	%xmm0, -112(%rbp)
	movl	$.LC6, %edi
	call	puts
	movq	-48(%rbp), %rax
	testq	%rax, %rax
	js	.L24
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	jmp	.L25
.L24:
	movq	%rax, %rdx
	shrq	%rdx
	andl	$1, %eax
	orq	%rax, %rdx
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rdx, %xmm0
	addsd	%xmm0, %xmm0
.L25:
	movsd	-112(%rbp), %xmm1
	divsd	%xmm0, %xmm1
	movq	-112(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %xmm0
	movq	%rax, %rsi
	movl	$.LC4, %edi
	movl	$2, %eax
	call	printf
	movq	-8112(%rbp), %rax
	movq	%rax, %xmm0
	movl	$.LC7, %edi
	movl	$1, %eax
	call	printf
	movq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movq	-56(%rbp), %rax
	movq	%rax, %rdi
	call	free
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	0
	.long	1072693248
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
