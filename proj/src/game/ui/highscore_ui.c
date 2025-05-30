#include "include/highscore_ui.h"

static const char* HIGHSCORE_TITLE = "NEW HIGHSCORE";

// Box dimensions
#define HS_BOX_W 500
#define HS_BOX_H 220
#define HS_BOX_BG 0x00
#define HS_BOX_FRAME COLOR_SELECTED

void highscore_ui_draw(const HighscoreState* state) {
    if (!state || !state->active) return;

    vg_clear_screen(COLOR_BACKGROUND);
    draw_moldure();
    draw_decoration();

    // Centered box
    int box_x = (SCREEN_WIDTH - HS_BOX_W) / 2;
    int box_y = (SCREEN_HEIGHT - HS_BOX_H) / 2;
    draw_highscore_box(box_x, box_y, HS_BOX_W, HS_BOX_H);

    // Title
    draw_highscore_title(box_x, box_y);

    // Score
    draw_highscore_score(box_x, box_y, state->score);

    // Name prompt and input
    int score_y = box_y + 60;
    int prompt_y = score_y + 30;
    draw_highscore_name_prompt(box_x, box_y, score_y);
    draw_highscore_name_input(box_x, box_y, prompt_y, state->name_input);

    // Continue prompt
    draw_highscore_continue_prompt(box_x, box_y);
}

void draw_highscore_box(int x, int y, int w, int h) {
    draw_rectangle(x-5, y-5, w+10, h+10, HS_BOX_FRAME); // Frame
    draw_rectangle(x, y, w, h, HS_BOX_BG);              // Background
}

void draw_highscore_title(int box_x, int box_y) {
    int title_width = strlen(HIGHSCORE_TITLE) * TITLE_CHAR_WIDTH;
    int title_x = box_x + (HS_BOX_W - title_width) / 2;
    int title_y = box_y - 95;
    draw_text(true, title_x, title_y, HIGHSCORE_TITLE, COLOR_TITLE, HS_BOX_BG);
}

void draw_highscore_score(int box_x, int box_y, unsigned int score) {
    char score_str[32];
    sprintf(score_str, "SCORE: %u", score);
    int score_width = strlen(score_str) * CHAR_WIDTH;
    int score_x = box_x + (HS_BOX_W - score_width) / 2;
    int score_y = box_y + 40;
    draw_text(false, score_x, score_y, score_str, COLOR_SCORE, HS_BOX_BG);
}

void draw_highscore_name_prompt(int box_x, int box_y, int score_y) {
    const char* prompt = "ENTER YOUR NAME:";
    int prompt_width = strlen(prompt) * CHAR_WIDTH;
    int prompt_x = box_x + (HS_BOX_W - prompt_width) / 2;
    int prompt_y = score_y + 40;
    draw_text(false, prompt_x, prompt_y, prompt, COLOR_O, HS_BOX_BG);
}

void draw_highscore_name_input(int box_x, int box_y, int prompt_y, const char* input) {
    char input_display[MAX_NAME_LENGTH + 1] = {0};
    strncpy(input_display, input, MAX_NAME_LENGTH);
    input_display[MAX_NAME_LENGTH] = '\0';
    int input_width = strlen(input_display) * CHAR_WIDTH;
    int input_x = box_x + (HS_BOX_W - input_width) / 2;
    int input_y = prompt_y + 40;
    draw_text(false, input_x, input_y, input_display, COLOR_NORMAL, HS_BOX_BG);
}

void draw_highscore_continue_prompt(int box_x, int box_y) {
    const char* continue_text = "Press ENTER to continue";
    int continue_width = strlen(continue_text) * CHAR_WIDTH;
    int continue_x = box_x + (HS_BOX_W - continue_width) / 2;
    int continue_y = box_y + HS_BOX_H - 30;
    draw_text(false, continue_x, continue_y, continue_text, COLOR_SELECTED, HS_BOX_BG);
}
