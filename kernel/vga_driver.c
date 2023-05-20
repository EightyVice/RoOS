//#include "../crt/stdlib.h"

#include "vga_driver.h"
#include "hal.h"


uint8_t vga_row = 0;
uint8_t vga_coloumn = 0;
uint16_t* vga_buffer = (uint16_t*)VGA_TEXT_MODE_BUFFER;

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vga_set_cursor(int x, int y){
    vga_coloumn = x;
    vga_row = y; 
}


void vga_putat_c(char c, size_t x, size_t y, uint8_t color){
    size_t pos = x + y * VGA_WIDTH;
    vga_buffer[pos] = (uint16_t)color << 8 | c;
}

void vga_putat_c_cnt(char c, size_t x, size_t y, uint8_t color, size_t count){
    for (size_t i = 0; i < count; i++)
        vga_putat_c(c, x + i, y, color);  
}

void vga_putat(char c, size_t x, size_t y){
    size_t pos = x + y * VGA_WIDTH;
    vga_buffer[pos] = (vga_buffer[pos] & 0xFF00) | c;
}

void vga_puts(char* str, size_t x, size_t y){
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        vga_putat(str[i], x + i, y);
    }
}
void vga_puts_c(char* str, size_t x, size_t y, uint8_t color){
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        vga_putat_c(str[i], x + i, y, color);
    }
}
void vga_putcolor(VGA_COLOR fore, VGA_COLOR back, size_t x, size_t y){
    if(back == 0xFF && fore == 0xFF)
        return;

    size_t pos = x + y * VGA_WIDTH;
    if(back == 0xFF)        // set the foreground color only
        vga_buffer[pos] = (vga_buffer[pos] & 0xF0FF) | fore << 8;
    else if(fore == 0xFF)   // set the background color only
        vga_buffer[pos] = (vga_buffer[pos] & 0x0FFF) | back << 12;
    else                    // set both colors
        vga_buffer[pos] = (vga_buffer[pos] & 0x00FF) | (fore << 8) | (back << 12);
}

void vga_clear(VGA_COLOR color){
    vga_set_cursor(0, 0);
    
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        for (size_t y = 0; y < VGA_HIEGHT; y++)
        {
            vga_putat_c(' ', x, y, COLOR_ATTR(VGA_COLOR_BLACK , color));
        }        
    }
}

char vga_getchar(size_t x, size_t y){
    return vga_buffer[x + y * VGA_WIDTH];
}
