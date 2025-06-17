#ifndef INSTRUCTIONS_STATE_H
#define INSTRUCTIONS_STATE_H

#include <stdbool.h>
#include "state.h"

typedef struct {
    bool active;
} InstructionsState;

// Instructions state management
void instructions_state_init(InstructionsState* state);
void instructions_state_handle_input(InstructionsState* state, InputEvent event);
void instructions_state_update(InstructionsState* state);
GameStateType instructions_state_get_selected_action(const InstructionsState* state);

#endif
