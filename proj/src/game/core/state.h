#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "../objects/piece.h"

typedef enum {
    STATE_MENU,
    STATE_GAME,
    STATE_GAME_OVER,
    STATE_QUIT
} GameState;

// Inicialização e gestão de estados
void state_init(void);
GameState state_get_current(void);
void state_set_current(GameState newState);

// Controlo de renderização
bool state_needs_redraw(void);
void state_set_redraw(bool redraw);

// Handlers principais
void state_handle_input(uint8_t scancode[2], bool twoByte);
void state_update(void);
void state_render(void);

// Acesso a dados do jogo
Piece* state_get_current_piece(void);
bool state_is_game_over(void);
void state_set_game_over(bool over);

#endif
