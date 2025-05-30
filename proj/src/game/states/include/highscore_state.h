#ifndef HIGHSCORE_STATE_H
#define HIGHSCORE_STATE_H

#include <stdbool.h>
#include "state.h"
#include "../../objects/include/highscore.h"

typedef struct {
    bool active;
    unsigned int score;
    char name_input[MAX_NAME_LENGTH];
    int name_input_length;
} HighscoreState;

void highscore_state_init(HighscoreState* state, unsigned int score);
void highscore_state_handle_input(HighscoreState* state, InputEvent event);
void highscore_state_update(HighscoreState* state);
GameStateType highscore_state_get_next(const HighscoreState* state);

#endif
