#ifndef _TETRIS_H_
#define _TETRIS_H_

#include "../../objects/include/piece.h"
#include "config.h"
#include "../../objects/include/score.h"

extern int grid[GRID_ROWS][GRID_COLS];

void animate_line_clear(int row);
int clear_full_lines();
void tetris_init();
void tetris_draw();
void fix_piece_to_grid(const Piece* piece);
PieceType random_piece_type();
GameScore* tetris_get_score(void);

#endif // _TETRIS_H_
