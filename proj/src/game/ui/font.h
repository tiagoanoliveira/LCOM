#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include "../../drivers/graphics/graphics.h"
#include <ctype.h>

// Font dimensions
#define CHAR_WIDTH 16
#define CHAR_HEIGHT 24

// Title font dimensions
#define TITLE_SCALE 3
#define TITLE_CHAR_WIDTH  (CHAR_WIDTH * TITLE_SCALE)
#define TITLE_CHAR_HEIGHT (CHAR_HEIGHT * TITLE_SCALE)git

// Font drawing function
void draw_character(int x, int y, char c, uint32_t fg_color, uint32_t bg_color);

// Helper function to draw text strings
void draw_text(int x, int y, const char* text, uint32_t fg_color, uint32_t bg_color);

// Helper functoin to draw title text strings
void draw_title_text(int x, int y, const char* text, uint32_t fg_color, uint32_t bg_color);

#endif // FONT_H
