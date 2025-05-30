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
    
    // --- Score Box ---
    draw_score();

    // --- Lines Box ---
    draw_lines();

    // --- High Scores ---
    draw_highscores();


    // --- Level Box ---
    draw_level();

    // --- Next Piece Box ---
    draw_next_piece();

}

void draw_score(void) {
    GameScore* score = tetris_get_score();

    int score_box_x = LEFT_INFOS_ORIGIN_X;
    int score_box_y = SCORE_ORIGIN_Y;
    int score_box_w = LEFT_INFOS_WIDTH;
    int score_box_h = 100;

    // Draw frame
    draw_rectangle(score_box_x-5, score_box_y-5, score_box_w+10, score_box_h+10, FRAME_INFOS_COLOR);
    draw_rectangle(score_box_x, score_box_y, score_box_w, score_box_h, SCORE_BG_COLOR);

    int score_label_x = score_box_x + score_box_w / 2 - strlen("Score") * CHAR_WIDTH / 2;
    int score_label_y = score_box_y + 8;
    draw_text(false, score_label_x, score_label_y, "Score", 0xFFFFFF, SCORE_BG_COLOR);

    char buffer[32];
    sprintf(buffer, "%u", score->score);
    int score_value_x = score_box_x + score_box_w / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int score_value_y = score_box_y + score_box_h / 2 - CHAR_HEIGHT / 2;
    draw_text(false, score_value_x, score_value_y, buffer, 0xFFFFFF, SCORE_BG_COLOR);
}

void draw_lines(void) {
    GameScore* score = tetris_get_score();

    int lines_box_x = LEFT_INFOS_ORIGIN_X;
    int lines_box_y = SCORE_ORIGIN_Y + 100 + 20;
    int lines_box_w = LEFT_INFOS_WIDTH;
    int lines_box_h = 100 + 10; // 100 for lines, 10 for spacing

    // Draw frame
    draw_rectangle(lines_box_x-5, lines_box_y-5, lines_box_w+10, lines_box_h+10, FRAME_INFOS_COLOR);
    draw_rectangle(lines_box_x, lines_box_y, lines_box_w, lines_box_h, LINES_BG_COLOR);

    int lines_label_x = lines_box_x + lines_box_w / 2 - strlen("Lines") * CHAR_WIDTH / 2;
    int lines_label_y = lines_box_y + 8;
    draw_text(false, lines_label_x, lines_label_y, "Lines", 0xFFFFFF, LINES_BG_COLOR);

    char buffer[32];
    sprintf(buffer, "%u", score->lines_cleared);
    int lines_value_x = lines_box_x + lines_box_w / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int lines_value_y = lines_box_y + lines_box_h / 2 - CHAR_HEIGHT / 2;
    draw_text(false, lines_value_x, lines_value_y, buffer, 0xFFFFFF, LINES_BG_COLOR);
}

void draw_highscores(void) {
    HighScore scores[MAX_HIGH_SCORES];
    int count;

    load_high_scores(scores, &count);

    // --- High Scores Box ---
    int hs_box_x = LEFT_INFOS_ORIGIN_X;
    int hs_box_y = SCORE_ORIGIN_Y + 300; // below lines box, adjust as needed
    int hs_box_w = LEFT_INFOS_WIDTH;
    int hs_box_h = 32 + count * 32 + 7; // 40 for label + space per score

    // Draw background and frame
    draw_rectangle(hs_box_x-5, hs_box_y-5, hs_box_w+10, hs_box_h+10, FRAME_INFOS_COLOR);
    draw_rectangle(hs_box_x, hs_box_y, hs_box_w, hs_box_h, HIGHSCORES_BG_COLOR);

    // Draw label, centered at the top of the box
    int label_x = hs_box_x + hs_box_w / 2 - strlen("Top Scores:") * CHAR_WIDTH / 2;
    int label_y = hs_box_y + 8;
    draw_text(false, label_x, label_y, "Top Scores:", 0xFFFFFF, HIGHSCORES_BG_COLOR);

    // Draw each high score entry
    for (int i = 0; i < count; i++) {
        char buffer[64];
        sprintf(buffer, "%d.%s-%u", i + 1, scores[i].name, scores[i].score);
        int entry_x = hs_box_x + 10; // left margin
        int entry_y = label_y + 32 + i * 32; // consistent spacing
        draw_text(false, entry_x, entry_y, buffer, 0xFFFFFF, HIGHSCORES_BG_COLOR);
    }
}

void draw_level(void) {
    GameScore* score = tetris_get_score();

    int level_box_x = RIGHT_INFOS_ORIGIN_X;
    int level_box_y = SCORE_ORIGIN_Y;
    int level_box_w = RIGHT_INFOS_WIDTH; 
    int level_box_h = 100;

    // Draw frame
    draw_rectangle(level_box_x-5, level_box_y-5, level_box_w+10, level_box_h+10, FRAME_INFOS_COLOR);
    draw_rectangle(level_box_x, level_box_y, level_box_w, level_box_h, LEVEL_BG_COLOR);

    // Draw label, centered at the top
    int level_label_x = level_box_x + level_box_w / 2 - strlen("Level") * CHAR_WIDTH / 2;
    int level_label_y = level_box_y + 8;
    draw_text(false, level_label_x, level_label_y, "Level", 0xFFFFFF, LEVEL_BG_COLOR);

    char buffer[16];
    sprintf(buffer, "%u", score->level);

    int level_value_x = level_box_x + level_box_w / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int level_value_y = level_box_y + level_box_h / 2 - CHAR_HEIGHT / 2;
    draw_text(false, level_value_x, level_value_y, buffer, 0xFFFFFF, LEVEL_BG_COLOR);
}

void draw_next_piece(void) {
    int next_box_x = RIGHT_INFOS_ORIGIN_X;
    int next_box_y = SCORE_ORIGIN_Y + 100 + 150;
    int next_box_w = RIGHT_INFOS_WIDTH;
    int next_box_h = 100;

    // Draw frame
    draw_rectangle(next_box_x-5, next_box_y-5, next_box_w+10, next_box_h+10, FRAME_INFOS_COLOR);
    draw_rectangle(next_box_x, next_box_y, next_box_w, next_box_h, NEXT_PIECE_BG_COLOR);

    // Draw label
    int label_x = next_box_x + next_box_w / 2 - strlen("Next") * CHAR_WIDTH / 2;
    int label_y = next_box_y + 8;
    draw_text(false, label_x, label_y, "Next", 0xFFFFFF, NEXT_PIECE_BG_COLOR);

    // Placeholder for the next piece 
    char buffer[8];
    sprintf(buffer, "-");
    int value_x = next_box_x + next_box_w / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int value_y = next_box_y + next_box_h / 2 - CHAR_HEIGHT / 2;
    draw_text(false, value_x, value_y, buffer, 0xFFFFFF, NEXT_PIECE_BG_COLOR);
}

