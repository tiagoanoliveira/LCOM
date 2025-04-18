#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "i8042.h"
#include "timer.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/**
 * @brief Função de teste para receber pacotes do mouse via interrupções
 *
 * @param cnt Número de pacotes a receber antes de terminar
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int (mouse_test_packet)(uint32_t cnt) {
    uint8_t bit_no = 0;
    int ipc_status;
    message msg;
    uint32_t packets_received = 0;

    // Subscrevendo as interrupções do mouse
    if (mouse_subscribe_int(&bit_no) != 0) {
        printf("Erro ao subscrever interrupções do mouse\n");
        return 1;
    }

    // Habilitando o envio de dados pelo mouse
    if (mouse_enable_data_reporting() != 0) {
        printf("Erro ao habilitar data reporting do mouse\n");
        mouse_unsubscribe_int();
        return 1;
    }

    // Criando uma estrutura packet para armazenar os dados do pacote
    struct packet pp;

    // Loop principal para receber os pacotes
    while (packets_received < cnt) {
        // Receber uma mensagem (síncrona ou assíncrona)
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("Erro ao receber driver_receive\n");
            continue;
        }

        // Se não for uma notificação, ignorar
        if (is_ipc_notify(ipc_status)) {
            // Se for uma notificação do tipo HARDWARE
            if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
                // Se for uma notificação do mouse
                if (msg.m_notify.interrupts & bit_no) {
                    // Processar a interrupção do mouse
                    mouse_ih();

                    // Verificar se temos um pacote completo
                    extern bool packet_ready;

                    if (packet_ready) {
                        // Analisar o pacote e imprimi-lo
                        mouse_parse_packet(&pp);
                        mouse_print_packet(&pp);

                        packets_received++;
                        packet_ready = false;
                    }
                }
            }
        }
    }

    // Desabilitando o envio de dados pelo mouse
    if (mouse_disable_data_reporting() != 0) {
        printf("Erro ao desabilitar data reporting do mouse\n");
        mouse_unsubscribe_int();
        return 1;
    }

    // Cancelando a subscrição das interrupções do mouse
    if (mouse_unsubscribe_int() != 0) {
        printf("Erro ao cancelar subscrição das interrupções do mouse\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Função de teste para receber pacotes do mouse via interrupções com timeout
 *
 * @param idle_time Tempo de inatividade (em segundos) antes de terminar
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int (mouse_test_async)(uint8_t idle_time) {
    uint8_t bit_no_mouse = 0;
    uint8_t bit_no_timer = 0;
    int ipc_status;
    message msg;

    // Subscrevendo as interrupções do mouse
    if (mouse_subscribe_int(&bit_no_mouse) != 0) {
        printf("Erro ao subscrever interrupções do mouse\n");
        return 1;
    }

    // Subscrevendo as interrupções do timer
    if (timer_subscribe_int(&bit_no_timer) != 0) {
        printf("Erro ao subscrever interrupções do timer\n");
        mouse_unsubscribe_int();
        return 1;
    }

    // Habilitando o envio de dados pelo mouse
    if (mouse_enable_data_reporting() != 0) {
        printf("Erro ao habilitar data reporting do mouse\n");
        timer_unsubscribe_int();
        mouse_unsubscribe_int();
        return 1;
    }

    // Criando uma estrutura packet para armazenar os dados do pacote
    struct packet pp;

    // Variáveis para controle de tempo
    extern int counter;  // Variável definida em timer.c
    counter = 0;
    uint32_t idle_timer = 0;
    uint32_t ticks_per_second = sys_hz();  // Número de interrupções do timer por segundo

    // Loop principal para receber os pacotes
    while (idle_timer < idle_time * ticks_per_second) {
        // Receber uma mensagem (síncrona ou assíncrona)
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("Erro ao receber driver_receive\n");
            continue;
        }

        // Se não for uma notificação, ignorar
        if (is_ipc_notify(ipc_status)) {
            // Se for uma notificação do tipo HARDWARE
            if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
                // Se for uma notificação do timer
                if (msg.m_notify.interrupts & bit_no_timer) {
                    timer_int_handler();
                    idle_timer++;  // Incrementar o contador de tempo ocioso
                }

                // Se for uma notificação do mouse
                if (msg.m_notify.interrupts & bit_no_mouse) {
                    // Processar a interrupção do mouse
                    mouse_ih();

                    // Verificar se temos um pacote completo
                    extern bool packet_ready;

                    if (packet_ready) {
                        // Analisar o pacote e imprimi-lo
                        mouse_parse_packet(&pp);
                        mouse_print_packet(&pp);

                        // Resetar o contador de tempo ocioso
                        idle_timer = 0;
                        packet_ready = false;
                    }
                }
            }
        }
    }

    // Desabilitando o envio de dados pelo mouse
    if (mouse_disable_data_reporting() != 0) {
        printf("Erro ao desabilitar data reporting do mouse\n");
        timer_unsubscribe_int();
        mouse_unsubscribe_int();
        return 1;
    }

    // Cancelando as subscrições das interrupções
    if (timer_unsubscribe_int() != 0) {
        printf("Erro ao cancelar subscrição das interrupções do timer\n");
        mouse_unsubscribe_int();
        return 1;
    }

    if (mouse_unsubscribe_int() != 0) {
        printf("Erro ao cancelar subscrição das interrupções do mouse\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Função de teste para detectar um gesto do mouse (V invertido)
 *
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int (mouse_test_gesture)() {
    uint8_t bit_no = 0;
    int ipc_status;
    message msg;
    bool gesture_detected = false;

    // Valores padrão para x_len e tolerance (podem ser ajustados conforme necessário)
    uint8_t x_len = 10;    // Comprimento mínimo de cada linha no eixo X
    uint8_t tolerance = 5; // Tolerância para movimentos indesejados

    // Subscrevendo as interrupções do mouse
    if (mouse_subscribe_int(&bit_no) != 0) {
        printf("Erro ao subscrever interrupções do mouse\n");
        return 1;
    }

    // Habilitando o envio de dados pelo mouse
    if (mouse_enable_data_reporting() != 0) {
        printf("Erro ao habilitar data reporting do mouse\n");
        mouse_unsubscribe_int();
        return 1;
    }

    // Criando uma estrutura packet para armazenar os dados do pacote
    struct packet pp;

    // Loop principal para receber os pacotes e detectar o gesto
    while (!gesture_detected) {
        // Receber uma mensagem (síncrona ou assíncrona)
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("Erro ao receber driver_receive\n");
            continue;
        }

        // Se não for uma notificação, ignorar
        if (is_ipc_notify(ipc_status)) {
            // Se for uma notificação do tipo HARDWARE
            if (_ENDPOINT_P(msg.m_source) == HARDWARE) {
                // Se for uma notificação do mouse
                if (msg.m_notify.interrupts & bit_no) {
                    // Processar a interrupção do mouse
                    mouse_ih();

                    // Verificar se temos um pacote completo
                    extern bool packet_ready;

                    if (packet_ready) {
                        // Analisar o pacote e imprimi-lo
                        mouse_parse_packet(&pp);
                        mouse_print_packet(&pp);

                        // Detectar o gesto
                        if (mouse_detect_gesture(&pp, x_len, tolerance)) {
                            gesture_detected = true;
                        }

                        packet_ready = false;
                    }
                }
            }
        }
    }

    // Desabilitando o envio de dados pelo mouse
    if (mouse_disable_data_reporting() != 0) {
        printf("Erro ao desabilitar data reporting do mouse\n");
        mouse_unsubscribe_int();
        return 1;
    }

    // Cancelando a subscrição das interrupções do mouse
    if (mouse_unsubscribe_int() != 0) {
        printf("Erro ao cancelar subscrição das interrupções do mouse\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Função de teste para operação do mouse em modo remoto
 * (Não será implementada neste laboratório)
 *
 * @param period Período em milissegundos
 * @param cnt Número de pacotes a receber
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
