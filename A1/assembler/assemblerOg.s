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
	.text
	.globl	main
	.type	main, @function
main:
.LFB11:
	.cfi_startproc
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	subq	$16, %rsp
	.cfi_def_cfa_offset 48
	movq	%rsi, %r12
	call	clock
	movq	%rax, %rbp
	movl	$0, %eax
	movl	$0, %ebx
	jmp	.L2
.L3:
	movslq	%eax, %rdx
	addq	%rdx, %rbx
	addl	$1, %eax
.L2:
	cmpl	$1000000000, %eax
	jle	.L3
	call	clock
	subq	%rbp, %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC0(%rip), %xmm0
	movsd	%xmm0, 8(%rsp)
	movl	$.LC1, %esi
	movl	$.LC2, %edi
	call	fopen
	movq	%rax, %rbp
	movq	8(%r12), %rcx
	movsd	8(%rsp), %xmm0
	movq	%rbx, %rdx
	movl	$.LC3, %esi
	movq	%rax, %rdi
	movl	$1, %eax
	call	fprintf
	movq	%rbp, %rdi
	call	fclose
	movl	$0, %eax
	addq	$16, %rsp
	.cfi_def_cfa_offset 32
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
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
