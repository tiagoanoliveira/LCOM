#include "render.h"

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
                // Example: always draw in blue, or you can store colors in the grid if you want
                draw_block(col, row, 0x0000FF);
            }
        }
    }
}

void draw_grid() {
    const uint32_t color = 0x3F; // Color of the grid lines

    // Horizontal lines
    for (int i = 0; i <= GRID_ROWS; i++) {
        int y = grid_origin_y + i * CELL_SIZE;
        draw_rectangle(grid_origin_x, y, GRID_COLS * CELL_SIZE, 1, color);
    }

    // Vertical lines
    for (int j = 0; j <= GRID_COLS; j++) {
        int x = grid_origin_x + j * CELL_SIZE;
        draw_rectangle(x, grid_origin_y, 1, GRID_ROWS * CELL_SIZE, color);
    }
}
