	.file	"different_file.c"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"Elapsed time of %d iterations: %f\n"
	.align 8
.LC4:
	.string	"First element of as with real and imaginary parts respectively: %f, %f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	movl	$200000, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	subq	$1440008, %rsp
	.cfi_def_cfa_offset 1440048
	call	clock
	movq	%rax, %r13
	.p2align 4,,10
	.p2align 3
.L2:
	leaq	240000(%rsp), %rbp
	xorl	%ebx, %ebx
	.p2align 4,,10
	.p2align 3
.L3:
	movq	.LC0(%rip), %rax
	leaq	(%rsp,%rbx), %rdi
	movq	%rbp, %rdx
	leaq	1200000(%rsp,%rbx), %r9
	leaq	480000(%rsp,%rbx), %r8
	addq	$8, %rbp
	movq	%rax, -8(%rbp)
	movq	.LC1(%rip), %rax
	movq	%rax, 960000(%rsp,%rbx)
	movq	.LC0(%rip), %rax
	movq	%rax, 480000(%rsp,%rbx)
	movq	.LC1(%rip), %rax
	movq	%rax, 1200000(%rsp,%rbx)
	leaq	960000(%rsp), %rax
	leaq	(%rax,%rbx), %rcx
	leaq	720000(%rsp), %rax
	leaq	(%rax,%rbx), %rsi
	addq	$8, %rbx
	call	different_file_mul
	cmpq	$240000, %rbx
	jne	.L3
	subl	$1, %r12d
	jne	.L2
	call	clock
	pxor	%xmm0, %xmm0
	movl	$200000, %esi
	movl	$.LC3, %edi
	subq	%r13, %rax
	cvtsi2sdq	%rax, %xmm0
	movl	$1, %eax
	divsd	.LC2(%rip), %xmm0
	call	printf
	movsd	(%rsp), %xmm0
	movl	$.LC4, %edi
	movsd	720000(%rsp), %xmm1
	movl	$2, %eax
	call	printf
	addq	$1440008, %rsp
	.cfi_def_cfa_offset 40
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	1073741824
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
