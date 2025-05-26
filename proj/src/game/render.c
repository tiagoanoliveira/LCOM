#include "render.h"

void draw_block(int col, int row, uint32_t color) {
    int x = grid_origin_x + col * CELL_SIZE;
    int y = grid_origin_y + row * CELL_SIZE;
    draw_rectangle(x, y, CELL_SIZE, CELL_SIZE, color);
}

int draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (uint16_t i = 0; i < height; i++) {
        for (uint16_t j = 0; j < width; j++) {
        if (vg_draw_pixel(x + j, y + i, color) != 0) return 1;
        }
    }
    return 0;
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
    const int cols = 10;
    const int rows = 20;
    const int cell_width = 30;
    const int cell_height = 30;

    const int grid_width = cols * cell_width;
    const int grid_height = rows * cell_height;

    const int screen_width = 1024;
    const int screen_height = 768;

    // Atualiza variáveis globais
    grid_origin_x = (screen_width - grid_width) / 2;
    grid_origin_y = (screen_height - grid_height) / 2;

    const uint32_t color = 0x3F;

    for (int i = 0; i <= rows; i++) {
        int y = grid_origin_y + i * cell_height;
        draw_rectangle(grid_origin_x, y, grid_width, 1, color);
    }

    for (int j = 0; j <= cols; j++) {
        int x = grid_origin_x + j * cell_width;
        draw_rectangle(x, grid_origin_y, 1, grid_height, color);
    }
}

