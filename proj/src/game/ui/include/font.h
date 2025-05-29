#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <ctype.h>
#include "../../../drivers/graphics/graphics.h"
#include "../../core/include/config.h"

// Font drawing function
void draw_character(int x, int y, char c, uint32_t fg_color, uint32_t bg_color);

// Helper function to draw text strings
void draw_text(int x, int y, const char* text, uint32_t fg_color, uint32_t bg_color);

// Helper function to draw title text strings
void draw_title_text(int x, int y, const char* text, uint32_t fg_color, uint32_t bg_color);

void draw_moldure(void);
void draw_decoration(void);

#endif // FONT_H
