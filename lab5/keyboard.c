#include <lcom/lcf.h>
#include "keyboard.h"
#include "KBC.h"

// Variáveis globais
int kbd_hook_id = IRQ_KEYBOARD;  // ID para o hook de interrupção
uint8_t scancode = 0;            // Armazena o último scancode recebido
bool two_byte= false;
int error_flag=0;

int kbd_subscribe_int(uint8_t *bit_no) {
    // Verificar se o ponteiro é válido
    if (bit_no == NULL) return 1;

    // Definir a máscara de bits para detetar interrupções
    *bit_no = BIT(kbd_hook_id);

    // Subscrever interrupções do teclado em modo exclusivo
    return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
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
    else {
        if(status==SCANCODE_TWO_BYTE) {
            two_byte= true;
        }
        else {
            scancode =status;
            error_flag=0;
        }
    }
}

int kbd_restore_interrupts() {
    uint8_t cmd;

    if (kbc_write_command(KBC_CMD_REG, KBC_READ_CMD_BYTE) != 0) {
        printf("Erro: Não foi possível escrever a nova palavra de comandos!\n");
        return 1;
    }
    // Ler a palavra de comandos atual
    if (kbc_read_status(&cmd) != 0) {
        printf("Erro: Não foi possível ler a palavra de comandos!\n");
        return 1;
    }

    // Ativar as interrupções do teclado (bit 0)
    cmd |= KBC_CMD_BYTE_ENABLE_INT_KBD;

    if (kbc_write_command(KBC_CMD_REG, KBC_READ_CMD_BYTE) != 0) {
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

int wait_for_esc() {
  int ipc_status, r;
  message msg;
  uint8_t bit_no;

  scancode = 0; // limpar a variável global

  // subscreve e recebe em bit_no a máscara de notificações (já é BIT(hook_id))
  if (kbd_subscribe_int(&bit_no)) 
    return 1;

  uint32_t irq_set = bit_no;

  while (scancode != 0x81) {       // até receber o ESC break code
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d\n", r);
      continue;
    }
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE &&
        (msg.m_notify.interrupts & irq_set)) {
        kbc_ih();  // atualiza a variável global scancode
    }
  }

  if (kbd_unsubscribe_int()) 
    return 1;

  return 0;
}
