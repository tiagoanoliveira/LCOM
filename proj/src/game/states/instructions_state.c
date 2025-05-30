#include "include/instructions_state.h"

#include "../core/include/input.h"

void instructions_state_init(InstructionsState* state) {
    if (!state) return;

    state->active = true;
}

void instructions_state_handle_input(InstructionsState* state, InputEvent event) {
    if (!state || !state->active) return;
    if (!event.pressed) return;
    if (event.action == INPUT_ENTER || event.action == INPUT_ESCAPE) {
        // Voltar ao menu
        state->active = false;
    }
}

void instructions_state_update(InstructionsState* state) {
    if (!state) return;
}

GameStateType instructions_state_get_selected_action(const InstructionsState* state) {
    if (!state || state->active) return STATE_INSTRUCTIONS;
    return STATE_MENU;
}
