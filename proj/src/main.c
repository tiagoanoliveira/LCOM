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
#include "drivers/graphics/graphics.h"
#include "drivers/timer/timer.h"
#include "game/states/include/state.h"
#include "game/core/include/input.h"
#include "game/core/include/config.h"

int (proj_main_loop)(int argc, char *argv[]) {
    // Inicialização dos drivers
    if (my_vg_init(SCREEN_MODE) == NULL) {
        printf("Error initializing graphics mode.\n");
        return 1;
    }

    vg_set_grid_position(10, 20, 30, 30);

    // Subscrições de interrupções
    uint8_t irq_kbd, irq_timer;
    if (keyboard_subscribe_int(&irq_kbd) ||
        timer_subscribe_int(&irq_timer)) {
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
                    // Processar interrupção do teclado
                    if (msg.m_notify.interrupts & irq_kbd) {
                        kbc_ih();
                        InputEvent event = input_process_scancode(scancode, two_byte);
                        if (event.action != INPUT_NONE) {
                            state_manager_handle_input(event);
                            needs_redraw = true;
                        }
                    }

                    // Processar interrupção do timer
                    if (msg.m_notify.interrupts & irq_timer) {
                        tick_count++;
                        if (tick_count >= 1) { // 60 FPS
                            tick_count = 0;
                            state_manager_update();
                            needs_redraw = true;
                        }
                    }
                    break;
            }
        }
        // Renderizar APENAS quando necessário
        if (needs_redraw) {
            state_manager_render();
            needs_redraw = false;
        }
    }

    // Cleanup
    keyboard_unsubscribe_int();
    timer_unsubscribe_int();
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
