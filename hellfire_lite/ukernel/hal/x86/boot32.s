/*
 *      longjmp(a,v)
 * will generate a "return(v)" from
 * the last call to
 *      setjmp(a)
 * by restoring registers from the stack,
 * The previous signal state is NOT restored.
 *      HF_RestoreExecution(a,v)
 * is the same as longjmp(a,v), except it tells
 * EOI to the 8259 chip and re-enables the interrupts
 *
 */

.code32

.text
.globl setjmp
.type    setjmp,@function
setjmp:

	movl  4(%esp),%eax
	movl  %ebx,0(%eax)
	movl  %ecx,4(%eax)
	movl  %edx,8(%eax)
	movl  %esi,12(%eax)
	movl  %edi,16(%eax)
	movl  %ebp,20(%eax)
	popl  %edx
	movl  %esp,24(%eax)
	movl  %edx,28(%eax)
	xorl  %eax,%eax
	pushl %edx
	ret

.text
.globl longjmp
.type    longjmp,@function
longjmp:
	movl  4(%esp),%eax
	movl  8(%esp),%ebx

	test %ebx,%ebx
	jnz 0f
	incl %ebx
0:
	movl  %ebx,32(%eax)

	movl  0(%eax),%ebx
	movl  4(%eax),%ecx
	movl  8(%eax),%edx
	movl  12(%eax),%esi
	movl  16(%eax),%edi
	movl  20(%eax),%ebp
	movl  24(%eax),%esp
	pushl 28(%eax)
	movl  32(%eax),%eax
	ret

.text
.globl HF_RestoreExecution
.type    HF_RestoreExecution,@function
HF_RestoreExecution:
	movl  4(%esp),%eax
	movl  8(%esp),%ebx

	test %ebx,%ebx
	jnz 0f
	incl %ebx
0:
	movl  %ebx,32(%eax)

	movl  0(%eax),%ebx
	movl  4(%eax),%ecx
	movl  8(%eax),%edx
	movl  12(%eax),%esi
	movl  16(%eax),%edi
	movl  20(%eax),%ebp
	movl  24(%eax),%esp
	pushl 28(%eax)
	movl  32(%eax),%eax

	pusha
	movl	$0x20,0x4(%esp)		/* tell the interrupt controller */
	movl	$0x20,(%esp)		/* this interrupt has finished */
	call	outportb
	popa
	sti				/* re-enable interrupts */
        ret

.text
.globl HF_Halt
.type  HF_Halt, @function
HF_Halt:
	hlt
	ret

