#include <lcom/lcf.h>
#include "keyboard.h"
#include "KBC.h"

// Variáveis globais
static int keyboard_hook_id = KBC_IRQ;  // ID para o hook de interrupção
static uint8_t scancode = 0;            // Armazena o último scancode recebido

int keyboard_subscribe_int(uint8_t *bit_no) {
    // Verificar se o ponteiro é válido
    if (bit_no == NULL) return 1;

    // Definir a máscara de bits para detetar interrupções
    *bit_no = BIT(keyboard_hook_id);

    // Subscrever interrupções do teclado em modo exclusivo
    return sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

int keyboard_unsubscribe_int() {
    // Cancelar a subscrição das interrupções
    return sys_irqrmpolicy(&keyboard_hook_id);
}

void kbc_ih() {
    uint8_t status;

    // Ler o registo de estado do KBC
    if (kbc_read_status(&status) != 0) {
        printf("Erro: Não foi possível ler o status do KBC!\n");
        return;
    }

    // Verificar se há dados disponíveis e se não há erros de paridade ou timeout
    if ((status & KBC_OUTPUT_BUFFER_FULL) &&
        !(status & KBC_STATUS_PARITY_ERR) &&
        !(status & KBC_STATUS_TIMEOUT_ERR)) {

        // Ler o scancode do buffer de saída
        if (util_sys_inb(KBC_OUTPUT_BUF, &scancode) != 0) {
            printf("Erro: Não foi possível ler o scancode!\n");
        }
    }
}

uint8_t keyboard_get_scancode() {
    // Retornar o último scancode recebido
    return scancode;
}

int keyboard_read_scancode(uint8_t *scancode) {
    // Verificar se o ponteiro é válido
    if (scancode == NULL) return 1;

    // Ler diretamente o próximo scancode do buffer de saída
    return kbc_read_output(KBC_OUTPUT_BUF, scancode);
}

int keyboard_restore() {
    uint8_t cmd_byte;

    // Ler a palavra de comandos atual
    if (kbc_read_command_byte(&cmd_byte) != 0) {
        printf("Erro: Não foi possível ler a palavra de comandos!\n");
        return 1;
    }

    // Ativar as interrupções do teclado (bit 0)
    cmd_byte |= KBC_CMD_INT;

    // Escrever a nova palavra de comandos
    if (kbc_write_command_byte(cmd_byte) != 0) {
        printf("Erro: Não foi possível escrever a nova palavra de comandos!\n");
        return 1;
    }

    return 0; // Sucesso
}