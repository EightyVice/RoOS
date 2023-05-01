//#include "../crt/stdlib.h"

#include "vga_driver.h"



uint8_t vga_row = 0;
uint8_t vga_coloumn = 0;
uint16_t* vga_buffer = (uint16_t*)VGA_TEXT_MODE_BUFFER;

void vga_set_cursor(int x, int y){
    vga_coloumn = x;
    vga_row = y;
}


void vga_putat_c(char c, size_t x, size_t y, uint16_t color){
    size_t pos = x + y * VGA_WIDTH;
    vga_buffer[pos] = (uint16_t)c << 8 | color;
}

void vga_clear(VGA_COLOR color){
    vga_set_cursor(0, 0);
    
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        for (size_t y = 0; y < VGA_HIEGHT; y++)
        {
            vga_putat_c(' ', x, y, color_attr(VGA_COLOR_WHITE, color));
        }        
    }
}