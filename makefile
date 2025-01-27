NeonOS.img: boot.bin makefile
	tool/copy/textcopy boot.bin NeonOS.img -o 0

boot.bin: boot.s makefile
	as -o boot.o $<
	ld -Ttext 0x7c00 -o boot.bin boot.o

create: 
	qemu-img create -f raw NeonOS.img 10M


run:
	qemu-system-x86_64 -drive file=NeonOS.img,format=raw

dbug:
	qemu-system-x86_64 -s -S -kernel NeonOS.img -append "nokaslr console=ttyS0" \
	-nographic

gdb:


clean:
	rm boot.o boot.bin
