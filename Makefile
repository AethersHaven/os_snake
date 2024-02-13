C_SOURCES = $(shell find kernel/ -type f -name '*.c')
HEADERS = $(shell find kernel/ -type f -name '*.h')
C_OBJECTS = $(patsubst %.c,build/%.o,$(C_SOURCES))

LD = /usr/local/i386elfgcc/bin/i386-elf-ld
GCC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = gdb
GCCFLAGS = -g -Icpu -ffreestanding -c -m32
GDBFLAGS = -tui -ex "symbol-file build/kernel.elf" -ex "break main" -ex "target remote localhost:1234" -ex "continue"

run: prepare_build build/os_image.bin
	qemu-system-i386 -drive file=build/os_image.bin,format=raw,index=0,if=floppy -m 32M

debug: clean prepare_build build/os_image.bin build/kernel.elf
	qemu-system-i386 -s -S -drive file=build/os_image.bin,format=raw,index=0,if=floppy -m 32M &
	${GDB} ${GDBFLAGS}

clean:
	rm -rf build

prepare_build:
	mkdir -p build

build/os_image.bin: build/boot_sector.bin build/kernel.bin
	cat $^ > $@

build/boot_sector.bin: boot/boot_loader.asm
	nasm $< -f bin -o $@

build/kernel.bin: build/boot/kernel_entry.o $(C_OBJECTS) build/kernel/core/cpu/interrupt.o
	${LD} -melf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

build/kernel.elf: build/boot/kernel_entry.o ${C_OBJECTS} build/kernel/core/cpu/interrupt.o
	${LD} -melf_i386 -o $@ -Ttext 0x1000 $^

build/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	${GCC} ${GCCFLAGS}  $< -o $@

build/%.o: %.asm
	@mkdir -p $(dir $@)
	nasm -f elf32 $< -f elf -o $@