kernel.bin: kernel.c
	gcc kernel.c -c -Wall -nostdlib -nostartfiles -ffreestanding -o kernel.o
	ld -T linker.ld -o kernel kernel.o -Map=kernel.map
	objcopy -O binary kernel kernel.bin

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.img
	rm -f *.map