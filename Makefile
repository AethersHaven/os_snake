run: prepare_build build/os_image.img
	qemu-system-i386 -drive file=build/os_image.img,format=raw,index=0,if=floppy -m 32M

clean:
	rm -rf build

prepare_build:
	mkdir -p build

build/os_image.img: build/boot_sector.bin
	cat $^ > $@

build/boot_sector.bin: boot/boot_loader.asm
	nasm $< -f bin -o $@