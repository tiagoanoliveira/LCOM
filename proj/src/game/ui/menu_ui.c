#include "menu_ui.h"
#include "font.h"
#include <stdio.h>
#include <string.h>

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
    menu_ui_draw_background();
    menu_ui_draw_title();
    menu_ui_draw_options(state);
}

void menu_ui_draw_background(void) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    // Desenhar moldura
    draw_rectangle(10, 10, screen_width - 20, screen_height - 20, COLOR_BORDER);
    draw_rectangle(15, 15, screen_width - 30, screen_height - 30, COLOR_BACKGROUND);

    // Blocos decorativos
    for (int i = 0; i < 8; i++) {
        int x = 40 + i * (screen_width - 100) / 7;
        int y = screen_height - 80;
        draw_rectangle(x, y, 25, 25, COLOR_BORDER);
        draw_rectangle(x + 2, y + 2, 21, 21, COLOR_SELECTED);
    }
}

void menu_ui_draw_title(void) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    const char* title = MENU_TITLE;
    int title_width = strlen(title) * CHAR_WIDTH;
    int title_x = (screen_width - title_width) / 2;
    int title_y = screen_height / 4;

    // Fundo do título
    draw_rectangle(title_x - 30, title_y - 25, title_width + 60, CHAR_HEIGHT + 50, COLOR_BORDER);
    draw_rectangle(title_x - 25, title_y - 20, title_width + 50, CHAR_HEIGHT + 40, COLOR_BACKGROUND);

    // Texto do título
    draw_text(title_x, title_y, title, COLOR_TITLE, COLOR_BACKGROUND);
}

void menu_ui_draw_options(const MenuState* state) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    int menu_start_y = screen_height / 2 - 50;
    int option_spacing = 80;

    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        bool is_selected = (i == state->selected);
        int option_y = menu_start_y + i * option_spacing;

        draw_menu_option(screen_width / 2, option_y, MENU_OPTIONS[i], is_selected);
    }
}

void draw_menu_option(int x, int y, const char* text, bool selected) {
    if (!text) return;

    uint32_t bg_color = selected ? COLOR_SELECTED : COLOR_BACKGROUND;
    uint32_t text_color = selected ? COLOR_BACKGROUND : COLOR_NORMAL;
    uint32_t border_color = selected ? COLOR_BORDER : COLOR_NORMAL;

    int text_width = strlen(text) * CHAR_WIDTH;
    int start_x = x - text_width / 2;

    int screen_width = mode_info.XResolution;
    if (start_x < 20) start_x = 20;
    if (start_x + text_width > screen_width - 20) {
        start_x = screen_width - text_width - 20;
    }

    // Desenhar fundo da opção
    draw_rectangle(start_x - 25, y - 20, text_width + 50, CHAR_HEIGHT + 40, border_color);
    draw_rectangle(start_x - 20, y - 15, text_width + 40, CHAR_HEIGHT + 30, bg_color);

    // Desenhar texto
    draw_text(start_x, y, text, text_color, bg_color);
}
