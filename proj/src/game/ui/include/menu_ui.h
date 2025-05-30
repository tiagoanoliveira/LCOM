#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include "../../states/include/menu_state.h"

#define BUTTON_WIDTH 250

// Menu UI functions - apenas renderização
void menu_ui_draw(const MenuState* state);
void menu_ui_draw_title(void);
void menu_ui_draw_options(const MenuState* state);

// Helper functions
void draw_menu_option(int x, int y, const char* text, bool selected);

#endif
