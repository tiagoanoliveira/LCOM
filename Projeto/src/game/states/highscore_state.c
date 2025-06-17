#include "include/highscore_state.h"
#include <string.h>

void highscore_state_init(HighscoreState* state, const unsigned int score) {
    if (!state) return;
    state->active = true;
    state->score = score;
    state->name_input_length = 0;
    memset(state->name_input, 0, MAX_NAME_LENGTH);
}

void highscore_state_handle_input(HighscoreState* state, const InputEvent event) {
    if (!state || !state->active) return;
    if (event.action == INPUT_CHAR && event.pressed) {
        char c = event.character;
        if (((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) &&
            state->name_input_length < MAX_NAME_LENGTH - 1) {
            state->name_input[state->name_input_length++] = c;
            state->name_input[state->name_input_length] = '\0';
        } else if (c == '\b' && state->name_input_length > 0) {
            state->name_input[--state->name_input_length] = '\0';
        }
    } else if (event.action == INPUT_ENTER && event.pressed) {
        if (state->name_input_length == 0) {
            strncpy(state->name_input, "???", MAX_NAME_LENGTH);
        }
        try_add_high_score(state->name_input, state->score);
        state->active = false;
    }
}

void highscore_state_update(HighscoreState* state) {
    // Placeholder for future use
    // Currently no update logic is needed for the highscore state.
}

GameStateType highscore_state_get_next(const HighscoreState* state) {
    return (state && !state->active) ? STATE_GAME_OVER : STATE_HIGHSCORE;
}
