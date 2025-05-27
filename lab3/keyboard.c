#include <lcom/lcf.h>
#include "keyboard.h"
#include "KBC.h"

<<<<<<< HEAD
uint8_t scancode[2];
int hook_id_kbd = 1;
bool two_byte = false;
uint32_t sys_inb_counter = 0;
=======
// Variáveis globais
int kbd_hook_id = KBC_IRQ;  // ID para o hook de interrupção
uint8_t scancode = 0;            // Armazena o último scancode recebido
bool two_byte= false;
int error_flag=0;
>>>>>>> 28b3ce5d3af3605bc2a48da67c3e11421ac5c75e

int kbd_subscribe_int(uint8_t *bit_no) {
    // Verificar se o ponteiro é válido
    if (bit_no == NULL) return 1;

    // Definir a máscara de bits para detetar interrupções
    *bit_no = BIT(kbd_hook_id);

    // Subscrever interrupções do teclado em modo exclusivo
    return sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int kbd_unsubscribe_int() {
    // Cancelar a subscrição das interrupções
    return sys_irqrmpolicy(&kbd_hook_id);
}

void (kbc_ih)() {
    uint8_t status;

    // Ler o registo de estado do KBC
    if (kbc_read_output(KBC_OUTPUT_BUF, &status) != 0) {
        printf("Erro: Não foi possível ler o status do KBC!\n");
        error_flag=1;
    }
<<<<<<< HEAD
    attempts++;
    tickdelay(micros_to_ticks(KBC_DELAY_US));
  }
  return 1;
}

int (kbc_write_command)(uint8_t cmd) {
  int attempts = 0;
  uint8_t status;

  while(attempts < MAX_ATTEMPS) {
    if ((util_sys_inb(KBC_STATUS_REG, &status)) != 0) return 1;
    if ((status & KBC_INPUT_BUFFER_FULL) == 0) {
      if ((sys_outb(KBC_CMD_REG, cmd)) != 0) return 1;
      return 0;
=======
    else {
        if(status==SCANCODE_TWO_BYTE) {
            two_byte= true;
        }
        else {
            scancode =status;
            error_flag=0;
        }
>>>>>>> 28b3ce5d3af3605bc2a48da67c3e11421ac5c75e
    }
}

int kbd_restore_interrupts() {
    uint8_t cmd;

    if (kbc_write_command(KBC_CMD_REG, KBC_READ_CMD) != 0) {
        printf("Erro: Não foi possível escrever a nova palavra de comandos!\n");
        return 1;
    }
    // Ler a palavra de comandos atual
    if (kbc_read_status(&cmd) != 0) {
        printf("Erro: Não foi possível ler a palavra de comandos!\n");
        return 1;
    }

    // Ativar as interrupções do teclado (bit 0)
    cmd |= KBC_CMD_INT;

    if (kbc_write_command(KBC_CMD_REG, KBC_READ_CMD) != 0) {
        printf("Erro: Não foi possível escrever a nova palavra de comandos!\n");
        return 1;
    }
    // Escrever a nova palavra de comandos
    if (kbc_write_command(KBC_INPUT_BUF, cmd) != 0) {
        printf("Erro: Não foi possível escrever a nova palavra de comandos!\n");
        return 1;
    }

    return 0; // Sucesso
}
