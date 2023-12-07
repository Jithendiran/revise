#install qemu, nasm, bochs
ASM=nasm
src=src
dest=target
pkgmgr=apt install

$(dest)/boot.img: $(dest)/boot.bin
	cp $(dest)/boot.bin $(dest)/boot.img
	truncate -s 1440k $(dest)/boot.img


$(dest)/boot.bin: $(src)/boot/boot.s
	mkdir -p $(dest)
	$(ASM) $(src)/boot/boot.s -f bin -o $(dest)/boot.bin

clean:
	rm -r $(dest)

run: $(dest)/boot.img
	qemu-system-i386 $(dest)/boot.img

install:
	sudo $(pkgmgr) qemu-system-x86  nasm bochs -y 