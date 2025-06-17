#include "include/menu_state.h"

void menu_state_init(MenuState* state) {
    if (!state) return;

    state->selected = MENU_PLAY;
    state->active = true;
}

void menu_state_handle_input(MenuState* state, const InputEvent event) {
    if (!state || !state->active || event.action == INPUT_NONE) return;

    switch (event.action) {
        case INPUT_UP:
            if (event.pressed) {
                state->selected = (state->selected == 0) ?
                    MENU_OPTIONS_COUNT - 1 : state->selected - 1;
            }
            break;
        case INPUT_DOWN:
            if (event.pressed) {
                state->selected = (state->selected + 1) % MENU_OPTIONS_COUNT;
            }
            break;
        case INPUT_ESCAPE:
            if (event.pressed) {
                state->selected = MENU_QUIT;
            }
            break;

        default:
            break;
    }
}

void menu_state_update(MenuState* state) {
    // depois podemos meter aqui animações
}

GameStateType menu_state_get_selected_action(const MenuState* state) {
    if (!state || !state->active) return -1;

    switch (state->selected) {
        case MENU_PLAY:
            return STATE_GAME;
        case MENU_INSTRUCTIONS:
            return STATE_INSTRUCTIONS; // Por agora volta ao menu
        case MENU_QUIT:
            return STATE_QUIT;
        default:
            return STATE_MENU;
    }
}
