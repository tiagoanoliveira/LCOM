#include "include/render.h"
#include "../core/include/config.h"
#include "../objects/include/score.h"

void draw_block(const int col, const int row, const uint32_t color) {
    const int x = grid_origin_x + col * CELL_SIZE;
    const int y = grid_origin_y + row * CELL_SIZE;
    draw_rectangle(x, y, CELL_SIZE, CELL_SIZE, color);
}

void draw_current_piece(const Piece* piece) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (piece->shape[i][j]) {
                const int gx = piece->x + j;
                const int gy = piece->y + i;
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
