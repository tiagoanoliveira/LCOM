/**
 * \mainpage Tetr-i/o-s : A Tetris-inspired game using MINIX and low-level device drivers
 *
 * This project demonstrates the use of timer, keyboard, mouse, and graphics drivers
 * to implement a classic game with an event-driven architecture.
 *
 * Developed for the LCOM course at FEUP.
 */

#include <lcom/lcf.h>
#include "drivers/timer/timer.h"
#include "drivers/keyboardMouse/keyboard.h"
#include "drivers/keyboardMouse/mouse.h"
#include "drivers/graphics/graphics.h"
#include "game/tetris.h"
#include "game/menu.h"
#include "utils/utils.h"

static bool running = true;

int main(int argc, char *argv[]) {
    // LCF setup
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");
    if (lcf_start(argc, argv)) return 1;

    // Device and game initialization
    if (graphics_init() != 0) return 1;
    if (timer_init() != 0) return 1;
    if (keyboard_init() != 0) return 1;
    if (mouse_init() != 0) return 1;
    tetris_init();

    // Main event loop (skeleton)
    while (running) {
        // TODO: Wait for and process events (timer, keyboard, mouse)
        // Example:
        //   - timer: move piece down
        //   - keyboard: move/rotate/drop piece
        //   - mouse: rotate/hard drop/menu

        // Render game state
        tetris_draw();

        // TODO: Check for exit condition (e.g., ESC key or game over)
    }

    // Cleanup
    graphics_exit();
    timer_exit();
    keyboard_exit();
    mouse_exit();

    lcf_cleanup();
    return 0;
}