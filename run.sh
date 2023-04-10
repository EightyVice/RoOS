# assemble the bootloader
nasm boot.asm -f bin -o boot.bin
nasm stage2.asm -f bin -o stage2.bin

# Compile the kernel
gcc kernel.c -Wall -nostdlib -nostartfiles -o kernel.o   # Compile
ld -T linker.ld -o kernel kernel.o                       # Link
objcopy -O binary kernel kernel.bin                      # Get binary image

# build a floppy disk image by appending sectors.
dd if=boot.bin of=disk.img conv=notrunc                  # Stage 1 Sector
dd if=stage2.bin of=disk.img bs=512 seek=1 conv=notrunc  # Stage 2 Sector
dd if=kernel.bin of=disk.img seek=2 conv=notrunc         # Kernel

# run qemu
qemu-system-i386 -hda disk.img