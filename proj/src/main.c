/**
 * \mainpage Tetr-i/o-s : A Tetris-inspired game using MINIX and low-level device drivers
 *
 * This project demonstrates the use of timer, keyboard, mouse, and graphics drivers
 * to implement a classic game with an event-driven architecture.
 *
 * Developed for the LCOM course at FEUP.
 */

#include <lcom/lcf.h>
#include "drivers/keyboardMouse/keyboard.h"
#include "drivers/keyboardMouse/mouse.h"
#include "drivers/graphics/graphics.h"
#include "drivers/timer/timer.h"
#include "drivers/utils/utils.h"
#include "game/core/tetris.h"
#include "game/ui/menu.h"
#include "game/ui/render.h"
#include "game/objects/piece.h"
#include "game/core/state.h"

Piece current_piece; // Global current piece
bool game_over = false; // Global game over flag

int(proj_main_loop)(int argc, char* argv[]) {
    int tick_count = 0;
    bool needs_redraw = false;

    int wait = 0;
    while (timer_get_ticks() == 0 && wait++ < 100000000); // waits for the first tick
    srand(timer_get_ticks());

    uint8_t prev_scancode[2] = {0, 0}; // to avoid multiple detections

    // Initializes graphics mode 0x105 (1024x768, 8bpp)
    if (my_vg_init(0x105) == NULL) {
        printf("Error initializing graphics mode.\n");
        return 1;
    }

    vg_set_grid_position(10, 20, 30, 30); // Repositions grid to center

    // Initialize game state and menu
    GameState currentState = STATE_MENU;
    menu_init(&mainMenu);

    // Initialize game components
    tetris_init();

    needs_redraw = true;

    // Subscribes keyboard interrupts
    int ipc_status;
    message msg;
    uint8_t irq_kbd;
    if (keyboard_subscribe_int(&irq_kbd)) {
        printf("Error subscribing keyboard.\n");
        vg_exit();
        return 1;
    }

    uint8_t irq_timer;
    if (timer_subscribe_int(&irq_timer)) {
        printf("Error subscribing timer\n");
        keyboard_unsubscribe_int();
        vg_exit();
        return 1;
    }

    bool running = true;

    while (running) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) continue;
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_kbd) {
                        kbc_ih();

                        // Avoid processing the same scancode repeatedly
                        if (prev_scancode[0] == scancode[0] && prev_scancode[1] == scancode[1])
                            break;

                        prev_scancode[0] = scancode[0];
                        prev_scancode[1] = scancode[1];
                        // Handle input based on current state
                        switch (currentState) {
                            case STATE_MENU:
                                menu_handle_input(&mainMenu, scancode, two_byte);

                                // Check if Enter was pressed to select menu option
                                if (scancode[0] == 0x1C) { // Enter make code
                                    GameState newState = menu_get_selected_action(&mainMenu);
                                    if (newState != currentState) {
                                        currentState = newState;
                                        if (currentState == STATE_GAME) {
                                            // Initialize game when starting
                                            piece_init(&current_piece, PIECE_O, 3, 0);
                                        }
                                    }
                                }
                                break;

                            case STATE_GAME:
                                // Handle game input
                                if (scancode[0] == 0x81) { // ESC - return to menu
                                    currentState = STATE_MENU;
                                    needs_redraw = true;
                                    break;
                                }

                                if (scancode[0] == 0xE0) { // Two-byte scancode
                                    switch (scancode[1]) {
                                        case 0x4B: // Left arrow
                                            if (piece_fits(&current_piece, current_piece.x - 1, current_piece.y)) {
                                                current_piece.x--;
                                                needs_redraw = true;
                                            }
                                            break;
                                        case 0x4D: // Right arrow
                                            if (piece_fits(&current_piece, current_piece.x + 1, current_piece.y)) {
                                                current_piece.x++;
                                                needs_redraw = true;
                                            }
                                            break;
                                        case 0x50: // Down arrow
                                            if (piece_fits(&current_piece, current_piece.x, current_piece.y + 1)) {
                                                current_piece.y++;
                                                needs_redraw = true;
                                            }
                                            break;
                                    }
                                }
                                break;

                            case STATE_QUIT:
                                running = false;
                                break;
                        }

                        needs_redraw = true;
                    }

                    if (msg.m_notify.interrupts & irq_timer) {
                        tick_count++;
                        if (currentState == STATE_GAME && tick_count >= 60) { // every second
                            tick_count = 0;

                            if (piece_fits(&current_piece, current_piece.x, current_piece.y + 1)) {
                                current_piece.y++;
                            } else {
                                fix_piece_to_grid(&current_piece);

                                // Attempt to spawn a new piece
                                Piece new_piece;
                                piece_init(&new_piece, random_piece_type(), 3, 0);

                                if (!piece_fits(&new_piece, new_piece.x, new_piece.y)) {
                                    // Game Over: new piece doesn't fit
                                    game_over = true;
                                    printf("Game Over!\n");

                                    // Return to menu
                                    currentState = STATE_MENU;
                                    menu_init(&mainMenu);
                                } else {
                                    current_piece = new_piece;
                                }
                            }

                            needs_redraw = true;
                        }
                    }


                    // Render based on current state
                    if (needs_redraw) {
                        switch (currentState) {
                            case STATE_MENU:
                                menu_draw(&mainMenu);
                                swap_buffers();
                                break;

                            case STATE_GAME:
                                vg_clear_screen(0x00); // clear framebuffer
                                draw_grid(); // draw grid on framebuffer
                                draw_grid_contents(grid); // draw grid contents on framebuffer
                                draw_current_piece(&current_piece); // draw piece on framebuffer
                                swap_buffers();
                                break;

                            case STATE_QUIT:
                                // No need to draw anything
                                break;
                        }
                        swap_buffers(); // copy everything to videomem at once
                        needs_redraw = false;
                    }
                    break;
            }
        }
    }

    // Clear global variables (optional but safe)
    scancode[0] = 0;
    scancode[1] = 0;
    two_byte = false;

    // Free keyboard and timer subscriptions
    if (keyboard_unsubscribe_int() != 0)
        printf("Error unsubscribing keyboard\n");

    if (timer_unsubscribe_int() != 0)
        printf("Error unsubscribing timer\n");

    // Return to text mode
    vg_exit();

    if (frame_buffer != NULL) {
        free(frame_buffer);
        frame_buffer = NULL;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // LCF setup
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");
    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();
    return 0;
}
