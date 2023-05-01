#include "vga_driver.h"

//#ifdef __GNUC__
__attribute__((section(".entry")))
//#endif
void kernel_entry()  {
	vga_clear(VGA_COLOR_RED);
    while(1){
        asm volatile("cli");
        asm volatile("hlt");
    } // Loop forever to prevent death!!
}

