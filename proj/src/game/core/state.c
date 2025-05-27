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
Piece current_piece;
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
                tetris_init(); // Initialize the game state
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
        case STATE_GAME_OVER:
            game_over_menu_handle_input(&gameOverMenu, scancode, two_byte);
            if (scancode[0] == 0x1C) { // Enter
                GameState newState = game_over_menu_get_selected_action(&gameOverMenu);
                if (newState == STATE_GAME) {
                    // Restart game logic
                    tetris_init();

                }
                currentState = newState;
                needsRedraw = true;
            }
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
            draw_current_piece(&current_piece);
            break;
        case STATE_GAME_OVER:
            game_over_menu_draw(&gameOverMenu); // draw game over screen
            swap_buffers();
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
    return &current_piece;
}

bool state_is_game_over(void) {
    return gameOver;
}

void state_set_game_over(bool over) {
    gameOver = over;
    if (over) {
        printf("Game Over! Going to game over menu\n");
        state_set_current(STATE_GAME_OVER); // <-- Go to game over menu!
        game_over_menu_init(&gameOverMenu);
    }
}
