#include "include/gameover_state.h"
#include <string.h>  // For memset, strncpy

// Helper: Handle high score name entry
static void handle_name_input(GameOverState* state, InputEvent event) {
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
        try_add_high_score(state->name_input, state->final_score);
        state->substate = GAMEOVER_SUBSTATE_MENU;
    }
}

// Helper: Handle game over menu navigation
static void handle_menu_input(GameOverState* state, InputEvent event) {
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

// Initialize the game over state
void gameover_state_init(GameOverState* state, unsigned int score, int lines) {
    if (!state) return;

    state->selected = GAMEOVER_PLAY_AGAIN;
    state->active = true;
    state->final_score = score;
    state->lines_cleared = lines;

    state->name_input_length = 0;
    memset(state->name_input, 0, MAX_NAME_LENGTH);

    HighScore scores[MAX_HIGH_SCORES];
    int count;
    load_high_scores(scores, &count);

    if (count < MAX_HIGH_SCORES || score > scores[count - 1].score) {
        state->substate = GAMEOVER_SUBSTATE_HIGHSCORE;
    } else {
        state->substate = GAMEOVER_SUBSTATE_MENU;
    }
}

// Handle player input in game over screen
void gameover_state_handle_input(GameOverState* state, InputEvent event) {
    if (!state || !state->active || event.action == INPUT_NONE) return;

    if (state->substate == GAMEOVER_SUBSTATE_HIGHSCORE) {
        handle_name_input(state, event);
    } else {
        handle_menu_input(state, event);
    }
}

// Game over update logic (placeholder for future use)
void gameover_state_update(GameOverState* state) {
    // animações, timers , etc
}

// Get next game state based on player selection
GameStateType gameover_state_get_selected_action(const GameOverState* state) {
    if (!state || !state->active) return -1;

    switch (state->selected) {
        case GAMEOVER_PLAY_AGAIN:
            return STATE_GAME;
        case GAMEOVER_BACK_TO_MENU:
            return STATE_MENU;
        case GAMEOVER_QUIT:
            return STATE_QUIT;
        default:
            return STATE_GAME_OVER;
    }
}
