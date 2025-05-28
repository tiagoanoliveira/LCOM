#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include "../core/config.h"
#include "../objects/piece.h"
#include "../objects/score.h"
#include "../core/tetris.h"
#include "../../drivers/graphics/graphics.h"
#include "font.h"

// Draws the Tetris grid
void draw_grid();

// Draws a single block (1 Tetris cell)
void draw_block(int col, int row, uint32_t color);

// Draws the current piece based on its position and shape
void draw_current_piece(const Piece* piece);

// Draws all occupied cells in the grid
void draw_grid_contents(int grid[20][10]);

// Draws the score, lines, and level information
void draw_score_info(void);

#endif // RENDER_H
