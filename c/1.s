	.file	"bit.c"
	.section	.rodata
.LC0:
	.string	"%llu\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$32, %esp
	movl	$0, 16(%esp)
	movl	$0, 20(%esp)
	movl	$168, 24(%esp)
	movl	$3, 28(%esp)
	movl	28(%esp), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	movl	%eax, 16(%esp)
	movl	%edx, 20(%esp)
	movl	16(%esp), %eax
	movl	20(%esp), %edx
	movl	%eax, %edx
	movl	$0, %eax
	movl	%eax, 16(%esp)
	movl	%edx, 20(%esp)
	movl	$.LC0, %ecx
	movl	16(%esp), %eax
	movl	20(%esp), %edx
	movl	%eax, 4(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, (%esp)
	call	printf
	movl	24(%esp), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	addl	%eax, 16(%esp)
	adcl	%edx, 20(%esp)
	movl	$.LC0, %ecx
	movl	16(%esp), %eax
	movl	20(%esp), %edx
	movl	%eax, 4(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, (%esp)
	call	printf
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-4)"
	.section	.note.GNU-stack,"",@progbits
