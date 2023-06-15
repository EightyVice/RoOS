#include "kernel.h"
#include "vga_driver.h"
#include "terminal.h"

#include "pic_driver.h"
#include "idt.h"

#include "../crt/stdio.h"

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

uint32_t kernel_tick = 0;

__attribute__((section(".entry")))
void kernel_entry()  {
    idt_init(); 
    pic_remap_vectors(0x20, 0x28);

    screen_init();

    terminal_init();
    
    char hello[50];
    sprintf(hello, "Welcome to %s Mr %s", "RoOS!", "Zeyad");

    terminal_println(hello);
    terminal_println("This is a very long text to be written and it should have the words to be wrapped, and It's very simple to be done.");
    while(1); // Loop forever to prevent death!!
}

