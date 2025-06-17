#ifndef MENU_STATE_H
#define MENU_STATE_H

#include <stdbool.h>
#include "state.h"

typedef enum {
    MENU_PLAY,
    MENU_INSTRUCTIONS,
    MENU_QUIT,
    MENU_OPTIONS_COUNT
} MenuOption;

typedef struct {
    MenuOption selected;
    bool active;
} MenuState;

// Menu state management
void menu_state_init(MenuState* state);
void menu_state_handle_input(MenuState* state, InputEvent event);
void menu_state_update(MenuState* state);
GameStateType menu_state_get_selected_action(const MenuState* state);

#endif
