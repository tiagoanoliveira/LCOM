#include "gameover_ui.h"
#include "font.h"
#include <string.h>
#include <stdio.h>

// Textos
static const char* GAMEOVER_TITLE = "GAME OVER";
static const char* GAMEOVER_OPTIONS[] = {
    "PLAY AGAIN",
    "BACK TO MENU",
    "QUIT"
};

void gameover_ui_draw(const GameOverState* state) {
    if (!state || !state->active) return;

    // Limpar ecrã
    vg_clear_screen(COLOR_BACKGROUND);

    // Desenhar fundo
    gameover_ui_draw_background();

    // Desenhar título "GAME OVER"
    gameover_ui_draw_title();

    // Desenhar pontuação
    gameover_ui_draw_score(state->final_score, state->lines_cleared);

    // Desenhar opções
    gameover_ui_draw_options(state);
}

void gameover_ui_draw_background(void) {
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

void gameover_ui_draw_title(void) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    int title_y = screen_height / 4;
    int title_width = strlen(GAMEOVER_TITLE) * CHAR_WIDTH;
    int title_x = (screen_width - title_width) / 2;

    // Fundo do título
    draw_rectangle(title_x - 30, title_y - 25, title_width + 60, CHAR_HEIGHT + 50, COLOR_BORDER);
    draw_rectangle(title_x - 25, title_y - 20, title_width + 50, CHAR_HEIGHT + 40, COLOR_BACKGROUND);

    // Texto do título
    draw_text(title_x, title_y, GAMEOVER_TITLE, COLOR_TITLE, COLOR_BACKGROUND);
}

void gameover_ui_draw_score(int score, int lines) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    char score_text[50];
    char lines_text[50];

    sprintf(score_text, "SCORE: %d", score);
    sprintf(lines_text, "LINES: %d", lines);

    int score_y = screen_height / 4 + 80;
    int lines_y = score_y + 40;

    // Desenhar pontuação
    int score_width = strlen(score_text) * CHAR_WIDTH;
    int score_x = (screen_width - score_width) / 2;
    draw_text(score_x, score_y, score_text, COLOR_SCORE, COLOR_BACKGROUND);

    // Desenhar linhas
    int lines_width = strlen(lines_text) * CHAR_WIDTH;
    int lines_x = (screen_width - lines_width) / 2;
    draw_text(lines_x, lines_y, lines_text, COLOR_SCORE, COLOR_BACKGROUND);
}

void gameover_ui_draw_options(const GameOverState* state) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    int menu_start_y = screen_height / 2;
    int option_spacing = 80;

    for (int i = 0; i < GAMEOVER_OPTIONS_COUNT; i++) {
        bool is_selected = (i == state->selected);
        int option_y = menu_start_y + i * option_spacing;

        draw_menu_option(screen_width / 2, option_y, GAMEOVER_OPTIONS[i], is_selected);
    }
}
