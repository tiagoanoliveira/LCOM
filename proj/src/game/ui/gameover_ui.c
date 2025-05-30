#include <string.h>
#include <stdio.h>
#include "include/gameover_ui.h"
#include "include/font.h"
#include "../core/include/config.h"
#include "../../xpm/background.XPM"

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
    draw_moldure();
    draw_decoration();

    // Desenhar título "GAME OVER"
    gameover_ui_draw_title();

    // Desenhar pontuação
    gameover_ui_draw_score(state->final_score, state->lines_cleared);

    // Desenhar opções
    gameover_ui_draw_options(state);

}

void gameover_ui_draw_background(void) {
    draw_moldure();

    draw_xpm_indexed(background_xpm, 16, 768 - 205);
}

void gameover_ui_draw_title(void) {
    int title_y = SCREEN_HEIGHT / 4;
    int title_width = strlen(GAMEOVER_TITLE) * TITLE_CHAR_WIDTH;
    int title_x = (SCREEN_WIDTH - title_width) / 2;

    draw_text(true, title_x, title_y, GAMEOVER_TITLE, COLOR_TITLE, COLOR_BACKGROUND);
}

void gameover_ui_draw_score(int score, int lines) {
    char score_text[50];
    char lines_text[50];

    sprintf(score_text, "SCORE:%d", score);
    sprintf(lines_text, "LINES:%d", lines);

    int score_y = SCREEN_HEIGHT / 4 + 80;
    int lines_y = score_y + 40;

    // Desenhar pontuação
    int score_width = strlen(score_text) * CHAR_WIDTH;
    int score_x = (SCREEN_WIDTH - score_width) / 2;
    draw_text(false, score_x, score_y, score_text, COLOR_SCORE, COLOR_BACKGROUND);

    // Desenhar linhas
    int lines_width = strlen(lines_text) * CHAR_WIDTH;
    int lines_x = (SCREEN_WIDTH - lines_width) / 2;
    draw_text(false, lines_x, lines_y, lines_text, COLOR_SCORE, COLOR_BACKGROUND);
}

void gameover_ui_draw_options(const GameOverState* state) {
    int menu_start_y = SCREEN_HEIGHT / 2;
    int option_spacing = 80;

    for (int i = 0; i < GAMEOVER_OPTIONS_COUNT; i++) {
        bool is_selected = (i == state->selected);
        int option_y = menu_start_y + i * option_spacing;

        draw_menu_option(SCREEN_WIDTH / 2, option_y, GAMEOVER_OPTIONS[i], is_selected);
    }
}
