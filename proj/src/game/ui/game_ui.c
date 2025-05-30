#include "include/game_ui.h"

#include <stdio.h>
#include <string.h>
#include "../objects/include/score.h"
#include "../objects/include/highscore.h"
#include "../core/include/tetris.h"
#include "include/font.h"

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

void draw_game_infos(const GameLogic* game) {
    draw_score();
    draw_lines();
    draw_highscores();
    draw_level();
    draw_next_piece(game);
}

void draw_score(void) {
    GameScore* score = tetris_get_score();

    // Draw frame
    draw_rectangle(LEFT_INFOS_ORIGIN_X, SCORE_ORIGIN_Y, LEFT_INFOS_WIDTH, SCORE_HEIGHT, FRAME_INFOS_COLOR);
    draw_rectangle(LEFT_INFOS_ORIGIN_X+5, SCORE_ORIGIN_Y+5, LEFT_INFOS_WIDTH-10, SCORE_HEIGHT-10, SCORE_BG_COLOR);

    int score_label_x = LEFT_INFOS_ORIGIN_X + LEFT_INFOS_WIDTH / 2 - strlen("Score") * CHAR_WIDTH / 2;
    draw_text(false, score_label_x, SCORE_ORIGIN_Y + 20, "Score", 0xFFFFFF, SCORE_BG_COLOR);

    char buffer[32];
    sprintf(buffer, "%u", score->score);
    int score_value_x = LEFT_INFOS_ORIGIN_X + LEFT_INFOS_WIDTH / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int score_value_y = SCORE_ORIGIN_Y + CHAR_HEIGHT + 20 + 15; // 20 for label height, 15 for padding
    draw_text(false, score_value_x, score_value_y, buffer, 0xFFFFFF, SCORE_BG_COLOR);
}

void draw_lines(void) {
    GameScore* score = tetris_get_score();

    // Draw frame
    draw_rectangle(LEFT_INFOS_ORIGIN_X, LINES_CLEANED_ORIGIN_Y, LEFT_INFOS_WIDTH, LINES_CLEANED_HEIGHT, FRAME_INFOS_COLOR);
    draw_rectangle(LEFT_INFOS_ORIGIN_X + 5, LINES_CLEANED_ORIGIN_Y + 5, LEFT_INFOS_WIDTH - 10, LINES_CLEANED_HEIGHT - 10, LINES_BG_COLOR);

    int lines_label_x = LEFT_INFOS_ORIGIN_X + LEFT_INFOS_WIDTH / 2 - strlen("Lines") * CHAR_WIDTH / 2;
    draw_text(false, lines_label_x, LINES_CLEANED_ORIGIN_Y + 20, "Lines", 0xFFFFFF, LINES_BG_COLOR);

    char buffer[32];
    sprintf(buffer, "%u", score->lines_cleared);
    int lines_value_x = LEFT_INFOS_ORIGIN_X + LEFT_INFOS_WIDTH / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int lines_value_y = LINES_CLEANED_ORIGIN_Y + CHAR_HEIGHT + 20 + 15; // 20 for label height, 15 for padding
    draw_text(false, lines_value_x, lines_value_y, buffer, 0xFFFFFF, LINES_BG_COLOR);
}

void draw_highscores(void) {
    HighScore scores[MAX_HIGH_SCORES];
    int count;
    load_high_scores(scores, &count);

    // Draw background and frame
    draw_rectangle(LEFT_INFOS_ORIGIN_X, HIGHSCORES_ORIGIN_Y, LEFT_INFOS_WIDTH, HIGHSCORES_HEIGHT, FRAME_INFOS_COLOR);
    draw_rectangle(LEFT_INFOS_ORIGIN_X + 5, HIGHSCORES_ORIGIN_Y + 5, LEFT_INFOS_WIDTH - 10, HIGHSCORES_HEIGHT - 10, HIGHSCORES_BG_COLOR);

    // Draw label, centered at the top of the box
    int label_x = LEFT_INFOS_ORIGIN_X + LEFT_INFOS_WIDTH / 2 - strlen("Top Scores:") * CHAR_WIDTH / 2;
    draw_text(false, label_x, HIGHSCORES_ORIGIN_Y + 20, "Top Scores:", 0xFFFFFF, HIGHSCORES_BG_COLOR);

    // Draw each high score entry
    for (int i = 0; i < count; i++) {
        char buffer[64];
        sprintf(buffer, "%d.%s-%u", i + 1, scores[i].name, scores[i].score);
        int entry_y = HIGHSCORES_ORIGIN_Y + CHAR_HEIGHT + 20 + 15 + i * 32; // consistent spacing
        draw_text(false, LEFT_INFOS_ORIGIN_X + 20, entry_y, buffer, 0xFFFFFF, HIGHSCORES_BG_COLOR);
    }
}

void draw_level(void) {
    GameScore* score = tetris_get_score();

    // Draw frames
    draw_rectangle(RIGHT_INFOS_ORIGIN_X, LEVEL_ORIGIN_Y, RIGHT_INFOS_WIDTH, LEVEL_HEIGHT, FRAME_INFOS_COLOR);
    draw_rectangle(RIGHT_INFOS_ORIGIN_X + 5, LEVEL_ORIGIN_Y + 5, RIGHT_INFOS_WIDTH - 10, LEVEL_HEIGHT - 10, LEVEL_BG_COLOR);

    // Draw label, centered at the top
    int level_label_x = RIGHT_INFOS_ORIGIN_X + RIGHT_INFOS_WIDTH / 2 - strlen("Level") * CHAR_WIDTH / 2;
    draw_text(false, level_label_x, LEVEL_ORIGIN_Y + 20, "Level", 0xFFFFFF, LEVEL_BG_COLOR);

    char buffer[16];
    sprintf(buffer, "%u", score->level);

    int level_value_x = RIGHT_INFOS_ORIGIN_X + RIGHT_INFOS_WIDTH / 2 - strlen(buffer) * CHAR_WIDTH / 2;
    int level_value_y = LEVEL_ORIGIN_Y + CHAR_HEIGHT + 20 + 15; // 20 for label height, 15 for padding
    draw_text(false, level_value_x, level_value_y, buffer, 0xFFFFFF, LEVEL_BG_COLOR);
}

void draw_next_piece(const GameLogic* game) {
    // Draw frame
    draw_rectangle(RIGHT_INFOS_ORIGIN_X-5, NEXT_PIECE_ORIGIN_Y-5, RIGHT_INFOS_WIDTH+10, NEXT_PIECE_HEIGHT+10, FRAME_INFOS_COLOR);
    draw_rectangle(RIGHT_INFOS_ORIGIN_X, NEXT_PIECE_ORIGIN_Y, RIGHT_INFOS_WIDTH, NEXT_PIECE_HEIGHT, NEXT_PIECE_BG_COLOR);

    // Draw label
    int label_x = RIGHT_INFOS_ORIGIN_X + RIGHT_INFOS_WIDTH / 2 - strlen("Next") * CHAR_WIDTH / 2;
    draw_text(false, label_x, NEXT_PIECE_ORIGIN_Y + 20, "Next", 0xFFFFFF, NEXT_PIECE_BG_COLOR);

    if (game && !game->game_over) {
        // Calcular os limites efetivos da peça
        int min_x = PIECE_SIZE, max_x = -1;
        int min_y = PIECE_SIZE, max_y = -1;

        for (int i = 0; i < PIECE_SIZE; i++) {
            for (int j = 0; j < PIECE_SIZE; j++) {
                if (game->next_piece.shape[i][j]) {
                    if (j < min_x) min_x = j;
                    if (j > max_x) max_x = j;
                    if (i < min_y) min_y = i;
                    if (i > max_y) max_y = i;
                }
            }
        }

        // Calcular largura e altura efetivas
        int effective_width = (max_x - min_x + 1) * CELL_SIZE;
        int effective_height = (max_y - min_y + 1) * CELL_SIZE;

        // Centrar baseado nas dimensões efetivas
        int piece_start_x = RIGHT_INFOS_ORIGIN_X + (RIGHT_INFOS_WIDTH - effective_width) / 2 - min_x * CELL_SIZE;
        int piece_start_y = NEXT_PIECE_ORIGIN_Y + CHAR_HEIGHT + 20 + 20 + (NEXT_PIECE_HEIGHT - CHAR_HEIGHT - 60 - effective_height) / 2 - min_y * CELL_SIZE;

        for (int i = 0; i < PIECE_SIZE; i++) {
            for (int j = 0; j < PIECE_SIZE; j++) {
                if (game->next_piece.shape[i][j]) {
                    int x = piece_start_x + j * CELL_SIZE;
                    int y = piece_start_y + i * CELL_SIZE;
                    draw_rectangle(x, y, CELL_SIZE, CELL_SIZE, game->next_piece.color);

                    // Desenhar bordas da célula
                    draw_rectangle(x, y, CELL_SIZE, 1, GRID_COLOR);
                    draw_rectangle(x, y, 1, CELL_SIZE, GRID_COLOR);
                    draw_rectangle(x + CELL_SIZE - 1, y, 1, CELL_SIZE, GRID_COLOR);
                    draw_rectangle(x, y + CELL_SIZE - 1, CELL_SIZE, 1, GRID_COLOR);
                }
            }
        }
    }
}
