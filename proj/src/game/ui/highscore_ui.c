#include <string.h>
#include <stdio.h>
#include "include/highscore_ui.h"

static const char* HIGHSCORE_TITLE = "NEW HIGHSCORE!";

void highscore_ui_draw(const HighscoreState* state) {
    if (!state || !state->active) return;

    vg_clear_screen(COLOR_BACKGROUND);
    draw_moldure();

    int screen_width = mode_info.XResolution;
    int screen_height = mode_info.YResolution;

    // Draw title centered
    int title_width = strlen(HIGHSCORE_TITLE) * TITLE_CHAR_WIDTH;
    int title_x = (screen_width - title_width) / 2;
    int title_y = screen_height / 6;
    draw_title_text(title_x, title_y, HIGHSCORE_TITLE, 0xFFFFFF, 0x222222); // Yellow

    // Draw score centered
    char score_str[32];
    sprintf(score_str, "SCORE: %u", state->score);
    int score_width = strlen(score_str) * CHAR_WIDTH;
    int score_x = (screen_width - score_width) / 2;
    int score_y = title_y + 60;
    draw_text(score_x, score_y, score_str, 0xFFFFFF, 0x222222); // White

    // Prompt for name (left-aligned)
    int prompt_y = score_y + 60;
    draw_text(40, prompt_y, "ENTER YOUR NAME:", 0xFF00FF, 0x222222); // Cyan

    // Show input field centered
    char input_display[MAX_NAME_LENGTH + 1] = {0};
    strncpy(input_display, state->name_input, MAX_NAME_LENGTH);
    int input_width = strlen(input_display) * CHAR_WIDTH;
    int input_x = (screen_width - input_width) / 2;
    int input_y = prompt_y + 60;
    draw_text(input_x, input_y, input_display, 0xFFFFFF, 0x222222); // White

    // Prompt to continue centered
    const char* continue_text = "Press ENTER to continue";
    int continue_width = strlen(continue_text) * CHAR_WIDTH;
    int continue_x = (screen_width - continue_width) / 2;
    int continue_y = input_y + 80;
    draw_text(continue_x, continue_y, continue_text, 0xFF8888, 0x222222); // Gray
}
