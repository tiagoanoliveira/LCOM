#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include "piece.h"                            // For struct Piece
#include "../drivers/graphics/graphics.h"     // For draw_rectangle and grid coordinates

#define GRID_COLS 10
#define GRID_ROWS 20
#define CELL_SIZE 30

// Draws the Tetris grid
void draw_grid();

// Draws a single block (1 Tetris cell)
void draw_block(int col, int row, uint32_t color);

// Draws the current piece based on its position and shape
void draw_current_piece(const Piece* piece);

// Draws all occupied cells in the grid
void draw_grid_contents(int grid[20][10]);

#endif // RENDER_H
