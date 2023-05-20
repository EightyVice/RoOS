#include "../crt/string.h"

#include "vga_driver.h"
#include "terminal.h"

size_t terminal_x = 0;
size_t terminal_y = 0;
size_t start = 0;

#define TERMINAL_WIDTH 25
#define TERMINAL_HIGHT 390 // 78 * 5

uint16_t buffer[TERMINAL_WIDTH][TERMINAL_HIGHT];
#define BUFFER_SIZE sizeof(buffer);

void putat(char c, size_t x, size_t y){
        buffer[x][y] = (buffer[x][y] & 0xFF00) | c;
        if(terminal_y - start >= 0){
            // put at VGA
            //vga_putat(c, x, terminal_y - start + 1);
            vga_buffer[x + ((y + 1) * VGA_WIDTH)] = buffer[x][y];
        }
}
void terminal_print(char* text){
    for (size_t i = 0; text[i] != '\0'; i++)
    {
        if(text[i] == '\n'){
            terminal_x = 0;
            terminal_y++;
            continue;
        }

        putat(text[i], terminal_x, terminal_y);
        terminal_x++;
    }  
} 

void terminal_status_print(char* text){
    vga_puts(text, 0, 24);
}

void terminal_init(){ 
    for (size_t i = 0; i < sizeof(buffer) / sizeof(uint16_t); i++)
    {
        *((((uint16_t*)buffer) + i)) = vga_entry(' ', TERMINAL_FRONTCOLOR, TERMINAL_BACKCOLOR);
    }
    
}