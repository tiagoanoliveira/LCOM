#include "KBC.h"
#include "i8042.h"

// Ler o registo de estado do KBC
int kbc_read_status(uint8_t *status) {
    // Verificar se o ponteiro é válido
    if (status == NULL) return 1;

    // Ler o registo de estado do KBC
    return util_sys_inb(KBC_STATUS_REG, status);
}

// Ler dados do buffer de saída do KBC (com verificação de erros)
int kbc_read_output(uint8_t port, uint8_t *data) {
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS;

    while (attempts > 0) {
        // Ler o status atual do KBC
        if (kbc_read_status(&status) != 0) {
            printf("Erro: Status não disponível!\n");
            return 1;
        }

        // Verificar se o buffer de saída está cheio (bit 0 = 1)
        if (status & KBC_OUTPUT_BUFFER_FULL) {
            // Buffer de saída está cheio, posso ler
            if (util_sys_inb(port, data) != 0) {
                printf("Erro: Não foi possível ler os dados!\n");
                return 1;
            }

            // Verificar erros de paridade e timeout
            if (status & KBC_STATUS_PARITY_ERR) {
                printf("Erro: Erro de paridade detectado!\n");
                return 1;
            }

            if (status & KBC_STATUS_TIMEOUT_ERR) {
                printf("Erro: Erro de timeout detectado!\n");
                return 1;
            }

            return 0; // Sucesso, dados lidos sem erros
        }

        // Esperar antes de tentar novamente
        tickdelay(micros_to_ticks(KBC_DELAY_US));
        attempts--;
    }

    printf("Erro: Número máximo de tentativas excedido!\n");
    return 1; // Falha após todas as tentativas
}

// Escreve um comando no KBC
int kbc_write_command(uint8_t port, uint8_t cmd) {
    uint8_t status;
    uint8_t attempts = MAX_ATTEMPTS;

    while (attempts > 0) {
        // Ler o status atual do KBC
        if (kbc_read_status(&status) != 0) {
            printf("Erro: Status não disponível!\n");
            return 1;
        }

        // Verificar se o buffer de entrada está vazio (bit 1 = 0)
        if ((status & KBC_INPUT_BUFFER_FULL) == 0) {
            // Buffer de entrada está vazio, posso escrever
            if (sys_outb(port, cmd) != 0) {
                printf("Erro: Não foi possível escrever o comando!\n");
                return 1;
            }
            return 0; // Sucesso
        }

        // Esperar antes de tentar novamente
        tickdelay(micros_to_ticks(KBC_DELAY_US));
        attempts--;
    }

    printf("Erro: Número máximo de tentativas excedido!\n");
    return 1; // Falha após todas as tentativas
}
