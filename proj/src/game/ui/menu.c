#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Menu mainMenu;
GameOverMenu gameOverMenu;

// Menu text constants
static const char* MENU_TITLE = "TETR-IO-S";
static const char* MENU_OPTIONS[] = {
    "PLAY",
    "INSTRUCTIONS",
    "QUIT"
};
static const char* GAME_OVER_OPTIONS[] = {
    "PLAY AGAIN",
    "BACK TO MENU",
    "QUIT"
};

// Colors
static const uint32_t COLOR_BACKGROUND = 0x00;      // Black
static const uint32_t COLOR_TITLE = 0x0F;           // White
static const uint32_t COLOR_SELECTED = 0x0E;        // Yellow
static const uint32_t COLOR_NORMAL = 0x07;          // Light gray
static const uint32_t COLOR_BORDER = 0x03;          // Cyan

void menu_init(Menu* menu) {
    if (!menu) return;

    menu->selected = MENU_PLAY;
    menu->active = true;
}

void menu_draw(const Menu* menu) {
    if (!menu || !menu->active) return;

    // Get current resolution from mode_info
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    // Clear screen with background color
    vg_clear_screen(COLOR_BACKGROUND);

    // Draw background elements
    draw_menu_background();

    // Draw title
    draw_menu_title();

    // Calculate menu position (center of screen)
    int menu_start_y = screen_height / 2 - 50;
    int option_spacing = 80;

    // Draw menu options
    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        bool is_selected = (i == menu->selected);
        int option_y = menu_start_y + (i * option_spacing);
        draw_menu_option(screen_width / 2, option_y, MENU_OPTIONS[i], is_selected);
    }
}

void menu_handle_input(Menu* menu, uint8_t scancode[2], bool two_byte) {
    if (!menu || !menu->active) return;

    // Handle ESC key (quit)
    if (scancode[0] == 0x81) { // ESC break code
        menu->selected = MENU_QUIT;
        state_set_redraw(true);
        return;
    }

    // Handle arrow keys for navigation
    if (!two_byte && scancode[0] == 0xE0) {
        switch (scancode[1]) {
            case 0x48: // Up arrow
                menu->selected = (menu->selected == 0) ? MENU_OPTIONS_COUNT - 1 : menu->selected - 1;
                state_set_redraw(true);
                break;
            case 0x50: // Down arrow
                menu->selected = (menu->selected + 1) % MENU_OPTIONS_COUNT;
                state_set_redraw(true);
                break;
        }
    }
}

void menu_update(Menu* menu) {
    (void)menu;
}

GameState menu_get_selected_action(const Menu* menu) {
    if (!menu || !menu->active) return STATE_MENU;
    switch (menu->selected) {
        case MENU_PLAY:
            return STATE_GAME;
        case MENU_INSTRUCTIONS:
            return STATE_MENU; // Por agora volta ao menu
        case MENU_QUIT:
            return STATE_QUIT;
        default:
            return STATE_MENU;
    }
}

void draw_menu_option(int x, int y, const char* text, bool selected) {
    if (!text) return;

    uint32_t bgcolor = selected ? COLOR_SELECTED : COLOR_BACKGROUND;
    uint32_t textcolor = selected ? COLOR_BACKGROUND : COLOR_NORMAL;
    uint32_t bordercolor = selected ? COLOR_BORDER : COLOR_NORMAL;

    int textwidth = strlen(text) * CHAR_WIDTH;
    int startx = x - textwidth / 2;

    int screen_width = mode_info.XResolution;
    if (startx < 20) startx = 20;
    if (startx + textwidth > screen_width - 20) startx = screen_width - textwidth - 20;

    // Draw option background with border
    draw_rectangle(startx - 25, y - 20, textwidth + 50, CHAR_HEIGHT + 40, bordercolor);
    draw_rectangle(startx - 20, y - 15, textwidth + 40, CHAR_HEIGHT + 30, bgcolor);

    // Draw text using the font
    draw_text(startx, y, text, textcolor, bgcolor);
}


void draw_menu_title(void) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    // Draw title at the top center
    int title_y = screen_height / 4;
    int title_width = strlen(MENU_TITLE) * CHAR_WIDTH;
    int title_x = (screen_width - title_width) / 2;

    // Draw title background
    draw_rectangle(title_x - 30, title_y - 25, title_width + 60, CHAR_HEIGHT + 50, COLOR_BORDER);
    draw_rectangle(title_x - 25, title_y - 20, title_width + 50, CHAR_HEIGHT + 40, COLOR_BACKGROUND);

    // Draw title text using the font
    draw_text(title_x, title_y, MENU_TITLE, COLOR_TITLE, COLOR_BACKGROUND);
}

void draw_menu_background(void) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    // Draw a border around the screen
    draw_rectangle(10, 10, screen_width - 20, screen_height - 20, COLOR_BORDER);
    draw_rectangle(15, 15, screen_width - 30, screen_height - 30, COLOR_BACKGROUND);

    // Draw decorative blocks
    for (int i = 0; i < 8; i++) {
        int x = 40 + (i * (screen_width - 100) / 7);
        int y = screen_height - 80;
        draw_rectangle(x, y, 25, 25, COLOR_BORDER);
        draw_rectangle(x + 2, y + 2, 21, 21, COLOR_SELECTED);
    }
}
void game_over_menu_init(GameOverMenu* menu) {
    if (!menu) return;
    menu->selected = GAME_OVER_PLAY_AGAIN;
    menu->active = true;
}

void game_over_menu_draw(const GameOverMenu* menu) {
    if (!menu || !menu->active) return;

    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    vg_clear_screen(COLOR_BACKGROUND);
    draw_menu_background();
    draw_menu_title();

    // Draw "GAME OVER" message
    const char* msg = "GAME OVER";
    int msg_width = strlen(msg) * CHAR_WIDTH;
    int msg_x = (screen_width - msg_width) / 2;
    int msg_y = screen_height / 4 + 80;
    draw_text(msg_x, msg_y, msg, COLOR_TITLE, COLOR_BACKGROUND);

    // Draw options
    int menu_start_y = screen_height / 2;
    int option_spacing = 80;
    for (int i = 0; i < GAME_OVER_OPTIONS_COUNT; i++) {
        bool is_selected = (i == menu->selected);
        int option_y = menu_start_y + (i * option_spacing);
        draw_menu_option(screen_width / 2, option_y, GAME_OVER_OPTIONS[i], is_selected);
    }
}

void game_over_menu_handle_input(GameOverMenu* menu, uint8_t scancode[2], bool twobyte) {
    if (!menu || !menu->active) return;

    // Handle arrow keys for navigation
    if (!twobyte && scancode[0] == 0xE0) {
        switch (scancode[1]) {
            case 0x48: // Up arrow
                menu->selected = (menu->selected == 0) ? GAME_OVER_OPTIONS_COUNT - 1 : menu->selected - 1;
                state_set_redraw(true);
                break;
            case 0x50: // Down arrow
                menu->selected = (menu->selected + 1) % GAME_OVER_OPTIONS_COUNT;
                state_set_redraw(true);
                break;
        }
    }
}

GameState game_over_menu_get_selected_action(const GameOverMenu* menu) {
    if (!menu || !menu->active) return STATE_GAME_OVER;
    switch (menu->selected) {
        case GAME_OVER_PLAY_AGAIN:
            return STATE_GAME;
        case GAME_OVER_BACK_TO_MENU:
            return STATE_MENU;
        case GAME_OVER_QUIT:
            return STATE_QUIT;
        default:
            return STATE_GAME_OVER;
    }
}
