/**
 * \mainpage Tetr-i/o-s : A Tetris-inspired game using MINIX and low-level device drivers
 *
 * This project demonstrates the use of timer, keyboard, mouse, and graphics drivers
 * to implement a classic game with an event-driven architecture.
 *
 * Developed for the LCOM course at FEUP.
 */

#include <stdio.h>
#include <lcom/lcf.h>
#include "drivers/timer/timer.h"
#include "drivers/graphics/graphics.h"
#include "drivers/keyboardMouse/mouse.h"
#include "drivers/keyboardMouse/keyboard.h"
#include "game/core/include/game.h"
#include "game/core/include/input.h"
#include "game/core/include/config.h"
#include "game/states/include/state.h"
#include "drivers/keyboardMouse/i8042.h"

int (proj_main_loop)(int argc, char *argv[]) {
    // Inicialização dos drivers
    if (my_vg_init(SCREEN_MODE) == NULL) {
        printf("Error initializing graphics mode.\n");
        return 1;
    }

    vg_set_grid_position(10, 20, 30, 30);

    // Subscrições de interrupções
    uint8_t irq_kbd, irq_timer, irq_mouse;
    if (keyboard_subscribe_int(&irq_kbd) ||
        timer_subscribe_int(&irq_timer) ||
        mouse_subscribe_int(&irq_mouse)) {
        vg_exit();
        return 1;
    }

    if (mouse_write(MOUSE_ENABLE_DATA_REPORTING) != 0) {
        printf("Error enabling mouse data reporting!\n");
        vg_exit();
        return 1;
    }

    state_manager_init();
    input_init();

    int ipc_status;
    message msg;
    bool running = true;
    int tick_count = 0;

    while (running && !state_manager_should_quit()) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            continue;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    // Timer interruption
                    if (msg.m_notify.interrupts & irq_timer) {
                        tick_count++;
                        if (tick_count >= 1) { // 60 FPS
                            tick_count = 0;
                            state_manager_update();
                        }
                    }

                    // Keyboard interruption
                    if (msg.m_notify.interrupts & irq_kbd) {
                        kbc_ih();
                        InputEvent event = input_process_scancode(scancode, two_byte);
                        if (event.action != INPUT_NONE) {
                            state_manager_handle_input(event);
                            needs_redraw = true;
                        }
                    }

                    // Mouse interruption
                    if (msg.m_notify.interrupts & irq_mouse) {
                        mouse_ih(); // Updates current_byte
                        mouse_sync_bytes();

                        if (packet_byte_index == 3) {
                            parse_packet();

                            uint8_t raw_packet[3];
                            memcpy(raw_packet, mouse_packet.bytes, 3);

                            InputEvent event = input_process_mouse_packet(raw_packet);

                            if (event.action != INPUT_NONE) {
                                if (mouse_action_cooldown == 0) {
                                    state_manager_handle_input(event);
                                    needs_redraw = true;
                                    if (event.type == INPUT_TYPE_MOUSE) {
                                        mouse_action_cooldown = MAX_MOUSE_TICKS;
                                    }
                                }
                            }

                            packet_byte_index = 0;
                        }
                    }
                    break;

            }
        }
        state_manager_render();
    }

    // Cleanup
    timer_unsubscribe_int();
    keyboard_unsubscribe_int();
    mouse_unsubscribe_int();
    mouse_disable_data_reporting();
    vg_exit();

    return 0;
}

int main(int argc, char *argv[]) {
    // LCF setup
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv))
        return 1;

    lcf_cleanup();
    return 0;
}
