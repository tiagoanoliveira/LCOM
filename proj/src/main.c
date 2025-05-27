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
    //GameState currentState = STATE_MENU;
    menu_init(&mainMenu);

    // Initialize game components
    tetris_init();

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
    bool needs_redraw = false;

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

                        if (scancode[0] == 0x81) {
                            running = false; // ESC
                            break;
                        }

                        handle_keyboard_input();
                        needs_redraw = true; // Redraw after handling input
                    }

                    if (msg.m_notify.interrupts & irq_timer) {
                        game_tick();
                        needs_redraw = true;
                    }


                    // Render based on current state
                    if (needs_redraw) {
                        render_frame();
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
