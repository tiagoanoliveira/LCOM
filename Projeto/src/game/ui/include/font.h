#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <stdbool.h>

// Font drawing function
void draw_character(bool is_title, int x, int y, char c, uint32_t fg_color, uint32_t bg_color);

// Helper function to draw text strings
void draw_text(bool is_title, int x, int y, const char* text, uint32_t fg_color, uint32_t bg_color);

void draw_moldure(void);
void draw_decoration(void);

#endif // FONT_H
