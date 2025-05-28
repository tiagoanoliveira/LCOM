#include "tetris.h"

int grid[GRID_ROWS][GRID_COLS] = {{0}}; // 0 means empty cell
static GameScore scoreData;

GameScore* tetris_get_score() {
    return &scoreData;
}

void tetris_init() {
    // Clear the grid
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            grid[i][j] = 0;
        }
    }

    extern Piece current_piece;
    piece_init(&current_piece, random_piece_type(), 3, 0);
    scoreInit(&scoreData);
}

void tetris_draw() {
    vg_clear_screen(BACKGROUND_COLOR);
    draw_grid();
}

PieceType random_piece_type() {
    return (PieceType)(rand() % 7);
}

void fix_piece_to_grid(const Piece* piece) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (piece->shape[i][j]) {
                int gx = piece->x + j;
                int gy = piece->y + i;
                if (gx >= 0 && gx < GRID_COLS && gy >= 0 && gy < GRID_ROWS)
                    grid[gy][gx] = piece->color;
            }
        }
    }
    int cleared = clear_full_lines(); // clears full lines and returns the number of lines cleared
    if (cleared > 0) {
        scoreAddLines(&scoreData, cleared); // Updates score
    }
}

int clear_full_lines() {
    int lines_cleared = 0;

    for (int row = GRID_ROWS - 1; row >= 0; row--) {
        bool full = true;
        for (int col = 0; col < GRID_COLS; col++) {
            if (grid[row][col] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            for (int r = row; r > 0; r--) {
                for (int c = 0; c < GRID_COLS; c++) {
                    grid[r][c] = grid[r - 1][c];
                }
            }
            for (int c = 0; c < GRID_COLS; c++) {
                grid[0][c] = 0;
            }
            lines_cleared++;
            row++; // Re-check this row
        }
    }
    return lines_cleared;
}
