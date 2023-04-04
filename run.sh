# assemble the bootloader
nasm boot.asm -f bin -o boot.bin
nasm stage2.asm -f bin -o stage2.bin

# build a floppy disk image by appending sectors.
dd if=boot.bin of=disk.img conv=notrunc                  # Stage 1 Sector
dd if=stage2.bin of=disk.img bs=512 seek=1 conv=notrunc  # Stage 2 Sector

# run qemu
qemu-system-i386 -fda disk.img