#include "include/gameover_state.h"
#include <string.h>  // For memset

// Initialize the game over state
void gameover_state_init(GameOverState* state, const unsigned int score, const int lines) {
    if (!state) return;

    state->selected = GAMEOVER_PLAY_AGAIN;
    state->active = true;
    state->final_score = score;
    state->lines_cleared = lines;

    state->name_input_length = 0;
    memset(state->name_input, 0, MAX_NAME_LENGTH);
}

// Handle game over menu navigation input
void gameover_state_handle_input(GameOverState* state, const InputEvent event) {
    if (!state || !state->active || event.action == INPUT_NONE) return;

    switch (event.action) {
        case INPUT_UP:
            if (event.pressed) {
                state->selected = (state->selected == 0)
                    ? GAMEOVER_OPTIONS_COUNT - 1
                    : state->selected - 1;
            }
            break;

        case INPUT_DOWN:
            if (event.pressed) {
                state->selected = (state->selected + 1) % GAMEOVER_OPTIONS_COUNT;
            }
            break;

        case INPUT_ESCAPE:
            if (event.pressed) {
                state->selected = GAMEOVER_BACK_TO_MENU;
            }
            break;

        default:
            break;
    }
}

// Placeholder for future animation/timer logic
void gameover_state_update(GameOverState* state) {
    // animations, timers, etc.
}

// Return the selected game state based on user choice
GameStateType gameover_state_get_selected_action(const GameOverState* state) {
    if (!state || !state->active) return STATE_GAME_OVER;

    switch (state->selected) {
        case GAMEOVER_PLAY_AGAIN: return STATE_GAME;
        case GAMEOVER_BACK_TO_MENU: return STATE_MENU;
        case GAMEOVER_QUIT: return STATE_QUIT;
        default: return STATE_GAME_OVER;
    }
}
