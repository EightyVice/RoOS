# assemble the bootloader
nasm ./boot/boot.asm -f bin -o ./out/boot.bin -i ./boot/
nasm ./boot/stage2.asm -f bin -o ./out/stage2.bin -i ./boot/

# Compile the kernel
i386_elf_gcc=./i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-gcc
i386_elf_ld=./i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-ld
i386_elf_objcopy=./i386-elf-7.5.0-Linux-x86_64/bin/i386-elf-objcopy

$i386_elf_gcc ./kernel/vga_driver.c ./kernel/kernel_main.c -Wall -nostdlib -nostartfiles -ffreestanding -o ./out/kernel -T ./kernel/linker.ld # Compile
#$i386_elf_ld -T ./kernel/linker.ld -o ./out/kernel ./out/kernel.o -Map=./out/kernel.map          # Link
$i386_elf_objcopy -O binary ./out/kernel ./out/kernel.bin                         # Get binary image

# build the image by appending sectors.
dd if=./out/boot.bin of=./out/disk.img conv=notrunc                  # Stage 1 Sector
dd if=./out/stage2.bin of=./out/disk.img bs=512 seek=1 conv=notrunc  # Stage 2 Sector
dd if=./out/kernel.bin of=./out/disk.img seek=2 conv=notrunc         # Kernel

# run qemu
qemu-system-i386 -hda ./out/disk.img