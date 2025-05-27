//
// Created by tiago-oliveira on 27-05-2025.
//

#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>
#include "../objects/piece.h"
#include "state.h"
#include "tetris.h"
#include "config.h"

// Inicialização e cleanup do jogo
void game_init(void);
void game_cleanup(void);

// Handlers de input e update
void game_handle_input(uint8_t scancode[2], bool twoByte);
void game_update(void);

// Gestão de peças
void game_spawn_new_piece(void);
bool game_move_piece(int deltaX, int deltaY);
void game_rotate_piece(bool clockwise);
void game_drop_piece(void);

#endif

