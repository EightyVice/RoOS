BOOT_DIR := ./boot
KERNEL_DIR := ./kernel
OUT_DIR := ./out
CROSS_DIR := ./i386-elf-7.5.0-Linux-x86_64/bin

GCC = $(CROSS_DIR)/i386-elf-gcc
LD = $(CROSS_DIR)/i386-elf-ld
OBJCOPY = $(CROSS_DIR)/i386-elf-objcopy

.PHONY : run all bootloader clean

all: $(OUT_DIR)/disk.img

run: all
	qemu-system-i386 -hda $(OUT_DIR)/disk.img

bootloader: $(OUT_DIR)/boot.bin $(OUT_DIR)/stage2.bin
# Stage 1 Bootloader
$(OUT_DIR)/boot.bin:
	nasm $(BOOT_DIR)/boot.asm -f bin -o $(OUT_DIR)/boot.bin -i $(BOOT_DIR)

# Stage 2 Bootloader
$(OUT_DIR)/stage2.bin:
	nasm $(BOOT_DIR)/stage2.asm -f bin -o $(OUT_DIR)/stage2.bin -i $(BOOT_DIR)

$(OUT_DIR)/kernel_main.o:
	$(GCC) $(KERNEL_DIR)/kernel_main.c -c -Wall -nostdlib -nostartfiles -ffreestanding -o $(OUT_DIR)/kernel_main.o

# Linking kernel
$(OUT_DIR)/kernel.o: $(OUT_DIR)/kernel_main.o
	$(LD) $(OUT_DIR)/kernel_main.o -T $(KERNEL_DIR)/linker.ld -o $(OUT_DIR)/kernel.o 

# Convert kernel.o to flat binary
$(OUT_DIR)/kernel.bin: $(OUT_DIR)/kernel.o
	$(OBJCOPY) -O binary $(OUT_DIR)/kernel.o $(OUT_DIR)/kernel.bin

# Building the image
$(OUT_DIR)/disk.img: $(OUT_DIR)/boot.bin $(OUT_DIR)/stage2.bin $(OUT_DIR)/kernel.bin
	dd if=$(OUT_DIR)/boot.bin of=$(OUT_DIR)/disk.img conv=notrunc
	dd if=$(OUT_DIR)/stage2.bin of=$(OUT_DIR)/disk.img bs=512 seek=1 conv=notrunc
	dd if=$(OUT_DIR)/kernel.bin of=$(OUT_DIR)/disk.img seek=2 conv=notrunc


clean:
	rm -f $(OUT_DIR)/*.o
	rm -f $(OUT_DIR)/*.bin
	rm -f $(OUT_DIR)/*.img
	rm -f $(OUT_DIR)/*.map