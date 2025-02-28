	.file	"input.c"
	.text
	.p2align 4
	.globl	process_input
	.type	process_input, @function
process_input:
	movsbq	(%rdi), %rcx
	testb	%cl, %cl
	je	.L12
	movq	%rsi, %r11
	xorl	%edx, %edx
	xorl	%esi, %esi
	xorl	%eax, %eax
	xorl	%r8d, %r8d
	movl	$1, %r9d
	jmp	.L26
	.p2align 4,,10
	.p2align 3
.L36:
	movl	$1, %edx
.L28:
	movsbq	1(%rdi), %rcx
	addq	$1, %rdi
	testb	%cl, %cl
	je	.L35
.L26:
	cmpb	$32, %cl
	je	.L28
	cmpb	$46, %cl
	je	.L36
	pushq	%rbx
.L29:
	leal	-48(%rcx), %r10d
	cmpb	$9, %r10b
	ja	.L4
	testb	%dl, %dl
	je	.L5
	leaq	(%rax,%rax,4), %rax
	leaq	(%r9,%r9,4), %r9
	leaq	-48(%rcx,%rax,2), %rax
	addq	%r9, %r9
	.p2align 4
	.p2align 3
.L3:
	movsbq	1(%rdi), %rcx
	addq	$1, %rdi
	testb	%cl, %cl
	je	.L37
.L11:
	cmpb	$32, %cl
	je	.L3
	cmpb	$46, %cl
	jne	.L29
	movsbq	1(%rdi), %rcx
	addq	$1, %rdi
	movl	$1, %edx
	testb	%cl, %cl
	jne	.L11
.L37:
	movzbl	%sil, %eax
	popq	%rbx
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	movq	%r8, %rbx
	orq	%rax, %rbx
	je	.L6
	salq	$32, %rax
	movq	%r9, %rdx
	salq	$32, %r8
	movzbl	%sil, %r10d
	shrq	%rdx
	addl	$1, %esi
	addq	%rdx, %rax
	xorl	%edx, %edx
	divq	%r9
	xorl	%edx, %edx
	movl	$1, %r9d
	addq	%r8, %rax
	movq	%rax, (%r11,%r10,8)
.L6:
	cmpb	$45, %cl
	je	.L38
	leal	-40(%rcx), %r10d
	xorl	%eax, %eax
	xorl	%r8d, %r8d
	cmpb	$47, %r10b
	ja	.L3
	movabsq	$9223372036854775768, %rbx
	movzbl	%sil, %r10d
	addl	$1, %esi
	addq	%rbx, %rcx
	movq	%rcx, (%r11,%r10,8)
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L5:
	leaq	(%r8,%r8,4), %r8
	leaq	-48(%rcx,%r8,2), %r8
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L38:
	movzbl	%sil, %eax
	leaq	0(,%rax,8), %rcx
	testb	%sil, %sil
	je	.L8
	cmpq	$0, -8(%r11,%rcx)
	jns	.L9
.L8:
	movabsq	$-9223372036854775807, %r8
	cmpq	%r8, -8(%r11,%rcx)
	je	.L9
	movabsq	$-9223372036854775804, %rbx
	addl	$1, %esi
	xorl	%r8d, %r8d
	movq	%rbx, (%r11,%rax,8)
	xorl	%eax, %eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L9:
	movabsq	$-9223372036854775803, %rbx
	addl	$1, %esi
	xorl	%r8d, %r8d
	movq	%rbx, (%r11,%rax,8)
	xorl	%eax, %eax
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L35:
	movzbl	%sil, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L12:
	xorl	%eax, %eax
	ret
	.size	process_input, .-process_input
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"+: %lu\n"
.LC1:
	.string	"-: %lu\n"
.LC2:
	.string	"*: %lu\n"
.LC3:
	.string	"/: %lu\n"
.LC4:
	.string	"(-): %lu\n"
.LC5:
	.string	"(: %lu\n"
.LC6:
	.string	"): %lu\n"
.LC7:
	.string	""
.LC8:
	.string	"Token: %lu\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
	pushq	%r12
	movq	stdin(%rip), %rdx
	movl	$1024, %esi
	pushq	%rbp
	leaq	str(%rip), %rbp
	pushq	%rbx
	movq	%rbp, %rdi
	leaq	buf(%rip), %rbx
	call	fgets@PLT
	movq	%rbp, %rdi
	movq	%rbx, %rsi
	call	process_input
	leaq	.LC0(%rip), %rdi
	movabsq	$-9223372036854775805, %rsi
	movq	%rax, %rbp
	xorl	%eax, %eax
	call	printf@PLT
	leaq	.LC1(%rip), %rdi
	xorl	%eax, %eax
	movabsq	$-9223372036854775803, %rsi
	call	printf@PLT
	leaq	.LC2(%rip), %rdi
	xorl	%eax, %eax
	movabsq	$-9223372036854775806, %rsi
	call	printf@PLT
	leaq	.LC3(%rip), %rdi
	xorl	%eax, %eax
	movabsq	$-9223372036854775801, %rsi
	call	printf@PLT
	leaq	.LC4(%rip), %rdi
	xorl	%eax, %eax
	movabsq	$-9223372036854775804, %rsi
	call	printf@PLT
	leaq	.LC5(%rip), %rdi
	xorl	%eax, %eax
	movabsq	$-9223372036854775808, %rsi
	call	printf@PLT
	leaq	.LC6(%rip), %rdi
	xorl	%eax, %eax
	movabsq	$-9223372036854775807, %rsi
	call	printf@PLT
	leaq	.LC7(%rip), %rdi
	call	puts@PLT
	testq	%rbp, %rbp
	je	.L40
	leaq	(%rbx,%rbp,8), %r12
	leaq	.LC8(%rip), %rbp
	.p2align 4
	.p2align 3
.L41:
	movq	(%rbx), %rsi
	movq	%rbp, %rdi
	xorl	%eax, %eax
	addq	$8, %rbx
	call	printf@PLT
	cmpq	%r12, %rbx
	jne	.L41
.L40:
	popq	%rbx
	xorl	%eax, %eax
	popq	%rbp
	popq	%r12
	ret
	.size	main, .-main
	.globl	buf
	.bss
	.align 32
	.type	buf, @object
	.size	buf, 8192
buf:
	.zero	8192
	.globl	str
	.align 32
	.type	str, @object
	.size	str, 1024
str:
	.zero	1024
	.ident	"GCC: (GNU) 14.2.1 20240910"
	.section	.note.GNU-stack,"",@progbits
