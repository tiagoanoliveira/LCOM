#include "include/state.h"
#include "include/menu_state.h"
#include "include/gameover_state.h"
#include "include/instructions_state.h"
#include "../ui/include/menu_ui.h"
#include "../ui/include/gameover_ui.h"
#include "../ui/include/instructions_ui.h"
#include "../core/include/game.h"
#include "../../drivers/graphics/graphics.h"
#include "../core/include/tetris.h"

// Estados
static MenuState menu_state;
static GameLogic game_logic;
static GameOverState gameover_state;
static InstructionsState instructions_state;

static GameStateType current_state = STATE_MENU;
static GameStateType next_state = STATE_MENU;
static bool should_quit = false;

void state_manager_init(void) {
    current_state = STATE_MENU;
    next_state = STATE_MENU;
    needs_redraw = true;
    should_quit = false;
    menu_state_init(&menu_state);
}

void state_manager_set_state(GameStateType type) {
    if (type != current_state) {
        next_state = type;
        needs_redraw = true;

        switch (type) {
            case STATE_MENU:
                menu_state_init(&menu_state);
                break;
            case STATE_GAME:
                game_logic_init(&game_logic);
                break;
            case STATE_GAME_OVER: {
                GameScore* score = tetris_get_score();
                gameover_state_init(&gameover_state, score->score, score->lines_cleared);
                break;
            }
            case STATE_INSTRUCTIONS:  // ADICIONADO
                instructions_state_init(&instructions_state);
                break;
            case STATE_QUIT:
                should_quit = true;
                break;
        }
        current_state = type;
    }
}

void state_manager_handle_input(InputEvent event) {
    switch (current_state) {
        case STATE_MENU:
            menu_state_handle_input(&menu_state, event);
            if (event.action == INPUT_UP || event.action == INPUT_DOWN || event.action == INPUT_ESCAPE) {
                needs_redraw = true;
            }
            if (event.action == INPUT_ENTER && event.pressed) {
                GameStateType action = menu_state_get_selected_action(&menu_state);
                state_manager_set_state(action);
            }
            break;
        case STATE_GAME:
            game_logic_handle_input(&game_logic, event);
            if (event.action != INPUT_NONE) {
                needs_redraw = true;
            }
            if (game_logic_is_game_over(&game_logic)) {
                state_manager_set_state(STATE_GAME_OVER);
            }
            break;
        case STATE_INSTRUCTIONS:
            instructions_state_handle_input(&instructions_state, event);
            if (event.action == INPUT_ENTER || event.action == INPUT_ESCAPE) {
                needs_redraw = true;
                GameStateType action = instructions_state_get_selected_action(&instructions_state);
                state_manager_set_state(action);
            }
            break;
        case STATE_GAME_OVER:
            gameover_state_handle_input(&gameover_state, event);
            if (event.action == INPUT_UP || event.action == INPUT_DOWN || event.action == INPUT_ESCAPE) {
                needs_redraw = true;
            }
            if (event.action == INPUT_ENTER && event.pressed) {
                GameStateType action = gameover_state_get_selected_action(&gameover_state);
                state_manager_set_state(action);
            }
            break;
        case STATE_QUIT:
            break;
    }

}

void state_manager_update(void) {
    switch (current_state) {
        case STATE_MENU:
            menu_state_update(&menu_state);
            break;
        case STATE_GAME:
            game_logic_update(&game_logic);
            if (!game_logic_is_paused(&game_logic)) {  // MODIFICADO
                needs_redraw = true;
            }
            if (game_logic_is_game_over(&game_logic)) {
                state_manager_set_state(STATE_GAME_OVER);
            }
            break;
        case STATE_INSTRUCTIONS:
            instructions_state_update(&instructions_state);
            break;
        case STATE_GAME_OVER:
            gameover_state_update(&gameover_state);
            break;
        case STATE_QUIT:
            break;
    }
}

void state_manager_render(void) {
    if (!needs_redraw) return;

    switch (current_state) {
        case STATE_MENU:
            menu_ui_draw(&menu_state);
            break;
        case STATE_GAME:
            game_logic_render(&game_logic);
            break;
        case STATE_INSTRUCTIONS:  // ADICIONADO
            instructions_ui_draw(&instructions_state);
            break;
        case STATE_GAME_OVER:
            gameover_ui_draw(&gameover_state);
            break;
        case STATE_QUIT:
            break;
    }

    if (current_state != STATE_QUIT) {
        swap_buffers();
    }

    needs_redraw = false;
}

bool state_manager_should_quit(void) {
    return should_quit;
}
