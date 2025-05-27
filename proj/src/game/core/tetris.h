#ifndef _TETRIS_H_
#define _TETRIS_H_

#include <lcom/lcf.h>
#include <lcom/vbe.h>

#include <stdint.h>
#include <string.h>
#include <sys/mman.h>       // vm_map_phys
#include <machine/int86.h>  // sys_int86
#include <minix/syslib.h>   // sys_privctl
#include <minix/sysutil.h>  // printf

#include "../../drivers/graphics/graphics.h"
#include "../objects/piece.h"
#include "../objects/grid.h"

extern int grid[GRID_ROWS][GRID_COLS];
extern Piece current_piece;

void game_tick();
void logic_tick();
void tetris_init();
void tetris_draw();
void render_frame();
void handle_keyboard_input();
void fix_piece_to_grid(const Piece* piece);

int clear_full_lines();

PieceType random_piece_type();

#endif /* _TETRIS_H_ */
