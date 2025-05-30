#include "include/game_ui.h"

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

void draw_game_infos() {
    GameScore* score = tetris_get_score();

    // --- Score Box ---
    int score_box_x = LEFT_INFOS_ORIGIN_X;
    int score_box_y = SCORE_ORIGIN_Y;
    int score_box_w = LEFT_INFOS_WIDTH;
    int score_box_h = 100;

    // Draw frame
    draw_rectangle(score_box_x-2, score_box_y-2, score_box_w+4, score_box_h+4, FRAME_INFOS_COLOR);
    draw_rectangle(score_box_x, score_box_y, score_box_w, score_box_h, SCORE_BG_COLOR);

    int score_label_x = score_box_x + score_box_w / 2 - strlen("Score") * CHAR_WIDTH / 2;
    int score_label_y = score_box_y + 8;
    draw_text(score_label_x, score_label_y, "Score", 0xFFFFFF, SCORE_BG_COLOR);

    char buffer[32];
    sprintf(buffer, "%u", score->score);
    int score_value_x = score_box_x + score_box_w / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int score_value_y = score_box_y + score_box_h / 2 - CHAR_HEIGHT / 2;
    draw_text(score_value_x, score_value_y, buffer, 0xFFFFFF, SCORE_BG_COLOR);


    // --- Lines Box ---
    int lines_box_x = LEFT_INFOS_ORIGIN_X;
    int lines_box_y = score_box_y + score_box_h + 10;
    int lines_box_w = LEFT_INFOS_WIDTH;
    int lines_box_h = 100;

    // Draw frame
    draw_rectangle(lines_box_x-2, lines_box_y-2, lines_box_w+4, lines_box_h+4, FRAME_INFOS_COLOR);
    draw_rectangle(lines_box_x, lines_box_y, lines_box_w, lines_box_h, LINES_BG_COLOR);

    int lines_label_x = lines_box_x + lines_box_w / 2 - strlen("Lines") * CHAR_WIDTH / 2;
    int lines_label_y = lines_box_y + 8;
    draw_text(lines_label_x, lines_label_y, "Lines", 0xFFFFFF, LINES_BG_COLOR);

    sprintf(buffer, "%u", score->lines_cleared);
    int lines_value_x = lines_box_x + lines_box_w / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int lines_value_y = lines_box_y + lines_box_h / 2 - CHAR_HEIGHT / 2;
    draw_text(lines_value_x, lines_value_y, buffer, 0xFFFFFF, LINES_BG_COLOR);

    // Moldura dos TopScores

    // Moldura da NextPiece

    // Moldura do Level
}

void draw_highscores(void) {
    HighScore scores[MAX_HIGH_SCORES];
    int count;

    load_high_scores(scores, &count);

    draw_text(LEFT_INFOS_ORIGIN_X, 300, "Top Scores:", 0xFFFFFF, 0x222222);

    for (int i = 0; i < count; i++) {
        char buffer[64];
        sprintf(buffer, "%d. %s - %u", i + 1, scores[i].name, scores[i].score);
        draw_text(LEFT_INFOS_ORIGIN_X, 330 + i * 25, buffer, 0xFFFFFF, 0x222222);
    }
}


