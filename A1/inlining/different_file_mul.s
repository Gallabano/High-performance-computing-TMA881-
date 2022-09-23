	.file	"different_file_mul.c"
	.text
	.p2align 4
	.globl	different_file_mul
	.type	different_file_mul, @function
different_file_mul:
.LFB0:
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
.LFE0:
	.size	different_file_mul, .-different_file_mul
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
