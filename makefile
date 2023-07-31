ASM=nasm
CC16=/usr/bin/watcom/binl/wcc
LD16=/usr/bin/watcom/binl/wlink
CFLAGS16=-4 -d3 -s -wx -ms -zl -zq -za99
all:  
	$(ASM) src/boot/boot.asm -f bin -o build/boot/boot.bin
	$(ASM) src/kernel/main.asm -f obj -o build/kernel/main_asm.o
	$(ASM) src/kernel/extend.asm -f obj -o build/kernel/extend.o
	$(ASM) src/kernel/io/io.asm -f obj -o build/kernel/io.o
	$(ASM) src/kernel/interrupts.asm -f obj -o build/kernel/interrupts.o
	$(CC16) $(CFLAGS16) -fobuild/kernel/main.o src/kernel/main.c
	$(CC16) $(CFLAGS16) -fobuild/kernel/io_c.o src/kernel/io/io.c
	$(CC16) $(CFLAGS16) -fobuild/kernel/devices.o src/kernel/devices.c
	$(CC16) $(CFLAGS16) -fobuild/kernel/malloc.o src/kernel/malloc.c
	$(CC16) $(CFLAGS16) -fobuild/kernel/sys.o src/kernel/sys.c
	$(CC16) $(CFLAGS16) -fobuild/kernel/interrupts_c.o src/kernel/interrupts.c
	$(CC16) $(CFLAGS16) -fobuild/kernel/printf.o src/kernel/printf.c
	$(CC16) $(CFLAGS16) -fobuild/kernel/fs.o src/kernel/fs.c
	$(LD16) NAME build/kernel/kernel.bin FILE \{build/kernel/fs.o build/kernel/printf.o build/kernel/malloc.o build/kernel/interrupts_c.o build/kernel/sys.o build/kernel/interrupts.o build/kernel/devices.o build/kernel/io_c.o build/kernel/main_asm.o build/kernel/extend.o build/kernel/main.o build/kernel/io.o \} OPTION MAP=build/kernel.map @linker.lnk
	cat build/boot/boot.bin build/kernel/kernel.bin > build/kernel.img 
	truncate -s 1M build/kernel.img
	make run

run:
	qemu-system-x86_64 -machine q35 -m 1G -cpu qemu64  -drive file=disk.img,if=floppy,index=1,format=raw -drive file=build/kernel.img,if=floppy,index=0,format=raw

clean: 
	rm -rf build/
	mkdir build
	mkdir build/boot/
	mkdir build/kernel/
	rm *.err

init: 
	mkdir build
	mkdir build/boot
	mkdir build/kernel
