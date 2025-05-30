#ifndef TETRIS_H
#define TETRIS_H

#include "../../objects/include/piece.h"
#include "config.h"
#include "../../objects/include/score.h"

extern int grid[GRID_ROWS][GRID_COLS];

void tetris_init();
void tetris_draw();
void fix_piece_to_grid(const Piece* piece);
PieceType random_piece_type();
int clear_full_lines();
GameScore* tetris_get_score(void);

#endif // TETRIS_H
