#ifndef _RENDER_H_
#define _RENDER_H_

#include <stdint.h>
#include "../objects/grid.h"                    // For GRID_COLS, GRID_ROWS, CELL_SIZE
#include "../objects/piece.h"                   // For struct Piece
#include "../../drivers/graphics/graphics.h"    // For draw_rectangle and grid coordinates

// Draws the Tetris grid
void draw_grid();

// Draws a single block (1 Tetris cell)
void draw_block(int col, int row, uint32_t color);

// Draws the current piece based on its position and shape
void draw_current_piece(const Piece* piece);

// Draws all occupied cells in the grid
void draw_grid_contents(int grid[20][10]);

#endif /* _RENDER_H_ */
