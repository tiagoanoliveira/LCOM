#include "state.h"
#include "game.h"
#include "../ui/menu.h"
#include "../ui/render.h"
#include "tetris.h"
#include "../../drivers/graphics/graphics.h"
#include "../../drivers/keyboardMouse/keyboard.h"
#include "../objects/piece.h"
#include "config.h"

static GameState currentState = STATE_MENU;
static bool needsRedraw = true;
static Piece currentPiece;
static bool gameOver = false;

void state_init(void) {
    currentState = STATE_MENU;
    menu_init(&mainMenu);
    tetris_init();
    needsRedraw = true;
}

GameState state_get_current(void) {
    return currentState;
}

void state_set_current(GameState newState) {
    if (newState != currentState) {
        currentState = newState;
        needsRedraw = true;

        switch (newState) {
            case STATE_GAME:
                piece_init(&currentPiece, random_piece_type(), 3, 0);
                gameOver = false;
                break;
            case STATE_MENU:
                menu_init(&mainMenu);
                break;
            default:
                break;
        }
    }
}

bool state_needs_redraw(void) {
    return needsRedraw;
}

void state_set_redraw(bool redraw) {
    needsRedraw = redraw;
}

void state_handle_input(uint8_t scancode[2], bool two_byte) {
    switch (currentState) {
        case STATE_MENU:
            menu_handle_input(&mainMenu, scancode, two_byte);

            if (scancode[0] == 0x1C) { // Enter make code
                GameState newState = menu_get_selected_action(&mainMenu);
                state_set_current(newState);
            }
            break;

        case STATE_GAME:
            game_handle_input(scancode, two_byte);
            break;

        case STATE_QUIT:
            // Não precisa de input
            break;
    }
}

void state_update(void) {
    switch (currentState) {
        case STATE_GAME:
            game_update();
            break;
        default:
            break;
    }
}

void state_render(void) {
    if (!needsRedraw) return;

    switch (currentState) {
        case STATE_MENU:
            menu_draw(&mainMenu);
            break;

        case STATE_GAME:
            vg_clear_screen(BACKGROUND_COLOR);
            draw_grid();
            draw_grid_contents(grid);
            draw_current_piece(&currentPiece);
            break;

        case STATE_QUIT:
            // Não precisa renderizar
            break;
    }

    if (currentState != STATE_QUIT) {
        swap_buffers();
    }
    needsRedraw = false;
}

Piece* state_get_current_piece(void) {
    return &currentPiece;
}

bool state_is_game_over(void) {
    return gameOver;
}

void state_set_game_over(bool over) {
    gameOver = over;
    if (over) {
        printf("Game Over! Return to menu\n");
        state_set_current(STATE_MENU);
    }
}
