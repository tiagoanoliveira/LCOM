#ifndef GAMEOVER_STATE_H
#define GAMEOVER_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "state.h"

// Game over options
typedef enum {
    GAMEOVER_PLAY_AGAIN,
    GAMEOVER_BACK_TO_MENU,
    GAMEOVER_QUIT,
    GAMEOVER_OPTIONS_COUNT
} GameOverOption;

typedef struct {
    GameOverOption selected;
    bool active;
    int final_score;
    int lines_cleared;
} GameOverState;

// Game over state management
void gameover_state_init(GameOverState* state, int score, int lines);
void gameover_state_handle_input(GameOverState* state, InputEvent event);
void gameover_state_update(GameOverState* state);
GameStateType gameover_state_get_selected_action(const GameOverState* state);

#endif
