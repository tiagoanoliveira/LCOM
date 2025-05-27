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

// Game over options
typedef enum {
    GAME_OVER_PLAY_AGAIN,
    GAME_OVER_BACK_TO_MENU,
    GAME_OVER_QUIT,
    GAME_OVER_OPTIONS_COUNT
} GameOverOption;

typedef struct {
    GameOverOption selected;
    bool active;
} GameOverMenu;

extern GameOverMenu gameOverMenu;

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

// Game over functions
void game_over_menu_init(GameOverMenu* menu);
void game_over_menu_draw(const GameOverMenu* menu);
void game_over_menu_handle_input(GameOverMenu* menu, uint8_t scancode[2], bool twobyte);
GameState game_over_menu_get_selected_action(const GameOverMenu* menu);

#endif // MENU_H
