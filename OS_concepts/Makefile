#install qemu, nasm, bochs
ASM=nasm
src=src
dest=target
pkgmgr=apt install
sample=sample

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

debug: bochs_config $(dest)/boot.img
	bochs -f ./bochs_config

debug-sample: bochs_config $(sample)/floppy.img
	bochs -f ./sample/bochs_config

$(sample)/floppy.img:# make ARGS="sample/pgm.s"
	$(ASM) $(ARGS) -f bin -o $(sample)/floppy.img

install:
	sudo $(pkgmgr) qemu-system-x86  nasm bochs bochs-sdl bochsbios vgabios -y 


# sudo apt-get install libsdl2-dev
# ./configure --enable-debugger --enable-debugger-gui --with-sdl2 --enable-x86-debugger 