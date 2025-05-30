#include "include/render.h"

void draw_block(int col, int row, uint32_t color) {
    int x = grid_origin_x + col * CELL_SIZE;
    int y = grid_origin_y + row * CELL_SIZE;
    draw_rectangle(x, y, CELL_SIZE, CELL_SIZE, color);
}

void draw_current_piece(const Piece* piece) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (piece->shape[i][j]) {
                int gx = piece->x + j;
                int gy = piece->y + i;
                draw_block(gx, gy, piece->color);
            }
        }
    }
}

void draw_grid_contents(int grid[20][10]) {
    for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
            if (grid[row][col]) {
                draw_block(col, row, grid[row][col]);
            }
        }
    }
}
void draw_grid_background(){
    draw_rectangle(grid_origin_x, grid_origin_y, GRID_WIDTH, GRID_HEIGHT, GRID_BACKGROUND_COLOR);
}

void draw_grid() {
    // Horizontal lines
    for (int i = 0; i <= GRID_ROWS; i++) {
        int y = grid_origin_y + i * CELL_SIZE;
        draw_rectangle(grid_origin_x, y, GRID_WIDTH, 1, GRID_COLOR);
    }

    // Vertical lines
    for (int j = 0; j <= GRID_COLS; j++) {
        int x = grid_origin_x + j * CELL_SIZE;
        draw_rectangle(x, grid_origin_y, 1, GRID_HEIGHT, GRID_COLOR);
    }

    //Moldura da grelha
    draw_rectangle(GRID_WITH_BORDER_ORIGIN_X, GRID_WITH_BORDER_ORIGIN_Y, CELL_SIZE, GRID_WITH_BORDER_HEIGHT, GRID_BORDER_COLOR); // Linha esquerda
    draw_rectangle(GRID_WITH_BORDER_ORIGIN_X + GRID_WIDTH + CELL_SIZE + 1, GRID_WITH_BORDER_ORIGIN_Y, CELL_SIZE, GRID_WITH_BORDER_HEIGHT, GRID_BORDER_COLOR); // Linha direita
    draw_rectangle(GRID_WITH_BORDER_ORIGIN_X, GRID_WITH_BORDER_ORIGIN_Y, GRID_WITH_BORDER_WIDTH, CELL_SIZE, GRID_BORDER_COLOR); // Linha de cima
    draw_rectangle(GRID_WITH_BORDER_ORIGIN_X, GRID_WITH_BORDER_ORIGIN_Y + GRID_HEIGHT + CELL_SIZE + 1, GRID_WITH_BORDER_WIDTH, CELL_SIZE, GRID_BORDER_COLOR); // Linha de baixo

    // Linhas horizontais na moldura da grelha
    for (int i = 0; i <= GRID_ROWS; i++) {
        int y = grid_origin_y + i * CELL_SIZE;
        if (i==0 || i==GRID_ROWS){
            draw_rectangle(GRID_WITH_BORDER_ORIGIN_X, y, GRID_WITH_BORDER_WIDTH, 1, GRID_BORDER_LINES_COLOR);
        }
        else {
            draw_rectangle(GRID_WITH_BORDER_ORIGIN_X, y, CELL_SIZE, 1, GRID_BORDER_LINES_COLOR);
            draw_rectangle(GRID_WITH_BORDER_ORIGIN_X + GRID_WIDTH + CELL_SIZE + 1, y, CELL_SIZE, 1, GRID_BORDER_LINES_COLOR);
        }
    }

    // Linhas verticais na moldura da grelha
    for (int j = 0; j <= GRID_COLS; j++) {
        int x = grid_origin_x + j * CELL_SIZE;
        if (j==0 || j==GRID_COLS){
            draw_rectangle(x, GRID_WITH_BORDER_ORIGIN_Y, 1, GRID_WITH_BORDER_HEIGHT, GRID_BORDER_LINES_COLOR);
        }
        draw_rectangle(x, GRID_WITH_BORDER_ORIGIN_Y, 1, CELL_SIZE, GRID_BORDER_LINES_COLOR);
        draw_rectangle(x, GRID_WITH_BORDER_ORIGIN_Y + GRID_HEIGHT + CELL_SIZE + 1, 1, CELL_SIZE, GRID_BORDER_LINES_COLOR);
    }
}

void draw_score_info() {
    GameScore* score = tetris_get_score();

    draw_rectangle(10, 40, 150, 70, 0x222222); // Reactangle for score info background

    char buffer[64];
    sprintf(buffer, "Score: %u", score->score);
    draw_text(20, 50, buffer, 0xFFFFFF, 0x222222);

    sprintf(buffer, "Lines: %u", score->lines_cleared);
    draw_text(20, 70, buffer, 0xFFFFFF, 0x222222);

    sprintf(buffer, "Level: %u", score->level);
    draw_text(20, 90, buffer, 0xFFFFFF, 0x222222);
}

void draw_highscores(void) {
    HighScore scores[MAX_HIGH_SCORES];
    int count;

    load_high_scores(scores, &count);

    draw_text(20, 110, "Top Scores:", 0xFFFFFF, 0x222222);

    for (int i = 0; i < count; i++) {
        char buffer[64];
        sprintf(buffer, "%d. %s - %u", i + 1, scores[i].name, scores[i].score);
        draw_text(20, 130 + i * 20, buffer, 0xFFFFFF, 0x222222);
    }
}

// Highscore entry UI is now only used by STATE_HIGHSCORE
/*
void highscore_ui_draw(const HighscoreState* state) {
    if (!state || !state->active) return;

    vg_clear_screen(COLOR_BACKGROUND);
    draw_moldure();

    // Draw "NEW HIGHSCORE!" title centered
    const char* title = "NEW HIGHSCORE!";
    int title_x = (mode_info.XResolution - (strlen(title) * TITLE_CHAR_WIDTH)) / 2;
    int title_y = 3 * 10;
    draw_title_text(title_x, title_y, title, 0xFFFFFF00, 0xFF000000);  // Yellow text, black background

    // Draw the player's score centered
    char score_str[32];
    snprintf(score_str, sizeof(score_str), "SCORE: %u", state->score);
    int score_x = (mode_info.XResolution - (strlen(score_str) * 8)) / 2;
    draw_text(score_x, 8 * 16, score_str, 0xFFFFFFFF, 0xFF000000);  // White text, black background

    // Prompt for name (left-aligned)
    draw_text(10, 11 * 16, "ENTER YOUR NAME:", 0xFF00FFFF, 0xFF000000);  // Cyan text, black background

    // Show input field centered
    char input_display[MAX_NAME_LENGTH + 1] = {0};
    strncpy(input_display, state->name_input, MAX_NAME_LENGTH);
    int input_x = (mode_info.XResolution - (strlen(input_display) * 8)) / 2;
    draw_text(input_x, 13 * 16, input_display, 0xFFFFFFFF, 0xFF000000);  // White text, black background

    // Prompt to continue centered
    const char* continue_text = "Press ENTER to continue";
    int continue_x = (mode_info.XResolution - (strlen(continue_text) * 8)) / 2;
    draw_text(continue_x, 16 * 16, continue_text, 0xFF888888, 0xFF000000);  // Gray text, black background
}*/

