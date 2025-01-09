.section .rodata

cls_str: .ascii "\033[H\033[2J"
.set CLS_SIZE, . - cls_str

greet_str: .ascii "______ELF_64_CALCULATOR______\n"
.set GREET_SIZE, . - greet_str

format_str: .asciz "%d\n"

sig_action: .quad exit
.quad 0x04000000

.section .bss

.set BUF_SIZE, 1096

result_buf: .skip BUF_SIZE
input_buf: .skip BUF_SIZE
token_buf: .skip BUF_SIZE

input_size: .skip 4
token_size: .skip 4
result_size: .skip 4

.set READ, 0
.set WRITE, 1
.set SIG_ACTION, 13
.set EXIT, 60

.set STDIN, 0
.set STDOUT, 1

.set NULL, 0
.set SIGINT, 2

.set OP_PLUS, 0x8000000000000000
.set OP_B_MINUS, 0x8000000000000001
.set OP_MUL, 0x8000000000000002
.set OP_DIV, 0x8000000000000003
.set OP_U_MINUS, 0x8000000000000004
.set OP_PAREN_R, 0x8000000000000005
.set OP_PAREN_L, 0x8000000000000006

.section .text
.global  _start

_start:
	movl $SIG_ACTION, %eax
	movl $SIGINT, %edi
	movl $sig_action, %esi
	movl $NULL, %edx
	movl $8, %r10d
	syscall

	movl $WRITE, %eax
	movl $STDOUT, %edi
	movl $greet_str, %esi
	movl $GREET_SIZE, %edx
	syscall

	jmp loop_start

loop:
	movl $WRITE, %eax
	movl $STDOUT, %edi
	movl $cls_str, %esi
	movl $CLS_SIZE, %edx
	syscall

	movl $WRITE, %eax
	movl $STDOUT, %edi
	movl $greet_str, %esi
	movl $GREET_SIZE, %edx
	syscall

	movl $WRITE, %eax
	movl $STDOUT, %edi
	movl $input_buf, %esi
	movl (input_size), %edx
	syscall

# movq $WRITE, %rax
# movq $STDOUT, %rdi
# movq $result_buf, %rsi
# movl (result_size), %edx
# syscall

loop_start:
	movl $READ, %eax
	movl $STDIN, %edi
	movl $input_buf, %esi
	movl $BUF_SIZE, %edx
	syscall

	movl $input_buf, %ebx # input
	movl %eax, %ecx # counter
	xorl %esi, %esi # result
	xorl %edi, %edi # fraction
	movl $1, %r8d # fraction_div
	xorl %r9d, %r9d # count
	xorb %r10b, %r10b # fractional

	testl %ecx, %ecx
	jmp   char_loop_start

char_loop:
	incl %ebx
	decl %ecx

char_loop_start:
	jnz  char_loop_body
	movl %r9d, (token_size)
	jmp  loop

char_loop_body:
	movl %esi, %eax
	xorl %eax, %edi
	jz   1f

	shlq $32, %rsi
	shlq $32, %rdi

	movl %r8d, %eax
	shll $1, %eax

	addq %rdi, %rax
	xorl %edx, %edx
	divq %r8

	addq %rsi, %rax
	movq %rax, token_buf(%r9d)
	incl %r9d

	xorl %esi, %esi
	xorl %edi, %edi
	movl $1, %r8d
	xorl %r9d, %r9d
	xorb %r10b, %r10b

1:
	movb (%ebx), %r11b # char

	movb %r11b, %dl
	subb $'0', %dl
	jbe  2f

	testb %r10b, %r10b
	jz    1f
	jmp   char_loop

1:
	jmp char_loop

2:
	jmp char_loop

exit:
	movl $EXIT, %eax
	movl $0, %edi
	syscall
