#include <stdio.h>
#include <string.h>
#include "include/menu_ui.h"
#include "include/font.h"
#include "../core/include/config.h"
#include "../../xpm/background.XPM"
#include "../../xpm/flip_background.XPM"

// Textos
static const char* MENU_TITLE = "TETR-IO-S";
static const char* MENU_OPTIONS[] = {
    "PLAY",
    "INSTRUCTIONS",
    "QUIT"
};

void menu_ui_draw(const MenuState* state) {
    if (!state || !state->active) return;

    // Limpar ecrã
    vg_clear_screen(COLOR_BACKGROUND);

    // Desenhar elementos do menu
    draw_moldure();
    draw_decoration();
    menu_ui_draw_title();
    menu_ui_draw_options(state);

    menu_ui_draw_background();
}

void menu_ui_draw_background(void) {
    draw_moldure();  

    draw_xpm_indexed(flip_background_xpm, 16, 16);

    draw_xpm_indexed(background_xpm, 16, 768 - 205);
}

void menu_ui_draw_title(void) {
    const char* title = MENU_TITLE;
    int title_width = strlen(title) * TITLE_CHAR_WIDTH;
    const int title_x = (SCREEN_WIDTH - title_width) / 2;
    const int title_y = SCREEN_HEIGHT / 4 + 35;

    draw_text(true, title_x, title_y, title, COLOR_TITLE, COLOR_BACKGROUND);
}

void menu_ui_draw_options(const MenuState* state) {
    int menu_start_y = SCREEN_HEIGHT / 2 - 50;

    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        const bool is_selected = (i == state->selected);
        const int option_y = menu_start_y + i * 80;

        draw_menu_option(SCREEN_WIDTH / 2, option_y, MENU_OPTIONS[i], is_selected);
    }
}

void draw_menu_option(const int x, const int y, const char* text, const bool selected) {
    if (!text) return;

    const uint32_t bg_color = selected ? COLOR_SELECTED : COLOR_BACKGROUND;
    const uint32_t text_color = selected ? COLOR_BACKGROUND : COLOR_NORMAL;
    const uint32_t border_color = selected ? COLOR_BORDER : COLOR_NORMAL;

    const int text_width = strlen(text) * CHAR_WIDTH;
    int start_x = x - BUTTON_WIDTH / 2;

    if (start_x < 20) start_x = 20;
    if (start_x + BUTTON_WIDTH > SCREEN_WIDTH - 20) {
        start_x = SCREEN_WIDTH - BUTTON_WIDTH - 20;
    }

    // Draw option background with border using BUTTON_WIDTH
    draw_rectangle(start_x - 25, y - 20, BUTTON_WIDTH + 50, CHAR_HEIGHT + 40, border_color);
    draw_rectangle(start_x - 20, y - 15, BUTTON_WIDTH + 40, CHAR_HEIGHT + 30, bg_color);

    // Draw text centered in the button
    const int text_x = x - text_width / 2;
    draw_text(false, text_x, y, text, text_color, bg_color);
}
