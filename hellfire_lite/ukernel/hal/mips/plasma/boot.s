#
#    This file is part of HellFire Lite.
#
#    HellFire is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    HellFire is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with HellFire.  If not, see <http://www.gnu.org/licenses/>.
#

# =====================================================================
# file: boot.s
# author: sergio johann filho [sergiojohannfilho@gmail.com]
# date: 04/2008
# last revision: 11/2013
# function: boot and context switch related code.
# =====================================================================
# MIPS specific boot and context switch code
# =====================================================================

#Reserve 512 bytes for stack
	.comm InitStack, 512

	.text
	.align 2

	.global _entry
	.ent _entry	
_entry:
	.set noreorder

#	la    $gp, _gp				# isso aki tem que ver.
	la    $a1, __bss_start
	la    $a0, _end
	la    $sp, InitStack+488

$BSS_CLEAR:
	sw    $zero, 0($a1)
	slt   $v1, $a1, $a0
	bnez  $v1, $BSS_CLEAR
	addiu $a1, $a1, 4

	jal   HF_Main
	nop

$L1:
	beq $zero, $zero, $L1
	nop
	nop
	nop
	nop
	nop
	nop
	nop

.end _entry


###################################################
# Interrupt service routine. Saves temporary registers and calls the kernel.
# It will call the interrupt handler and on return execution will be restored.
	.global interrupt_service_routine
	.ent interrupt_service_routine
interrupt_service_routine:
	.set noreorder
	.set noat

	#Registers $k0 and $k1 are reserved for HellFire
	#Save all temporary registers. Ramaining registers will be saved on TCB.
	#Slots 0($sp) through 12($sp) reserved for saving a0-a3
	addi  $sp, $sp, -104
	sw    $at, 16($sp)
	sw    $v0, 20($sp)
	sw    $v1, 24($sp)
	sw    $a0, 28($sp)
	sw    $a1, 32($sp)
	sw    $a2, 36($sp)
	sw    $a3, 40($sp)
	sw    $t0, 44($sp)
	sw    $t1, 48($sp)
	sw    $t2, 52($sp)
	sw    $t3, 56($sp)
	sw    $t4, 60($sp)
	sw    $t5, 64($sp)
	sw    $t6, 68($sp)
	sw    $t7, 72($sp)
	sw    $t8, 76($sp)
	sw    $t9, 80($sp)
	sw    $ra, 84($sp)
	mfc0  $k0, $14        #C0_EPC=14 (Exception PC)
	addi  $k0, $k0, -4    #Backup one opcode
	sw    $k0, 88($sp)
	mfhi  $k1
	sw    $k1, 92($sp)
	mflo  $k1
	sw    $k1, 96($sp)

	lui   $a1,  0x2000
	lw    $a0,  0x20($a1)   #IRQ_STATUS
	lw    $a2,  0x10($a1)   #IRQ_MASK

	jal	HF_InterruptServiceRoutine
	and   $a0,  $a0, $a2

	#Restore all temporary registers
	lw    $at, 16($sp)
	lw    $v0, 20($sp)
	lw    $v1, 24($sp)
	lw    $a0, 28($sp)
	lw    $a1, 32($sp)
	lw    $a2, 36($sp)
	lw    $a3, 40($sp)
	lw    $t0, 44($sp)
	lw    $t1, 48($sp)
	lw    $t2, 52($sp)
	lw    $t3, 56($sp)
	lw    $t4, 60($sp)
	lw    $t5, 64($sp)
	lw    $t6, 68($sp)
	lw    $t7, 72($sp)
	lw    $t8, 76($sp)
	lw    $t9, 80($sp)
	lw    $ra, 84($sp)
	lw    $k0, 88($sp)
	lw    $k1, 92($sp)
	mthi  $k1
	lw    $k1, 96($sp)
	mtlo  $k1
	addi  $sp, $sp, 104

isr_return:
	ori   $k1, $zero, 0x1    #re-enable interrupts
	jr    $k0
	mtc0  $k1, $12

.end interrupt_service_routine
	.set at


###################################################
	.global HF_AsmInterruptEnable
	.ent HF_AsmInterruptEnable
HF_AsmInterruptEnable:
	.set noreorder

	mfc0  $v0, $12
	mtc0  $a0, $12            #STATUS=1; enable interrupts
	jr    $ra
	mtc0  $a0, $12            #STATUS=1; enable interrupts

	.set reorder
.end HF_AsmInterruptEnable

###################################################
# configure ISR address
	.global  HF_AsmInterruptInit
	.ent    HF_AsmInterruptInit
HF_AsmInterruptInit:
	.set noreorder

	#Patch interrupt vector
	la    $a1, HF_AsmPatchValue
	lw    $a2, 0($a1)
	sw    $a2, 0x3c($zero)
	lw    $a2, 4($a1)
	sw    $a2, 0x40($zero)
	lw    $a2, 8($a1)
	sw    $a2, 0x44($zero)
	lw    $a2, 12($a1)
	sw    $a2, 0x48($zero)

	jr    $ra
	nop

HF_AsmPatchValue:
	la    $k0, interrupt_service_routine
	jr    $k0
	nop

	.set reorder
.end HF_AsmInterruptInit


###################################################
# saves task registers on a TCB entry
	.global   setjmp
	.ent     setjmp
setjmp:
	.set noreorder

	sw    $s0, 0($a0)
	sw    $s1, 4($a0)
	sw    $s2, 8($a0)
	sw    $s3, 12($a0)
	sw    $s4, 16($a0)
	sw    $s5, 20($a0)
	sw    $s6, 24($a0)
	sw    $s7, 28($a0)
	sw    $fp, 32($a0)
	sw    $gp, 36($a0)
	sw    $sp, 40($a0)
	sw    $ra, 44($a0)

	jr    $ra
	ori   $v0,  $zero, 0

	.set reorder
.end setjmp


###################################################
# restores task registers from a TCB entry
	.global   longjmp
	.ent     longjmp
longjmp:
	.set noreorder

	lw    $s0, 0($a0)
	lw    $s1, 4($a0)
	lw    $s2, 8($a0)
	lw    $s3, 12($a0)
	lw    $s4, 16($a0)
	lw    $s5, 20($a0)
	lw    $s6, 24($a0)
	lw    $s7, 28($a0)
	lw    $fp, 32($a0)
	lw    $gp, 36($a0)
	lw    $sp, 40($a0)
	lw    $ra, 44($a0)

	jr    $ra
	ori   $v0,  $a1, 0

	.set reorder
.end longjmp

###################################################
# restores task registers on a TCB entry, then enable interrupts.
	.global   HF_RestoreExecution
	.ent     HF_RestoreExecution
HF_RestoreExecution:
	.set noreorder

	lw    $s0, 0($a0)
	lw    $s1, 4($a0)
	lw    $s2, 8($a0)
	lw    $s3, 12($a0)
	lw    $s4, 16($a0)
	lw    $s5, 20($a0)
	lw    $s6, 24($a0)
	lw    $s7, 28($a0)
	lw    $fp, 32($a0)
	lw    $gp, 36($a0)
	lw    $sp, 40($a0)
	lw    $ra, 44($a0)

	ori   $k1, $zero, 0x1    # enable interrupts
	mtc0  $k1, $12

	jr    $ra
	ori   $v0,  $a1, 0

	.set reorder
.end HF_RestoreExecution

###################################################
# we don't have a halt instruction on MIPS, just do
# a nop and the idle task will loop

	.global   HF_Halt
	.ent     HF_Halt
HF_Halt:
	.set noreorder

	nop
	jr    $ra
	nop

	.set reorder
.end HF_Halt

