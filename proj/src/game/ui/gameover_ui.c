#include <string.h>
#include <stdio.h>
#include "include/gameover_ui.h"

// Textos
static const char* GAMEOVER_TITLE = "GAME OVER";
static const char* GAMEOVER_OPTIONS[] = {
    "PLAY AGAIN",
    "BACK TO MENU",
    "QUIT"
};

void gameover_ui_draw(const GameOverState* state) {
    if (!state || !state->active) return;

    vg_clear_screen(COLOR_BACKGROUND);
    gameover_ui_draw_background();

    if (state->substate == GAMEOVER_SUBSTATE_HIGHSCORE) {
        // Only draw highscore input
        draw_highscore_input(state);
    } else {
        // Draw normal game over UI
        gameover_ui_draw_title();
        gameover_ui_draw_score(state->final_score, state->lines_cleared);
        gameover_ui_draw_options(state);
    }
}

void gameover_ui_draw_background(void) {
    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    draw_moldure();

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
    int title_width = strlen(GAMEOVER_TITLE) * TITLE_CHAR_WIDTH;
    int title_x = (screen_width - title_width) / 2;

    draw_title_text(title_x, title_y, GAMEOVER_TITLE, COLOR_TITLE, COLOR_BACKGROUND);
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

void draw_highscore_input(const GameOverState* state) {
    if (!(state->substate==GAMEOVER_SUBSTATE_HIGHSCORE)) return;

    int screen_width = mode_info.XResolution;
    int y = mode_info.YResolution / 2 - 100;

    draw_text(screen_width / 2 - 100, y, "NEW HIGHSCORE! ENTER NAME:", 0xFFFFFF, COLOR_BACKGROUND);
    draw_rectangle(screen_width / 2 - 50, y + 30, 100, 30, 0x444444);

    char display[MAX_NAME_LENGTH + 1] = "";
    strncpy(display, state->name_input, MAX_NAME_LENGTH);
    draw_text(screen_width / 2 - 40, y + 35, display, 0xFFFFFF, 0x444444);
}

