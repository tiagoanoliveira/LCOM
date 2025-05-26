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

#include "../drivers/graphics/graphics.h"

void tetris_init();
void tetris_draw();

#endif // TETRIS_H
