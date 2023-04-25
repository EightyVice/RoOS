# assemble the bootloader
nasm boot.asm -f bin -o boot.bin
nasm stage2.asm -f bin -o stage2.bin

# Compile the kernel
i386_elf_gcc=./i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-gcc
i386_elf_ld=./i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-ld
i386_elf_objcopy=./i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-objcopy

$i386_elf_gcc kernel.c -c -Wall -nostdlib -nostartfiles -ffreestanding -o kernel.o   # Compile
$i386_elf_ld -T linker.ld -o kernel kernel.o -Map=kernel.map          # Link
$i386_elf_objcopy -O binary kernel kernel.bin                         # Get binary image

# build a floppy disk image by appending sectors.
dd if=boot.bin of=disk.img conv=notrunc                  # Stage 1 Sector
dd if=stage2.bin of=disk.img bs=512 seek=1 conv=notrunc  # Stage 2 Sector
dd if=kernel.bin of=disk.img seek=2 conv=notrunc         # Kernel

# run qemu
qemu-system-i386 -hda disk.img