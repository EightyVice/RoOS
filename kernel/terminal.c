#include "../crt/string.h"

#include "vga_driver.h"
#include "terminal.h"

size_t terminal_x = 0;
size_t terminal_y = 0;
size_t start = 0;

#define TERMINAL_WIDTH 79 // 80 - 1 for scrollbar
#define TERMINAL_HIGHT 230 // (25 - 2) * 10 Pages

uint16_t buffer[TERMINAL_WIDTH][TERMINAL_HIGHT];
#define BUFFER_SIZE sizeof(buffer);

void putat(char c, size_t x, size_t y){
        buffer[x][y] = (buffer[x][y] & 0xFF00) | c;
        if(terminal_y - start >= 0){
            // put at VGA
            //vga_putat(c, x, terminal_y - start + 1);

            if(terminal_y - start < VGA_HIEGHT - 2)
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
        
        if(i % TERMINAL_WIDTH == 0 && i != 0){
            terminal_x = 0;
            terminal_y++;
        }

        putat(text[i], terminal_x, terminal_y);
        terminal_x++;
    }  
} 

void terminal_println(char* text){
    terminal_print(text);
    terminal_x = 0;
    terminal_y++;
}
void terminal_status_print(char* text){
    vga_puts(text, 0, 24);
}

void terminal_init(){ 
    for (size_t i = 0; i < sizeof(buffer) / sizeof(uint16_t); i++)
    {
        *((((uint16_t*)buffer) + i)) = vga_entry(' ', TERMINAL_FRONTCOLOR, TERMINAL_BACKCOLOR);
    }

    // Draw scrollbar
    vga_putat_c(30, 79, 1, COLOR_ATTR(VGA_COLOR_LIGHT_MAGENTA, TERMINAL_BACKCOLOR));
    vga_putat_c(31, 79, 23, COLOR_ATTR(VGA_COLOR_LIGHT_MAGENTA, TERMINAL_BACKCOLOR));

    for (size_t i = 2; i < VGA_HIEGHT - 2; i++)
    {
        vga_putat_c(179, 79, i, COLOR_ATTR(VGA_COLOR_LIGHT_MAGENTA, TERMINAL_BACKCOLOR));
    }
    
    
}