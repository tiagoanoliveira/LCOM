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

int(proj_main_loop)(int argc, char* argv[]) {
    int col = 4; // começa centrado
    int row = 0;
    int tick_count = 0;
    bool needs_redraw = false;

    uint8_t prev_scancode[2] = {0, 0}; // para evitar múltiplas deteções

    // Inicializa o modo gráfico 0x105 (1024x768, 8bpp)
    if (my_vg_init(0x105) == NULL) {
        printf("Erro ao inicializar o modo gráfico.\n");
        return 1;
    }

    vg_clear_screen(0x00);
    draw_grid();
    draw_piece(4, 0, 0x01); // coluna 4 → centrado (há 10 colunas), linha 0 → topo

    // Desenha um retângulo azul (cor 0x01) no canto superior esquerdo
    //draw_rectangle(100, 100, 200, 150, 0x01);

    // Subscreve interrupções do teclado
    int ipc_status;
    message msg;
    uint8_t irq_kbd;
    if (keyboard_subscribe_int(&irq_kbd)) {
        printf("Erro ao subscrever teclado.\n");
        vg_exit();
        return 1;
    }

    uint8_t irq_timer;
    if (timer_subscribe_int(&irq_timer)) {
        printf("Erro ao subscrever timer\n");
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

                        // Evitar processar o mesmo scancode repetidamente
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
                                case 0x4B: if (col > 0) { col--; needs_redraw = true; } break;      // ←
                                case 0x4D: if (col < 8) { col++; needs_redraw = true; } break;      // → (máx = 8 porque peça O tem 2 colunas)
                                case 0x50: if (row < 18) { row++; needs_redraw = true; } break;     // ↓ (máx = 18 para peça caber)
                            }
                        }
                    }

                    if (msg.m_notify.interrupts & irq_timer) {
                        tick_count++;
                        if (tick_count >= 60) { // a cada segundo
                            tick_count = 0;
                            if (row < 18) {
                                row++;      // move para baixo
                                needs_redraw = true;
                            }
                        }
                    }

                    if (needs_redraw) {
                        vg_clear_screen(0x00);          // limpa o frame_buffer
                        draw_grid();                    // desenha grelha no frame_buffer
                        draw_piece(col, row, 0x01);     // desenha peça no frame_buffer
                        swap_buffers();                 // copia para video_mem tudo de uma vez
                        needs_redraw = false;
                    }

                    break;
            }
        }
    }

    // Limpar variáveis globais (opcional mas seguro)
    scancode[0] = 0;
    scancode[1] = 0;
    two_byte = false;

    // Libertar teclado
    if (keyboard_unsubscribe_int() != 0)
        printf("Erro ao cancelar subscrição do teclado\n");

    if (timer_unsubscribe_int() != 0)
        printf("Erro ao cancelar subscrição do timer\n");

    // Voltar ao modo texto
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
