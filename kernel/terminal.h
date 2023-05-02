#pragma once
// 78x25 terminal
void terminal_init();
void terminal_print(char* text);

#define TERMINAL_BACKCOLOR VGA_COLOR_BLACK
#define TERMINAL_FRONTCOLOR VGA_COLOR_LIGHT_GREY