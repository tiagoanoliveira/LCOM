#ifndef TETRIS_H
#define TETRIS_H

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
#include "config.h"
#include "../ui/render.h"
#include "../objects/score.h"

extern int grid[GRID_ROWS][GRID_COLS];

void tetris_init();
void tetris_draw();
void fix_piece_to_grid(const Piece* piece);
PieceType random_piece_type();
int clear_full_lines();
GameScore* tetris_get_score(void);

#endif // TETRIS_H
