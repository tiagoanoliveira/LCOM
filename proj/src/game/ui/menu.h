#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "../../drivers/graphics/graphics.h"
#include "../../drivers/keyboardMouse/keyboard.h"
#include "../core/state.h"
#include "font.h"

// Menu options
typedef enum {
    MENU_PLAY,
    MENU_INSTRUCTIONS,
    MENU_QUIT,
    MENU_OPTIONS_COUNT
} MenuOption;

// Menu structure
typedef struct {
    MenuOption selected;
    bool active;
} Menu;

// Global menu instance
extern Menu mainMenu;

// Menu functions
void menu_init(Menu* menu);
void menu_draw(const Menu* menu);
void menu_handle_input(Menu* menu, uint8_t scancode[2], bool twobyte);
void menu_update(Menu* menu);
GameState menu_get_selected_action(const Menu* menu);

// Helper functions
void draw_menu_option(int x, int y, const char* text, bool selected);
void draw_menu_title(void);
void draw_menu_background(void);

#endif // MENU_H
