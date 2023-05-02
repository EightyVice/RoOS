#include "vga_driver.h"
#include "terminal.h"

void screen_init(){
	vga_clear(TERMINAL_BACKCOLOR);

    vga_putat_c_cnt(' ', 0, 0, COLOR_ATTR(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_CYAN), VGA_WIDTH);
    vga_putat_c_cnt(' ', 0, VGA_HIEGHT - 1, COLOR_ATTR(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_CYAN), VGA_WIDTH);
    vga_puts_c(" RoOS ", 0, 0, COLOR_ATTR(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BROWN));
}

/*
    Kernel Entry Point
    ------------------
    this function must be exported with section .entry
*/
__attribute__((section(".entry")))
void kernel_entry()  {
    screen_init();

    terminal_init();
    terminal_print("Welcome^_^\nTest");

    while(1); // Loop forever to prevent death!!
}

