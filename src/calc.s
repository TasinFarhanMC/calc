.section .rodata

cls_str: .ascii "\033[H\033[2J"
.set CLS_SIZE, . - cls_str

greet_str: .ascii "______ELF_64_CALCULATOR______\n"
.set GREET_SIZE, . - greet_str

sig_action: .quad exit
.quad 0x04000000

.section .data

.set BUF_SIZE, 256
input_buf: .skip BUF_SIZE
result_buf: .ascii "done...\n"
input_size: .quad 0
result_size: .quad 0

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
	movq $SIG_ACTION, %rax
	movq $SIGINT, %rdi
	movq $sig_action, %rsi
	movq $NULL, %rdx
	movq $8, %r10
	syscall

	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $greet_str, %rsi
	movq $GREET_SIZE, %rdx
	syscall

	movq $8, (result_size)
	jmp  loop_start

loop:
	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $cls_str, %rsi
	movq $CLS_SIZE, %rdx
	syscall

	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $greet_str, %rsi
	movq $GREET_SIZE, %rdx
	syscall

	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $input_buf, %rsi
	movq (input_size), %rdx
	syscall

	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $result_buf, %rsi
	movq (result_size), %rdx
	syscall

loop_start:
	movq $READ, %rax
	movq $STDIN, %rdi
	movq $input_buf, %rsi
	movq $BUF_SIZE, %rdx
	syscall
	movq %rax, (input_size)

	jmp loop

exit:
	movq $EXIT, %rax
	movq $0, %rdi
	syscall
