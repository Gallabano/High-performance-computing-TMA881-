	.file	"assembler.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"a"
.LC2:
	.string	"naiveBenchmarking.txt"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"\nValue of sum: %llu. It took %f s using optimization: %s"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	call	clock
	movq	%rax, %rbx
	call	clock
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	pxor	%xmm0, %xmm0
	subq	%rbx, %rax
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC0(%rip), %xmm0
	movsd	%xmm0, 8(%rsp)
	call	fopen
	movq	8(%rbp), %rcx
	movsd	8(%rsp), %xmm0
	movabsq	$500000000500000000, %rdx
	movq	%rax, %rbx
	movq	%rax, %rdi
	movl	$.LC3, %esi
	movl	$1, %eax
	call	fprintf
	movq	%rbx, %rdi
	call	fclose
	addq	$24, %rsp
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE11:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1093567616
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
