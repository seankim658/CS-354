	.file	"decode.c"
	.section	.rodata
.LC0:
	.string	"Ciphertext:\n%s\n"
.LC1:
	.string	"Plaintext:\n%s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	call	read_cipher_file
	movl	%eax, 20(%esp)
	movl	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC0, (%esp)
	call	printf
	call	get_login_key
	movl	%eax, 24(%esp)
	movl	24(%esp), %eax
	movl	%eax, 4(%esp)
	movl	20(%esp), %eax
	movl	%eax, (%esp)
	call	decode
	movl	%eax, 28(%esp)
	movl	28(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$.LC1, (%esp)
	call	printf
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.section	.rodata
.LC2:
	.string	"r"
.LC3:
	.string	"ciphertext.txt"
	.align 4
.LC4:
	.string	"Cannot open file for reading.\n"
	.align 4
.LC5:
	.string	"Error reading ciphertext file.\n"
	.text
	.globl	read_cipher_file
	.type	read_cipher_file, @function
read_cipher_file:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$.LC2, 4(%esp)
	movl	$.LC3, (%esp)
	call	fopen
	movl	%eax, -20(%ebp)
	cmpl	$0, -20(%ebp)
	jne	.L4
	movl	stderr, %eax
	movl	%eax, 12(%esp)
	movl	$30, 8(%esp)
	movl	$1, 4(%esp)
	movl	$.LC4, (%esp)
	call	fwrite
	movl	$1, (%esp)
	call	exit
.L4:
	movl	$1000, (%esp)
	call	malloc
	movl	%eax, -16(%ebp)
	movl	-20(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	$1000, 4(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	fgets
	testl	%eax, %eax
	jne	.L5
	movl	stderr, %eax
	movl	%eax, 12(%esp)
	movl	$31, 8(%esp)
	movl	$1, 4(%esp)
	movl	$.LC5, (%esp)
	call	fwrite
	movl	$1, (%esp)
	call	exit
.L5:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	strlen
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	leal	-1(%eax), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	cmpb	$10, %al
	jne	.L6
	movl	-12(%ebp), %eax
	leal	-1(%eax), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movb	$0, (%eax)
.L6:
	movl	-16(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	read_cipher_file, .-read_cipher_file
	.section	.rodata
.LC6:
	.string	"Enter your CS login: "
.LC7:
	.string	"Error reading user input.\n"
	.text
	.globl	get_login_key
	.type	get_login_key, @function
get_login_key:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$50, (%esp)
	call	malloc
	movl	%eax, -16(%ebp)
	movl	$.LC6, (%esp)
	call	printf
	movl	stdin, %eax
	movl	%eax, 8(%esp)
	movl	$50, 4(%esp)
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	fgets
	testl	%eax, %eax
	jne	.L9
	movl	stderr, %eax
	movl	%eax, 12(%esp)
	movl	$26, 8(%esp)
	movl	$1, 4(%esp)
	movl	$.LC7, (%esp)
	call	fwrite
	movl	$1, (%esp)
	call	exit
.L9:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	call	strlen
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	leal	-1(%eax), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movzbl	(%eax), %eax
	cmpb	$10, %al
	jne	.L10
	movl	-12(%ebp), %eax
	leal	-1(%eax), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movb	$0, (%eax)
.L10:
	movl	-16(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	get_login_key, .-get_login_key
	.globl	decode
	.type	decode, @function
decode:
.LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	12(%ebp), %eax
	movl	%eax, (%esp)
	call	calculate_shifts
	movl	%eax, -20(%ebp)
	movl	8(%ebp), %eax
	movl	%eax, -24(%ebp)
	jmp	.L13
.L17:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$96, %al
	jle	.L14
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	cmpb	$122, %al
	jle	.L15
.L14:
	jmp	.L16
.L15:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	subl	$97, %eax
	movl	%eax, -16(%ebp)
	movl	-20(%ebp), %eax
	movl	-16(%ebp), %edx
	leal	(%edx,%eax), %ecx
	movl	$1321528399, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$3, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	imull	$26, %eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, -12(%ebp)
	movl	-12(%ebp), %eax
	addl	$97, %eax
	movl	%eax, %edx
	movl	-24(%ebp), %eax
	movb	%dl, (%eax)
.L16:
	addl	$1, -24(%ebp)
.L13:
	movl	-24(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L17
	movl	8(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	decode, .-decode
	.globl	calculate_shifts
	.type	calculate_shifts, @function
calculate_shifts:
.LFB6:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movl	%eax, -8(%ebp)
	movl	$0, -4(%ebp)
	jmp	.L20
.L21:
	movl	-8(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	xorl	%eax, -4(%ebp)
	addl	$1, -8(%ebp)
.L20:
	movl	-8(%ebp), %eax
	movzbl	(%eax), %eax
	testb	%al, %al
	jne	.L21
	movl	-4(%ebp), %ecx
	movl	$1321528399, %edx
	movl	%ecx, %eax
	imull	%edx
	sarl	$3, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	imull	$26, %eax, %eax
	subl	%eax, %ecx
	movl	%ecx, %eax
	cltd
	xorl	%edx, %eax
	movl	%eax, -4(%ebp)
	subl	%edx, -4(%ebp)
	cmpl	$0, -4(%ebp)
	jne	.L22
	movl	$1, -4(%ebp)
.L22:
	movl	-4(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	calculate_shifts, .-calculate_shifts
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
