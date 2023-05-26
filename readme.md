# My OS Implementation (Not named yet)
I'm doing a little OS for teaching myself this black magic
## Goal
I'm trying to write the possible minimal OS for me, so I will be writing stuff from scratch (that's why I'm not going to use GRUB even, imagine the pain, don't hate me!) 

I aim to implement the basic principles of virtualization of CPU and memory, So no fancy stuff? well maybe a very tiny size? ;)

## Architecture
The OS targets Legacy BIOS, not UEFI so it's MBR (Master Boot Record). I'm not using any file systems, the code is directly on disk sectors.

### Structure of the image (disk.img)
* Bootloader (two stages because of the 512 limit per sector)
    * Stage 1 bootloader *on track sector 1*
    * Stage 2 bootloader *on track sector 2*
* Kernel  *on the rest of the image sectors*
    * Drivers
        * 8592 PIC
        * VGA
    * Interfaces & Usermode
        * Terminal Interface


## Development Environent
I'm working on Windows, WSL and using NASM as assembler, QEMU for testing as a VM. A i386 GCC cross-compiler is used for developing the kernel. few utilities used like `dd` for building the floppy disk image.
**Install the following packages:**

`sudo apt-get make gcc nasm qemu-system`


## Resources that I learn from
* https://wiki.osdev.org/ 
* [Intel® 64 and IA-32 Architectures Software Developer’s Manual Volume 3A: System Programming Guide, Part 1](https://www.intel.com/content/www/us/en/architecture-and-technology/64-ia-32-architectures-software-developer-vol-3a-part-1-manual.html)
* https://github.com/pritamzope/OS
* http://www.brokenthorn.com/Resources/OSDevIndex.html
* I already learnt assembly before so only I use NASM documentation
