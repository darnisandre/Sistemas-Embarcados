#ARCHITECTURE = mips/hf_risc
ARCHITECTURE = mips/plasma
#ARCHITECTURE = x86

# hf-risc compiler config and flags

CFLAGS_FEW_REGS = -ffixed-t0 -ffixed-t1 -ffixed-t2 -ffixed-t3 -ffixed-t4 -ffixed-t5 -ffixed-t6 -ffixed-t7 -ffixed-s0 -ffixed-s1 -ffixed-s2 -ffixed-s3 -ffixed-s4 -ffixed-s5 -ffixed-s6 -ffixed-s7
CFLAGS_FEW_FEW_REGS = -ffixed-t8 -ffixed-t9 -ffixed-a0 -ffixed-a1 -ffixed-v0 -ffixed-v1 -ffixed-fp -ffixed-gp
CFLAGS_NO_HW_MULDIV = -mnohwmult -mnohwdiv -ffixed-lo -ffixed-hi
CFLAGS_NO_HW_DIV = -mnohwdiv
# recycle $gp as a temporary register, do not put instructions on the BDS
CFLAGS_OTHER = -fcall-used-gp -fno-delayed-branch
# more aggressive optimizations, also turns -frename-registers on by default.
CFLAGS_XTREME = -flto -finline-limit-500 -funroll-all-loops #-funroll-loops

#remove unreferenced functions
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections

GCC_MIPS_hf_risc = mips-elf-gcc -O2 -c -mips1 -mpatfree -mno-check-zero-division -msoft-float -fshort-double -nostdinc -fno-builtin -fomit-frame-pointer -fno-delayed-branch -G 0 -I ./ukernel -I ./hflibc -I ./ukernel/hal/$(ARCHITECTURE) -DCPU_SPEED=25000000 -DBIG_ENDIAN $(CFLAGS_STRIP) $(CFLAGS_NO_HW_MULDIV) -DDEBUG_PORT #-fPIC -mabicalls #$(CFLAGS_FEW_REGS) -DDEBUG_PORT -DMMAP_MUL

AS_MIPS_hf_risc = mips-elf-as -mips1
LD_MIPS_hf_risc = mips-elf-ld -mips1 $(LDFLAGS_STRIP)
DUMP_MIPS_hf_risc = mips-elf-objdump
READ_MIPS_hf_risc = mips-elf-readelf
OBJ_MIPS_hf_risc = mips-elf-objcopy

# plasma compiler config and flags
GCC_MIPS = mips-elf-gcc -mips1 -mpatfree -Os -c -msoft-float -nostdinc -fno-builtin -I ./ukernel -I ./hflibc -I ./ukernel/hal/$(ARCHITECTURE) -fomit-frame-pointer -G 0 -DBIG_ENDIAN #-fno-delayed-branch -mno-check-zero-division
AS_MIPS = mips-elf-as
LD_MIPS = mips-elf-ld
DUMP_MIPS = mips-elf-objdump
READ_MIPS = mips-elf-readelf
OBJ_MIPS = mips-elf-objcopy

# x86 compiler config and flags
GCC_X86 = gcc -m32 -O0 -c -nostdinc -fno-builtin -I ./ukernel -I ./hflibc -I ./ukernel/hal/$(ARCHITECTURE) -DLITTLE_ENDIAN
NASM_X86 = nasm -f aout
AS_X86 = as --32
LD_X86 = ld -melf_i386
DUMP_X86 = objdump
READ_X86 = readelf
OBJ_X86 = objcopy
ISO_X86 = genisoimage #mkisofs

#OS configuration (plasma)
OS_CONFIG = -DDEBUG -DCPU_ID=0 -DMAX_TASKS=20 -DHEAP_SIZE=32768 -DSTACK_MAGIC=0xa5a5a5a5 -DCPU_SPEED=25000000 -DTICK_TIME=18 -DN_CORES=6 -DFAST_STDOUT #-DKERNEL_TASK_LOG 

OS_CONFIG_HUGE = -DDEBUG -DCPU_ID=0 -DMAX_TASKS=50 -DHEAP_SIZE=524288 -DSTACK_MAGIC=0xa5a5a5a5 -DCPU_SPEED=25000000 -DTICK_TIME=18 -DN_CORES=6 -DFAST_STDOUT

COREX_OS_CONFIG = -DDEBUG -DMAX_TASKS=20 -DHEAP_SIZE=524288 -DSTACK_MAGIC=0xa5a5a5a5 -DCPU_SPEED=25000000 -DTICK_TIME=18 -DN_CORES=6 -DNOC_INTERCONNECT -DNOC_WIDTH=3 -DNOC_HEIGHT=2 -DPACKET_SIZE=64 -DFAST_STDOUT #-DCOUNTER_REG=COUNTER

MANY_CORE_LIST1 = 0 1 2 3

MANY_CORE_LIST2 = 0 1 2 3 4 5

MANY_CORE_LIST3 = 0 1 2 3 4 5 6 7 8

#OS configuration (x86)
X86_OS_CONFIG = -DDEBUG -DCPU_ID=0 -DMAX_TASKS=30 -DHEAP_SIZE=67108864 -DSTACK_MAGIC=0xa5a5a5a5 -DCPU_SPEED=2800000000 -DTICK_TIME=100 -DN_CORES=1
# to make a usb boot drive, type dd if=hellfire_x86.img of=/dev/sdb
# after reboot, type on the grub menu:
# kernel 200+500
# boot
#

all:

test1:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o dummy_test.o ./app/test1.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o dummy_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

test2:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o dummy_test.o ./app/test2.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o dummy_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

test3:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o dummy_test.o ./app/test3.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o dummy_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

dummy_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o dummy_test.o ./app/dummy_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o dummy_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

preemption_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o preemption_test.o ./app/preemption_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o preemption_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

preemption_test_hf_risc:
	$(AS_MIPS_hf_risc) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS_hf_risc) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS_hf_risc) -o preemption_test.o ./app/preemption_test.c $(OS_CONFIG)
	$(LD_MIPS_hf_risc) -T./ukernel/hal/$(ARCHITECTURE)/hf_risc.ld -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o preemption_test.o
	$(DUMP_MIPS_hf_risc) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS_hf_risc) -h test.axf > test.sec
	$(DUMP_MIPS_hf_risc) -s test.axf > test.cnt
	$(OBJ_MIPS_hf_risc) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code.txt

preemption_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o preemption_test.o ./app/preemption_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o preemption_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

mutex_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o mutex_test.o ./app/mutex_test.c $(OS_CONFIG)
	$(LD_MIPS)  -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o mutex_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

mutex_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o mutex_test.o ./app/mutex_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mutex_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

sem_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o sem_test.o ./app/sem_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o sem_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

sem_test_hf_risc:
	$(AS_MIPS_hf_risc) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS_hf_risc) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS_hf_risc) -o sem_test.o ./app/sem_test.c $(OS_CONFIG)
	$(LD_MIPS_hf_risc) -T./ukernel/hal/$(ARCHITECTURE)/hf_risc.ld -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o sem_test.o
	$(DUMP_MIPS_hf_risc) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS_hf_risc) -h test.axf > test.sec
	$(DUMP_MIPS_hf_risc) -s test.axf > test.cnt
	$(OBJ_MIPS_hf_risc) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code.txt

sem_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o sem_test.o ./app/sem_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o sem_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

phi_test_hf_risc:
	$(AS_MIPS_hf_risc) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS_hf_risc) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS_hf_risc) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS_hf_risc) -o philosophers.o ./app/philosophers.c $(OS_CONFIG)
	$(LD_MIPS_hf_risc) -T./ukernel/hal/$(ARCHITECTURE)/hf_risc.ld -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o philosophers.o
	$(DUMP_MIPS_hf_risc) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS_hf_risc) -h test.axf > test.sec
	$(DUMP_MIPS_hf_risc) -s test.axf > test.cnt
	$(OBJ_MIPS_hf_risc) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code.txt

fork_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o fork_test.o ./app/fork_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o fork_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

fork_test_rt:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o fork_test_rt.o ./app/fork_test_rt.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o fork_test_rt.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

fork_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o fork_test.o ./app/fork_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o fork_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

oneshot_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o oneshot.o ./app/oneshot.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o oneshot.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

sleep_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o sleep_test.o ./app/sleep_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o sleep_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

sleep_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o sleep_test.o ./app/sleep_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o sleep_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

malloc_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o malloc_test.o ./app/malloc_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o malloc_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

malloc_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o malloc_test.o ./app/malloc_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o malloc_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

mailbox_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o mailbox_test.o ./app/mailbox_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o mailbox_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

mailbox_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o mailbox_test.o ./app/mailbox_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mailbox_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

message_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG)

	$(GCC_MIPS) -o message_test.o ./app/message_test.c $(OS_CONFIG)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o message_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

message_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o message_test.o ./app/message_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o message_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

vopd_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG_HUGE)

	$(GCC_MIPS) -o vopd_test.o ./app/vopd_test.c $(OS_CONFIG_HUGE)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o vopd_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

vopd_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o vopd_test.o ./app/vopd_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o vopd_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

mpeg4_test:
	$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(OS_CONFIG_HUGE)
	$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(OS_CONFIG_HUGE)

	$(GCC_MIPS) -o mpeg4_test.o ./app/mpeg4_test.c $(OS_CONFIG_HUGE)
	$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
		boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o mpeg4_test.o
	$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst
	$(DUMP_MIPS) -h test.axf > test.sec
	$(DUMP_MIPS) -s test.axf > test.cnt
	$(OBJ_MIPS) -O binary test.axf test.bin
	mv test.axf code0.axf
	mv test.bin code0.bin
	mv test.lst code0.lst
	mv test.sec code0.sec
	mv test.cnt code0.cnt
	mv test.map code0.map
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt

mpeg4_test_x86:
	$(NASM_X86) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s
	$(AS_X86) -o boot32.o ./ukernel/hal/$(ARCHITECTURE)/boot32.s
	$(GCC_X86) -o gdt_idt.o ./ukernel/hal/$(ARCHITECTURE)/gdt_idt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(X86_OS_CONFIG)
	$(GCC_X86) -o drivers.o ./ukernel/hal/$(ARCHITECTURE)/drivers.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(X86_OS_CONFIG)
	$(GCC_X86) -o mutex.o ./ukernel/mutex.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ipc.o ./ukernel/ipc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o hflibc.o ./hflibc/hflibc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o malloc.o ./hflibc/malloc.c $(X86_OS_CONFIG)
	$(GCC_X86) -o ukernel.o ./ukernel/ukernel.c $(X86_OS_CONFIG)

	$(GCC_X86) -o mpeg4_test.o ./app/mpeg4_test.c $(X86_OS_CONFIG)
	$(LD_X86)  -Ttext 0x00100000 -e_entry -Map test.map -N -o hellfire_x86.bin \
		boot.o boot32.o gdt_idt.o interrupt.o drivers.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpeg4_test.o
	$(DUMP_X86) --disassemble --reloc hellfire_x86.bin > hellfire_x86.lst
# make a floppy image
	dd if=/dev/zero of=pad bs=1 count=750
	cat ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage1 ukernel/hal/$(ARCHITECTURE)/iso/boot/grub/stage2 pad hellfire_x86.bin > hellfire_x86.img
# make an ISO image
	cp hellfire_x86.bin ukernel/hal/$(ARCHITECTURE)/iso/boot/
	$(ISO_X86) -R -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -o hellfire_x86.iso ukernel/hal/$(ARCHITECTURE)/iso
	rm ukernel/hal/$(ARCHITECTURE)/iso/boot/hellfire_x86.bin

filter:
	for i in $(MANY_CORE_LIST3) ; do \
		echo "Building image for core $$i.."; \
		$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s && \
		$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(COREX_OS_CONFIG) && \
		$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o filter$$i.o ./app/tp2_files/filter.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
			boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o filter$$i.o && \
		$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst && \
		$(DUMP_MIPS) -h test.axf > test.sec && \
		$(DUMP_MIPS) -s test.axf > test.cnt && \
		$(OBJ_MIPS) -O binary test.axf test.bin && \
		mv test.axf code$$i.axf && \
		mv test.bin code$$i.bin && \
		mv test.lst code$$i.lst && \
		mv test.sec code$$i.sec && \
		mv test.cnt code$$i.cnt && \
		mv test.map code$$i.map && \
		hexdump -v -e '4/1 "%02x" "\n"' code$$i.bin > code$$i.txt \
	;done

noc_test2:
	for i in $(MANY_CORE_LIST2) ; do \
		echo "Building image for core $$i.."; \
		$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s && \
		$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(COREX_OS_CONFIG) && \
		$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o noc_test2$$i.o ./app/noc_test2.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
			boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o noc_test2$$i.o && \
		$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst && \
		$(DUMP_MIPS) -h test.axf > test.sec && \
		$(DUMP_MIPS) -s test.axf > test.cnt && \
		$(OBJ_MIPS) -O binary test.axf test.bin && \
		mv test.axf code$$i.axf && \
		mv test.bin code$$i.bin && \
		mv test.lst code$$i.lst && \
		mv test.sec code$$i.sec && \
		mv test.cnt code$$i.cnt && \
		mv test.map code$$i.map && \
		hexdump -v -e '4/1 "%02x" "\n"' code$$i.bin > code$$i.txt \
	;done

noc_test3:
	for i in $(MANY_CORE_LIST2) ; do \
		echo "Building image for core $$i.."; \
		$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s && \
		$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(COREX_OS_CONFIG) && \
		$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o noc_test3$$i.o ./app/noc_test3.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
			boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o noc_test3$$i.o && \
		$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst && \
		$(DUMP_MIPS) -h test.axf > test.sec && \
		$(DUMP_MIPS) -s test.axf > test.cnt && \
		$(OBJ_MIPS) -O binary test.axf test.bin && \
		mv test.axf code$$i.axf && \
		mv test.bin code$$i.bin && \
		mv test.lst code$$i.lst && \
		mv test.sec code$$i.sec && \
		mv test.cnt code$$i.cnt && \
		mv test.map code$$i.map && \
		hexdump -v -e '4/1 "%02x" "\n"' code$$i.bin > code$$i.txt \
	;done

noc_test4:
	for i in $(MANY_CORE_LIST2) ; do \
		echo "Building image for core $$i.."; \
		$(AS_MIPS) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s && \
		$(GCC_MIPS) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(COREX_OS_CONFIG) && \
		$(GCC_MIPS) -o mutex.o ./ukernel/mutex.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ipc.o ./ukernel/ipc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o hflibc.o ./hflibc/hflibc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o malloc.o ./hflibc/malloc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o ukernel.o ./ukernel/ukernel.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS) -o noc_test4$$i.o ./app/noc_test4.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(LD_MIPS) -Ttext 0x10000000 -e_entry -Map test.map -N -o test.axf \
			boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o noc_test4$$i.o && \
		$(DUMP_MIPS) --disassemble --reloc test.axf > test.lst && \
		$(DUMP_MIPS) -h test.axf > test.sec && \
		$(DUMP_MIPS) -s test.axf > test.cnt && \
		$(OBJ_MIPS) -O binary test.axf test.bin && \
		mv test.axf code$$i.axf && \
		mv test.bin code$$i.bin && \
		mv test.lst code$$i.lst && \
		mv test.sec code$$i.sec && \
		mv test.cnt code$$i.cnt && \
		mv test.map code$$i.map && \
		hexdump -v -e '4/1 "%02x" "\n"' code$$i.bin > code$$i.txt \
	;done

noc_test_hf_risc:
	for i in $(MANY_CORE_LIST2) ; do \
		echo "Building image for core $$i.."; \
		$(AS_MIPS_hf_risc) -o boot.o ./ukernel/hal/$(ARCHITECTURE)/boot.s && \
		$(GCC_MIPS_hf_risc) -o interrupt.o ./ukernel/hal/$(ARCHITECTURE)/interrupt.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS_hf_risc) -o hal.o ./ukernel/hal/$(ARCHITECTURE)/hal.c $(COREX_OS_CONFIG) && \
		$(GCC_MIPS_hf_risc) -o mutex.o ./ukernel/mutex.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS_hf_risc) -o ipc.o ./ukernel/ipc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS_hf_risc) -o hflibc.o ./hflibc/hflibc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS_hf_risc) -o malloc.o ./hflibc/malloc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS_hf_risc) -o ukernel.o ./ukernel/ukernel.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS_hf_risc) -o mpsoc.o ./ukernel/hal/$(ARCHITECTURE)/mpsoc.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(GCC_MIPS_hf_risc) -o noc_test$$i.o ./app/noc_test.c $(COREX_OS_CONFIG) -DCPU_ID=$$i && \
		$(LD_MIPS_hf_risc) -T./ukernel/hal/$(ARCHITECTURE)/hf_risc.ld -Map test.map -N -o test.axf \
			boot.o interrupt.o hal.o mutex.o ipc.o hflibc.o malloc.o ukernel.o mpsoc.o noc_test$$i.o && \
		$(DUMP_MIPS_hf_risc) --disassemble --reloc test.axf > test.lst && \
		$(DUMP_MIPS_hf_risc) -h test.axf > test.sec && \
		$(DUMP_MIPS_hf_risc) -s test.axf > test.cnt && \
		$(OBJ_MIPS_hf_risc) -O binary test.axf test.bin && \
		mv test.axf code$$i.axf && \
		mv test.bin code$$i.bin && \
		mv test.lst code$$i.lst && \
		mv test.sec code$$i.sec && \
		mv test.cnt code$$i.cnt && \
		mv test.map code$$i.map && \
		hexdump -v -e '4/1 "%02x" "\n"' code$$i.bin > code$$i.txt \
	;done

clean:
	-rm -rf ./ukernel/*~
	-rm -rf ./ukernel/hal/$(ARCHITECTURE)/*~
	-rm -rf ./tools/*~
	-rm -rf ./app/*~
	-rm -rf ./*~
	-rm -rf *.o *.obj *.exe *.axf *.map *.lst *.sec *.cnt *.hex *.txt *.bin *.iso *.vhd *~ *.img pad

