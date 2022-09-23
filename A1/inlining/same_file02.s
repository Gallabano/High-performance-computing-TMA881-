	.file	"same_file.c"
	.text
	.p2align 4
	.globl	mul_cpx_mainfile
	.type	mul_cpx_mainfile, @function
mul_cpx_mainfile:
.LFB11:
	.cfi_startproc
	movsd	(%rdx), %xmm0
	movsd	(%rcx), %xmm1
	mulsd	(%r8), %xmm0
	mulsd	(%r9), %xmm1
	subsd	%xmm1, %xmm0
	movsd	%xmm0, (%rdi)
	movsd	(%rdx), %xmm0
	movsd	(%rcx), %xmm1
	mulsd	(%r9), %xmm0
	mulsd	(%r8), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, (%rsi)
	ret
	.cfi_endproc
.LFE11:
	.size	mul_cpx_mainfile, .-mul_cpx_mainfile
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"Elapsed time of %d iterations: %f\n"
	.align 8
.LC6:
	.string	"First element of as with real and imaginary parts respectively: %f, %f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB12:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$480000, %rsp
	.cfi_def_cfa_offset 480016
	call	clock
	movsd	.LC1(%rip), %xmm1
	movl	$200000, %edx
	movsd	.LC3(%rip), %xmm0
	movq	%rax, %rbx
	unpcklpd	%xmm1, %xmm1
	unpcklpd	%xmm0, %xmm0
	.p2align 4,,10
	.p2align 3
.L4:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L5:
	movaps	%xmm1, (%rsp,%rax)
	movaps	%xmm0, 240000(%rsp,%rax)
	addq	$16, %rax
	cmpq	$240000, %rax
	jne	.L5
	subl	$1, %edx
	jne	.L4
	call	clock
	pxor	%xmm0, %xmm0
	movl	$200000, %esi
	movl	$.LC5, %edi
	subq	%rbx, %rax
	cvtsi2sdq	%rax, %xmm0
	movl	$1, %eax
	divsd	.LC4(%rip), %xmm0
	call	printf
	movsd	(%rsp), %xmm0
	movl	$.LC6, %edi
	movsd	240000(%rsp), %xmm1
	movl	$2, %eax
	call	printf
	addq	$480000, %rsp
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	-1073217536
	.align 8
.LC3:
	.long	0
	.long	1074790400
	.align 8
.LC4:
	.long	0
	.long	1093567616
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
