.section .rodata

cls_str: .ascii "\033[H\033[2J"
.set CLS_SIZE, . - cls_str

greet_str: .ascii "______ELF_64_CALCULATOR______\n"
.set GREET_SIZE, . - greet_str

.section .data

.set BUF_SIZE, 256
input_buf: .skip BUF_SIZE
result_buf: .ascii "done...\n"

.set READ, 0
.set WRITE, 1
.set EXIT, 60

.set STDIN, 0
.set STDOUT, 1

.set STACK_SIZE 256

.set input_size, 0
.set result_size, 8

.section .text
.global  _start

_start:
	movq %rsp, %rbp
	subq $STACK_SIZE, %rbp

	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $greet_str, %rsi
	movq $GREET_SIZE, %rdx
	syscall

	movq $8, result_size(%rbp)
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
	movq input_size(%rbp), %rdx
	syscall

	movq $WRITE, %rax
	movq $STDOUT, %rdi
	movq $result_buf, %rsi
	movq result_size(%rbp), %rdx
	syscall

loop_start:
	movq $READ, %rax
	movq $STDIN, %rdi
	movq $input_buf, %rsi
	movq $BUF_SIZE, %rdx
	syscall
	movq %rax, input_size(%rbp)

	jmp loop

addq $STACK_SIZE, %rbp
movq $EXIT, %rax
movq $0, %rdi
syscall
