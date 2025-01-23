NeonOS.img: boot.sys
	tool/copy/textcopy boot.sys NeonOS.img

boot.sys: boot.s
	as -o boot.o boot.s
	ld -Ttext 0x7c00 -o boot.sys boot.o

create: 
	qemu-img create -f raw NeonOS.img 1M

run:
	qemu-system-x86_64 -drive file=NeonOS.img,format=raw

clean:
	rm boot.o boot.sys
