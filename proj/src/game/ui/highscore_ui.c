#include "include/highscore_ui.h"

static const char* HIGHSCORE_TITLE = "NEW HIGHSCORE!";

// Helper declarations
static void draw_highscore_title(void);
static void draw_highscore_score(uint32_t score);
static void draw_name_prompt(void);
static void draw_name_input(const char* input);
static void draw_continue_prompt(void);

void highscore_ui_draw(const HighscoreState* state) {
    if (!state || !state->active) return;

    vg_clear_screen(COLOR_BACKGROUND);
    draw_moldure();
    draw_decoration();

    draw_highscore_title();
    draw_highscore_score(state->score);
    draw_name_prompt();
    draw_name_input(state->name_input);
    draw_continue_prompt();
}

static void draw_highscore_title(void) {
    int title_width = strlen(HIGHSCORE_TITLE) * TITLE_CHAR_WIDTH;
    int title_x = (SCREEN_WIDTH - title_width) / 2;
    int title_y = SCREEN_HEIGHT / 6;

    draw_text(true, title_x, title_y, HIGHSCORE_TITLE, COLOR_TITLE, COLOR_BACKGROUND);
}

static void draw_highscore_score(uint32_t score) {
    char score_str[32];
    sprintf(score_str, "SCORE: %u", score);

    int score_width = strlen(score_str) * CHAR_WIDTH;
    int score_x = (SCREEN_WIDTH - score_width) / 2;
    int score_y = SCREEN_HEIGHT / 6 + 60;

    draw_text(false, score_x, score_y, score_str, COLOR_SCORE, COLOR_BACKGROUND);
}

static void draw_name_prompt(void) {
    int prompt_y = SCREEN_HEIGHT / 6 + 120;

    draw_text(false, 40, prompt_y, "ENTER YOUR NAME:", COLOR_O, COLOR_BACKGROUND);  // Using COLOR_O (red) as prompt color
}

static void draw_name_input(const char* input) {
    char input_display[MAX_NAME_LENGTH + 1] = {0};
    strncpy(input_display, input, MAX_NAME_LENGTH);
    input_display[MAX_NAME_LENGTH] = '\0';

    int input_width = strlen(input_display) * CHAR_WIDTH;
    int input_x = (SCREEN_WIDTH - input_width) / 2;
    int input_y = SCREEN_HEIGHT / 6 + 180;

    draw_text(false, input_x, input_y, input_display, COLOR_NORMAL, COLOR_BACKGROUND);
}

static void draw_continue_prompt(void) {
    const char* continue_text = "Press ENTER to continue";

    int continue_width = strlen(continue_text) * CHAR_WIDTH;
    int continue_x = (SCREEN_WIDTH - continue_width) / 2;
    int continue_y = SCREEN_HEIGHT / 6 + 260;

    draw_text(false, continue_x, continue_y, continue_text, COLOR_SELECTED, COLOR_BACKGROUND);
}
