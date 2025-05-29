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

    // Moldura do score
    draw_rectangle(LEFT_INFOS_ORIGIN_X, SCORE_ORIGIN_Y, LEFT_INFOS_WIDTH, FRAME_INFOS_THICKNESS, FRAME_INFOS_COLOR); // Linha superior
    draw_rectangle(LEFT_INFOS_ORIGIN_X, SCORE_ORIGIN_Y + SCORE_HEIGHT, LEFT_INFOS_WIDTH, FRAME_INFOS_THICKNESS, FRAME_INFOS_COLOR); // Linha inferior
    draw_rectangle(LEFT_INFOS_ORIGIN_X, SCORE_ORIGIN_Y, FRAME_INFOS_THICKNESS, SCORE_HEIGHT, FRAME_INFOS_COLOR); // Linha esquerda
    draw_rectangle(LEFT_INFOS_ORIGIN_X+LEFT_INFOS_WIDTH, SCORE_ORIGIN_Y, FRAME_INFOS_THICKNESS, SCORE_HEIGHT+FRAME_INFOS_THICKNESS, FRAME_INFOS_COLOR); // Linha direita

    // Moldura das Lines_Cleaned

    // Moldura dos TopScores

    // Moldura da NextPiece

    // Moldura do Level

        // ISTO ESTAVA FEITO JÁ. Não apaguei para depois pegar na informação e meter dentro das molduras conforme design que tinhamos planeado
    char buffer[64];
    sprintf(buffer, "Score: %u", score->score);
    draw_text(LEFT_INFOS_ORIGIN_X, 200, buffer, 0xFFFFFF, 0x222222);

    sprintf(buffer, "Lines: %u", score->lines_cleared);
    draw_text(LEFT_INFOS_ORIGIN_X, 230, buffer, 0xFFFFFF, 0x222222);

    sprintf(buffer, "Level: %u", score->level);
    draw_text(LEFT_INFOS_ORIGIN_X, 260, buffer, 0xFFFFFF, 0x222222);
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


