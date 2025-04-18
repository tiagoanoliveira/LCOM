#include "mouse.h"
#include "KBC.h"
#include <lcom/lcf.h>

// Variáveis globais para armazenar os dados do mouse
static int hook_id_mouse = 12;        // IRQ do mouse é 12
static uint8_t packet_bytes[3];      // Array para armazenar os 3 bytes do pacote do mouse
static uint8_t packet_index = 0;     // Índice do byte atual no pacote
static bool packet_ready = false;    // Flag para indicar se um pacote completo está pronto

/**
 * @brief Subscreve as interrupções do mouse
 *
 * @param bit_no Endereço da variável onde será armazenado o bit de notificação
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int mouse_subscribe_int(uint8_t *bit_no) {
    if (bit_no == NULL) return 1;

    *bit_no = BIT(hook_id_mouse); // Retorna o bit correspondente à IRQ do mouse

    // Subscreve as interrupções e usa as policies IRQ_REENABLE e IRQ_EXCLUSIVE
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_mouse) != 0) {
        printf("Erro ao subscrever as interrupções do mouse\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Cancela a subscrição das interrupções do mouse
 *
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int mouse_unsubscribe_int() {
    // Remove a política de interrupção para cancelar a subscrição
    if (sys_irqrmpolicy(&hook_id_mouse) != 0) {
        printf("Erro ao cancelar subscrição das interrupções do mouse\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Handler de interrupções do mouse
 * Lê um byte do buffer de saída do KBC e o armazena no array de bytes do pacote
 */
void (mouse_ih)() {
    uint8_t data;
    uint8_t status;

    // Lê o status do KBC
    if (kbc_read_status(&status) != 0) {
        printf("Erro ao ler o status do KBC na mouse_ih\n");
        return;
    }

    // Verifica se o buffer de saída está cheio e se os dados são do mouse (bit AUX = 1)
    if ((status & KBC_OUTPUT_BUFFER_FULL) && (status & KBC_AUX)) {
        // Lê o byte do buffer de saída
        if (util_sys_inb(KBC_OUT_BUF, &data) != 0) {
            printf("Erro ao ler byte do buffer de saída na mouse_ih\n");
            return;
        }

        // Verifica se houve erro de paridade ou timeout
        if ((status & (KBC_STATUS_PARITY_ERR | KBC_STATUS_TIMEOUT_ERR)) == 0) {
            // Armazena o byte no array de bytes do pacote
            if (packet_index == 0) {
                // Verifica se o primeiro byte tem o bit 3 = 1 (sempre deve ser 1 no primeiro byte)
                if (data & BIT(3)) {
                    packet_bytes[packet_index] = data;
                    packet_index++;
                }
            }
            else {
                // Armazena os bytes subsequentes
                packet_bytes[packet_index] = data;
                packet_index++;

                // Verifica se um pacote completo foi recebido
                if (packet_index == 3) {
                    packet_ready = true;
                    packet_index = 0; // Reseta o índice para o próximo pacote
                }
            }
        }
    }
}

/**
 * @brief Escreve um comando para o mouse
 *
 * @param cmd O comando a ser enviado ao mouse
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int mouse_write_cmd(uint8_t cmd) {
    uint8_t attempts = 5;  // Número de tentativas
    uint8_t ack;          // Byte de reconhecimento do mouse

    while (attempts > 0) {
        // Passo 1: Enviar comando 0xD4 para o KBC (para encaminhar o byte para o mouse)
        if (kbc_write_command(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE) != 0) {
            printf("Erro ao enviar comando 0xD4 para o KBC\n");
            return 1;
        }

        // Passo 2: Enviar o comando para o mouse através da porta 0x60
        if (kbc_write_command(KBC_IN_BUF, cmd) != 0) {
            printf("Erro ao enviar comando 0x%x para o mouse\n", cmd);
            return 1;
        }

        // Passo 3: Ler o byte de reconhecimento (ACK) do mouse
        if (kbc_read_output(KBC_OUT_BUF, &ack) != 0) {
            printf("Erro ao ler ACK do mouse\n");
            return 1;
        }

        // Verificar o valor do ACK
        if (ack == MOUSE_ACK) {
            return 0;  // Sucesso
        }
        else if (ack == MOUSE_NACK) {
            printf("Mouse enviou NACK (0x%x), tentando novamente\n", ack);
            attempts--;
        }
        else if (ack == MOUSE_ERROR) {
            printf("Mouse enviou ERROR (0x%x), tentando novamente\n", ack);
            attempts--;
        }
        else {
            printf("Mouse enviou resposta inesperada (0x%x), tentando novamente\n", ack);
            attempts--;
        }

        // Espera um pouco antes de tentar novamente
        tickdelay(micros_to_ticks(WAIT_KBC));
    }

    printf("Número máximo de tentativas excedido ao enviar comando para o mouse\n");
    return 1;
}

/**
 * @brief Habilita o envio de dados do mouse em modo de stream
 *
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int mouse_enable_data_reporting() {
    // Envia o comando para habilitar data reporting
    if (mouse_write_cmd(MOUSE_ENABLE_DATA_REPORTING) != 0) {
        printf("Erro ao habilitar data reporting do mouse\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Desabilita o envio de dados do mouse em modo de stream
 *
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int mouse_disable_data_reporting() {
    // Envia o comando para desabilitar data reporting
    if (mouse_write_cmd(MOUSE_DISABLE_DATA_REPORTING) != 0) {
        printf("Erro ao desabilitar data reporting do mouse\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Sincroniza os bytes do pacote do mouse
 * Espera até receber o primeiro byte válido de um pacote
 *
 * @return int 0 em caso de sucesso, 1 em caso de erro
 */
int mouse_sync_bytes() {
    uint8_t byte;
    uint8_t status;

    while (1) {
        // Lê o status do KBC
        if (kbc_read_status(&status) != 0) {
            printf("Erro ao ler status do KBC em mouse_sync_bytes\n");
            return 1;
        }

        // Verifica se o buffer de saída está cheio e se os dados são do mouse
        if ((status & KBC_OUTPUT_BUFFER_FULL) && (status & KBC_AUX)) {
            // Lê o byte do buffer de saída
            if (util_sys_inb(KBC_OUT_BUF, &byte) != 0) {
                printf("Erro ao ler byte do buffer em mouse_sync_bytes\n");
                return 1;
            }

            // Verifica se o byte tem o bit 3 = 1 (primeiro byte do pacote)
            if (byte & BIT(3)) {
                packet_bytes[0] = byte;
                packet_index = 1;
                return 0;  // Sincronizado com sucesso
            }
        }

        // Espera um pouco antes de tentar novamente
        tickdelay(micros_to_ticks(WAIT_KBC));
    }
}

/**
 * @brief Analisa um pacote completo do mouse e preenche a estrutura packet
 *
 * @param pp Ponteiro para a estrutura packet a ser preenchida
 */
void mouse_parse_packet(struct packet *pp) {
    if (pp == NULL) return;

    // Copia os 3 bytes do pacote para a estrutura
    for (int i = 0; i < 3; i++) {
        pp->bytes[i] = packet_bytes[i];
    }

    // Extrai os estados dos botões (bits 0, 1 e 2 do primeiro byte)
    pp->lb = packet_bytes[0] & BIT(0);  // Left button
    pp->rb = packet_bytes[0] & BIT(1);  // Right button
    pp->mb = packet_bytes[0] & BIT(2);  // Middle button

    // Extrai os flags de overflow
    pp->x_ov = packet_bytes[0] & BIT(6);  // X overflow
    pp->y_ov = packet_bytes[0] & BIT(7);  // Y overflow

    // Extrai o deslocamento em X e Y
    // Para o X, verifica o sinal (bit 4 do primeiro byte)
    if (packet_bytes[0] & BIT(4)) {
        // Valor negativo - complemento de 2
        pp->delta_x = packet_bytes[1] | 0xFF00;  // Extensão do sinal
    } else {
        pp->delta_x = packet_bytes[1];
    }

    // Para o Y, verifica o sinal (bit 5 do primeiro byte)
    if (packet_bytes[0] & BIT(5)) {
        // Valor negativo - complemento de 2
        pp->delta_y = packet_bytes[2] | 0xFF00;  // Extensão do sinal
    } else {
        pp->delta_y = packet_bytes[2];
    }
}

// Definição de estados para a máquina de estados do gesto
typedef enum {
    INITIAL,        // Estado inicial
    DRAW_UP,        // Desenhando a primeira linha (para cima)
    VERTEX,         // No vértice
    DRAW_DOWN       // Desenhando a segunda linha (para baixo)
} GestureState;

// Variáveis globais para detecção de gestos
static GestureState current_state = INITIAL;
static bool last_lb = false;         // Estado anterior do botão esquerdo
static bool last_rb = false;         // Estado anterior do botão direito

/**
 * @brief Detecta o gesto de um V invertido
 *
 * @param pp Ponteiro para o pacote atual do mouse
 * @param x_len Comprimento mínimo no eixo X para cada linha
 * @param tolerance Tolerância para movimentos indesejados
 * @return int 1 se o gesto foi detectado, 0 caso contrário
 */
int mouse_detect_gesture(struct packet *pp, uint8_t x_len, uint8_t tolerance) {
    if (pp == NULL) return 0;

    // Verificar se é um pacote de início ou fim de linha (botão pressionado/solto)
    // Neste caso, ignoramos qualquer deslocamento para simplificar a máquina de estados
    bool button_event = ((last_lb != pp->lb) || (last_rb != pp->rb));

    // Atualizar os estados dos botões para a próxima chamada
    last_lb = pp->lb;
    last_rb = pp->rb;

    // Lógica da máquina de estados
    switch (current_state) {
        case INITIAL:
            // Verificar se o botão esquerdo foi pressionado e os outros estão soltos
            if (pp->lb && !pp->rb && !pp->mb && !last_lb) {
                printf("Iniciando primeira linha (para cima)\n");
                current_state = DRAW_UP;
                return 0;
            }
            break;

        case DRAW_UP:
            // Se o botão esquerdo foi solto
            if (!pp->lb && last_lb) {
                printf("Botão esquerdo solto, entrando no vértice\n");
                current_state = VERTEX;
                return 0;
            }

            // Se estamos desenhando (botão esquerdo pressionado) e não é um evento de botão
            if (pp->lb && !button_event) {
                // Verificar se o movimento é válido (para cima e direita)
                // O slope absoluto deve ser > 1 (|dy/dx| > 1 => |dy| > |dx|)
                if (pp->delta_x < -tolerance) {
                    // Movimento para a esquerda maior que a tolerância, resetar
                    printf("Movimento para a esquerda detectado, reiniciando\n");
                    current_state = INITIAL;
                    return 0;
                }

                if (pp->delta_y < -tolerance) {
                    // Movimento para baixo maior que a tolerância, resetar
                    printf("Movimento para baixo detectado, reiniciando\n");
                    current_state = INITIAL;
                    return 0;
                }

                // Verificar o slope absoluto (somente se houver movimento em X significativo)
                if (pp->delta_x > tolerance) {
                    // Se o slope absoluto não for maior que 1, resetar
                    if (abs(pp->delta_y) <= abs(pp->delta_x)) {
                        printf("Inclinação inválida na primeira linha, reiniciando\n");
                        current_state = INITIAL;
                        return 0;
                    }
                }
            }

            // Se algum outro botão for pressionado, resetar
            if (pp->rb || pp->mb) {
                printf("Botão errado pressionado durante a primeira linha, reiniciando\n");
                current_state = INITIAL;
                return 0;
            }
            break;

        case VERTEX:
            // Verificar se o botão direito foi pressionado e os outros estão soltos
            if (pp->rb && !pp->lb && !pp->mb && !last_rb) {
                printf("Botão direito pressionado, iniciando segunda linha (para baixo)\n");
                current_state = DRAW_DOWN;
                return 0;
            }

            // Verificar se há movimento residual no vértice
            if (!button_event) {
                if ((abs(pp->delta_x) > tolerance) || (abs(pp->delta_y) > tolerance)) {
                    printf("Movimento residual excessivo no vértice, reiniciando\n");
                    current_state = INITIAL;
                    return 0;
                }
            }

            // Se algum botão incorreto for pressionado, resetar
            if (pp->lb || pp->mb) {
                printf("Botão errado pressionado no vértice, reiniciando\n");
                current_state = INITIAL;
                return 0;
            }
            break;

        case DRAW_DOWN:
            // Se o botão direito foi solto
            if (!pp->rb && last_rb) {
                printf("Botão direito solto, gesto completado com sucesso!\n");
                current_state = INITIAL;  // Resetar para permitir novos gestos
                return 1;  // Gesto detectado!
            }

            // Se estamos desenhando (botão direito pressionado) e não é um evento de botão
            if (pp->rb && !button_event) {
                // Verificar se o movimento é válido (para baixo e direita)
                if (pp->delta_x < -tolerance) {
                    // Movimento para a esquerda maior que a tolerância, resetar
                    printf("Movimento para a esquerda detectado na segunda linha, reiniciando\n");
                    current_state = INITIAL;
                    return 0;
                }

                if (pp->delta_y > tolerance) {
                    // Movimento para cima maior que a tolerância, resetar
                    printf("Movimento para cima detectado na segunda linha, reiniciando\n");
                    current_state = INITIAL;
                    return 0;
                }

                // Verificar o slope absoluto (somente se houver movimento em X significativo)
                if (pp->delta_x > tolerance) {
                    // Se o slope absoluto não for maior que 1, resetar
                    if (abs(pp->delta_y) <= abs(pp->delta_x)) {
                        printf("Inclinação inválida na segunda linha, reiniciando\n");
                        current_state = INITIAL;
                        return 0;
                    }
                }
            }

            // Se algum outro botão for pressionado, resetar
            if (pp->lb || pp->mb) {
                printf("Botão errado pressionado durante a segunda linha, reiniciando\n");
                current_state = INITIAL;
                return 0;
            }
            break;
    }

    return 0;  // Gesto ainda não foi detectado
}
