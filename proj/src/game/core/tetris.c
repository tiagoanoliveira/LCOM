#include "tetris.h"
#include "../ui/render.h"

int grid[GRID_ROWS][GRID_COLS] = {{0}}; // 0 means empty cell

void tetris_init() {
    // Future initialization: grid, active piece, score...
}

void tetris_draw() {
    vg_clear_screen(0x00); // black background
    draw_grid();           // Tetris grid
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
}
