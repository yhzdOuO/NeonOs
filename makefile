build: boot.sys NeonOs.img
	tool/copy/textcopy boot.sys NeonOs.img

boot.sys: boot.s
	as -o boot.o boot.s
	ld -Ttext 0x7c00 -o boot.sys boot.o

NeonOs.img:
	qemu-img create -f raw NeonOs.img 16M

run: NeonOs.img
	qemu-system-x86_64 -drive file=NeonOs.img,format=raw

clean:
	rm boot.o
