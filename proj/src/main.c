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
#include "game/tetris.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/piece.h"

Piece current_piece; // Global current piece

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

    piece_init(&current_piece, PIECE_O, 4, 0); // Initializes piece O in the middle

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

                        if (scancode[0] == 0x81) {
                            running = false; // ESC
                            break;
                        }

                        if (scancode[0] == 0xE0) {
                            switch (scancode[1]) {
                                    case 0x4B: // ←
                                        if (piece_fits(&current_piece, current_piece.x - 1, current_piece.y)) {
                                            current_piece.x--;
                                            needs_redraw = true;
                                        }
                                        break;

                                    case 0x4D: // →
                                        if (piece_fits(&current_piece, current_piece.x + 1, current_piece.y)) {
                                            current_piece.x++;
                                            needs_redraw = true;
                                        }
                                        break;

                                    case 0x50: // ↓
                                        if (piece_fits(&current_piece, current_piece.x, current_piece.y + 1)) {
                                            current_piece.y++;
                                            needs_redraw = true;
                                        }
                                        break;
                            }
                        }
                    }

                    if (msg.m_notify.interrupts & irq_timer) {
                        tick_count++;
                        if (tick_count >= 60) { // each second
                            tick_count = 0;
                            if (piece_fits(&current_piece, current_piece.x, current_piece.y + 1)) {
                                current_piece.y++;
                            } else {
                                fix_piece_to_grid(&current_piece);
                                piece_init(&current_piece, random_piece_type(), 4, 0);
                            }
                            needs_redraw = true;
                        }
                    }

                    if (needs_redraw) {
                        vg_clear_screen(0x00);                  // clear frame_buffer
                        draw_grid();                            // draw grid on frame_buffer
                        draw_grid_contents(grid);               // draw grid contents on frame_buffer
                        draw_current_piece(&current_piece);     // draw piece on frame_buffer
                        swap_buffers();                         // copy everything to video_mem at once
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
