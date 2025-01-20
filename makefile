boot.sys: boot.s
	as -o boot.o boot.s
	ld -Ttext 0x7c00 -o boot.sys boot.o

build: boot.sys
	tool/objcopy/objcopy boot.sys 3072 512 OS.img 0

run: boot.sys
	qemu-system-x86_64 -drive file=OS.img,format=raw

clean:
	rm boot.o
