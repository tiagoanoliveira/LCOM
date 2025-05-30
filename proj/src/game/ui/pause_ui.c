#include <string.h>
#include "include/font.h"
#include "include/pause_ui.h"

#include "../core/include/config.h"

static const char* PAUSE_TITLE = "PAUSED";
static const char* PAUSE_OPTIONS[] = {"BACK TO GAME", "EXIT"};
#define PAUSE_OPTIONS_COUNT 2

void pause_ui_draw(const GameLogic* game) {
    if (!game || !game->paused) return;

    // Desenhar overlay semi-transparente
    pause_ui_draw_overlay();

    // Desenhar menu de pause
    pause_ui_draw_menu(game);
}

void pause_ui_draw_overlay(void) {
    // Overlay escuro semi-transparente (simulado com padrão)
    for (int y = 0; y < SCREEN_HEIGHT; y += 4) {
        for (int x = 0; x < SCREEN_WIDTH; x += 4) {
            draw_rectangle(x, y, 2, 2, 0x00);
        }
    }
}

void pause_ui_draw_menu(const GameLogic* game) {
    // Dimensões da janela de pause
    int window_x = (SCREEN_WIDTH - PAUSE_WIDTH) / 2;
    int window_y = (SCREEN_HEIGHT - PAUSE_HEIGHT) / 2;

    // Fundo da janela
    draw_rectangle(window_x, window_y, PAUSE_WIDTH, PAUSE_HEIGHT, COLOR_BORDER);
    draw_rectangle(window_x + 5, window_y + 5, PAUSE_WIDTH - 10, PAUSE_HEIGHT - 10, COLOR_BACKGROUND);

    // Título
    int title_width = strlen(PAUSE_TITLE) * CHAR_WIDTH;
    int title_x = window_x + (PAUSE_WIDTH - title_width) / 2;
    int title_y = window_y + 30;
    draw_text(false, title_x, title_y, PAUSE_TITLE, COLOR_TITLE, COLOR_BACKGROUND);

    // Opções
    int options_start_y = window_y + 80;

    for (int i = 0; i < PAUSE_OPTIONS_COUNT; i++) {
        int option_spacing = 50;
        bool is_selected = (i == game->pause_option);
        int option_y = options_start_y + i * option_spacing;

        uint32_t bg_color = is_selected ? COLOR_SELECTED : COLOR_BACKGROUND;
        uint32_t text_color = is_selected ? COLOR_BACKGROUND : COLOR_NORMAL;

        int text_width = strlen(PAUSE_OPTIONS[i]) * CHAR_WIDTH;
        int text_x = window_x + (PAUSE_WIDTH - text_width) / 2;

        // Fundo da opção
        if (is_selected) {
            draw_rectangle(text_x - 10, option_y - 5, text_width + 20, CHAR_HEIGHT + 10, bg_color);
        }

        draw_text(false, text_x, option_y, PAUSE_OPTIONS[i], text_color, bg_color);
    }
}
