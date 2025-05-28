#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "font.h"
#include "../states/menu_state.h"
#include "../../drivers/graphics/graphics.h"
#include "../core/config.h"

#define BUTTON_WIDTH 250

// Menu UI functions - apenas renderização
void menu_ui_draw(const MenuState* state);
void menu_ui_draw_background(void);
void menu_ui_draw_title(void);
void menu_ui_draw_options(const MenuState* state);

// Helper functions
void draw_menu_option(int x, int y, const char* text, bool selected);

#endif
