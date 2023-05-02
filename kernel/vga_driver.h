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

#define COLOR_ATTR(fore, back) (((uint8_t)back << 4) | (uint8_t)fore)


inline static uint16_t vga_entry(char c, VGA_COLOR fc, VGA_COLOR bc){
    return ((uint16_t)(COLOR_ATTR(fc, bc))) << 8 | c;
}

#define VGA_WIDTH 80
#define VGA_HIEGHT 25
#define VGA_TEXT_MODE_BUFFER 0xB8000

extern uint8_t vga_row; // Y
extern uint8_t vga_coloumn; // X
extern uint16_t* vga_buffer;

void vga_set_cursor(int x, int y);
void vga_clear(VGA_COLOR color);
void vga_putat_c(char c, size_t x, size_t y, uint8_t color);
void vga_putat_c_cnt(char c, size_t x, size_t y, uint8_t color, size_t count);
void vga_putat(char c, size_t x, size_t y);
void vga_puts(char* str, size_t x, size_t y);
void vga_puts(char* str, size_t x, size_t y);
void vga_puts_c(char* str, size_t x, size_t y, uint8_t color);
void vga_putcolor(VGA_COLOR fore, VGA_COLOR back, size_t x, size_t y);
char vga_getchar(size_t x, size_t y);