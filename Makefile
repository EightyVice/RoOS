BOOT_DIR := ./boot
KERNEL_DIR := ./kernel
OUT_DIR := ./out
CROSS_DIR := ./i386-elf-7.5.0-Linux-x86_64/bin

GCC = $(CROSS_DIR)/i386-elf-gcc
LD = $(CROSS_DIR)/i386-elf-ld
OBJCOPY = $(CROSS_DIR)/i386-elf-objcopy
OBJDUMP = $(CROSS_DIR)/i386-elf-objdump

SRCS := $(shell find $(KERNEL_DIR) -name '*.c')
OBJSLIST := $(SRCS:.c=.o)
OBJS := $(foreach obj, $(OBJSLIST), $(OUT_DIR)/$(shell basename $(obj))) 

all: $(OUT_DIR)/disk.img


.PHONY : run all bootloader clean kernel


run: all
	qemu-system-i386 -hda $(OUT_DIR)/disk.img

bootloader: $(OUT_DIR)/boot.bin $(OUT_DIR)/stage2.bin
# Stage 1 Bootloader
$(OUT_DIR)/boot.bin: $(BOOT_DIR)/boot.asm
	nasm $(BOOT_DIR)/boot.asm -f bin -o $(OUT_DIR)/boot.bin -i $(BOOT_DIR)

# Stage 2 Bootloader
$(OUT_DIR)/stage2.bin: $(BOOT_DIR)/stage2.asm
	nasm $(BOOT_DIR)/stage2.asm -f bin -o $(OUT_DIR)/stage2.bin -i $(BOOT_DIR)

# Linking kernel
$(OUT_DIR)/kernel.elf: $(OBJS) $(KERNEL_DIR)/linker.ld
	echo $(OBJS)
	$(LD) -T $(KERNEL_DIR)/linker.ld $(OBJS)  -o $@

disasm: $(OUT_DIR)/kernel.elf
	$(OBJDUMP) --disassemble-all $^ > kernel.s1

# Convert kernel.o to flat binary
$(OUT_DIR)/kernel.bin: $(OUT_DIR)/kernel.elf
	$(OBJCOPY) -O binary $^ $@

$(OUT_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(GCC) $^ -I $(KERNEL_DIR) -c -Wall -nostdlib -ffreestanding -o $@ -mno-sse -mno-sse2


# Building the image
$(OUT_DIR)/disk.img: $(OUT_DIR)/boot.bin $(OUT_DIR)/stage2.bin $(OUT_DIR)/kernel.bin
	dd if=$(OUT_DIR)/boot.bin of=$(OUT_DIR)/disk.img conv=notrunc
	dd if=$(OUT_DIR)/stage2.bin of=$(OUT_DIR)/disk.img bs=512 seek=1 conv=notrunc
	dd if=$(OUT_DIR)/kernel.bin of=$(OUT_DIR)/disk.img seek=2 conv=notrunc


clean:
	rm -rf $(OUT_DIR)
	mkdir $(OUT_DIR)