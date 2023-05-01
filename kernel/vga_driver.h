#pragma once

#include "../crt/stdint.h"
#include "../crt/stdlib.h"

typedef enum {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
} VGA_COLOR;

#define SIZE sizeof(VGA_ATTRIBUTE)

inline static uint8_t color_attr(VGA_COLOR fc, VGA_COLOR bc){
    return ((uint8_t)fc << 4) | (uint8_t)bc;
}

inline static uint16_t vga_entry(char c, VGA_COLOR fc, VGA_COLOR bc){
    return (uint16_t)c << 8 | (uint16_t)color_attr(fc, bc);
}

#define VGA_WIDTH 80
#define VGA_HIEGHT 25
#define VGA_TEXT_MODE_BUFFER 0xB8000

extern uint8_t vga_row; // Y
extern uint8_t vga_coloumn; // X
extern uint16_t* vga_buffer;

void vga_set_cursor(int x, int y);
void vga_clear(VGA_COLOR color);
void vga_putat_c(char c, size_t x, size_t y, uint16_t color);
//void vga_putat(char c, size_t x, size_t y);