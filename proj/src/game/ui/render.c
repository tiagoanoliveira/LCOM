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

void draw_ghost_piece(const Piece* piece) {
    for (int i = 0; i < PIECE_SIZE; i++) {
        for (int j = 0; j < PIECE_SIZE; j++) {
            if (piece->shape[i][j]) {
                const int line_thickness = 2;
                const int gx = piece->x + j;
                const int gy = piece->y + i;
                if (gx < 0 || gx >= GRID_COLS || gy < 0 || gy >= GRID_ROWS) {
                    continue;
                }
                const int pixel_x = grid_origin_x + gx * CELL_SIZE;
                const int pixel_y = grid_origin_y + gy * CELL_SIZE;
                draw_rectangle(pixel_x, pixel_y,CELL_SIZE,line_thickness, piece->color);                                   // Linha superior
                draw_rectangle(pixel_x, pixel_y + CELL_SIZE - line_thickness,CELL_SIZE, line_thickness, piece->color);  // Linha inferior
                draw_rectangle(pixel_x, pixel_y,line_thickness,CELL_SIZE, piece->color);                                   // Linha esquerda
                draw_rectangle(pixel_x + CELL_SIZE - line_thickness, pixel_y,line_thickness,CELL_SIZE, piece->color);   // Linha direita
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
