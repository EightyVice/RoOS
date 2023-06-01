BOOT_DIR := ./boot
KERNEL_DIR := ./kernel
OUT_DIR := ./out
CROSS_DIR := ./i386-elf-7.5.0-Linux-x86_64/bin
CRT_DIR := ./crt

GCC = $(CROSS_DIR)/i386-elf-gcc
LD = $(CROSS_DIR)/i386-elf-ld
OBJCOPY = $(CROSS_DIR)/i386-elf-objcopy
OBJDUMP = $(CROSS_DIR)/i386-elf-objdump

SRCS := $(shell find $(CRT_DIR) $(KERNEL_DIR) -name '*.c')
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

# Compile the kernel
$(OUT_DIR)/%.o: $(CRT_DIR)/%.c
	$(GCC) $< -I $(CRT_DIR) -c -Wall -nostdlib -ffreestanding -o $@ -mno-sse -mno-sse2 -mgeneral-regs-only -funsigned-char

$(OUT_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(GCC) $< -I $(KERNEL_DIR) -c -Wall -nostdlib -ffreestanding -o $@ -mno-sse -mno-sse2 -mgeneral-regs-only -funsigned-char

# Linking kernel
$(OUT_DIR)/kernel.elf: $(OBJS) $(KERNEL_DIR)/linker.ld
	$(LD) -T $(KERNEL_DIR)/linker.ld $(OBJS)  -o $@ -Map=kernel.map

# Convert kernel.elf to flat binary
$(OUT_DIR)/kernel.bin: $(OUT_DIR)/kernel.elf
	$(OBJCOPY) -O binary $^ $@

# Building the image
$(OUT_DIR)/disk.img: $(OUT_DIR)/boot.bin $(OUT_DIR)/stage2.bin $(OUT_DIR)/kernel.bin
	dd if=$(OUT_DIR)/boot.bin of=$(OUT_DIR)/disk.img conv=notrunc
	dd if=$(OUT_DIR)/stage2.bin of=$(OUT_DIR)/disk.img bs=512 seek=1 conv=notrunc
	dd if=$(OUT_DIR)/kernel.bin of=$(OUT_DIR)/disk.img seek=2 conv=notrunc

disasm: $(OUT_DIR)/kernel.elf
	$(OBJDUMP) --disassemble-all $^ > kernel.s1

clean:
	rm -rf $(OUT_DIR)
	mkdir $(OUT_DIR)