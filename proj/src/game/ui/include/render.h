#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include "../../objects/include/piece.h"

// Draws the Tetris grid
void draw_grid();

// Draws the backgroud of Tetris grid
void draw_grid_background();

// Draws a single block (1 Tetris cell)
void draw_block(int col, int row, uint32_t color);

// Draws the current piece based on its position and shape
void draw_current_piece(const Piece* piece);

// Draws all occupied cells in the grid
void draw_grid_contents(int grid[20][10]);

#endif // RENDER_H
