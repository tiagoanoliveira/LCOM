#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>
#include "i80442.h"

// Variáveis globais
int hook_id_kbd = KBD_IRQ;       // Hook ID para subscrição de interrupções
uint8_t scancode_bytes[2];       // Buffer para armazenar bytes de scancode
uint8_t scancode_size = 0;       // Tamanho atual do scancode (1 ou 2 bytes)
bool two_byte = false;           // Indica se o próximo byte é o segundo de um scancode de 2 bytes
uint32_t sys_inb_counter = 0;    // Contador para sys_inb calls

// Subscrição de interrupções do teclado
int (kbd_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id_kbd); //Retorna o bit correspondente ao IRQ

  // Subscreve as interrupções do teclado com IRQ_REENABLE e IRQ_EXCLUSIVE
  if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_kbd) != 0) return 1;

  return 0;
}

// Cancelamento da subscrição de interrupções do teclado
int (kbd_unsubscribe_int)() {
  if ((sys_irqrmpolicy(&hook_id_kbd)) != 0) return 1;
  return 0;
}

//Função auxiliar para ler um byte do buffer de saída do KBC
int (kbc_read_output)(uint8_t *data, uint8_t *status) {
  int attempts = 0;

  while(attempts < MAX_ATTEMPS) {
    // Ler o registo de estado
    if (util_sys_inb(KBC_STATUS_REG, status) != 0) return 1;
    sys_inb_counter++; // Incrementar contador de chamadas sys_inb

    // Verificar se o buffer de saída está cheio (pronto para leitura)
    if (*status & KBC_OUTPUT_BUFFER_FULL) {
      // Verificar se não há errps
      if ((*status & (KBC_STATUS_PARITY_ERR | KBC_STATUS_TIMEOUT_ERR)) != 0 ) return 1;
      // Ler o dado do buffer de saída
      if (util_sys_inb(KBC_OUTPUT_BUF, data) != 0) return 1;
      sys_inb_counter++; // Incrementar contador de chamadas sys_inb
      return 0;
    }
    attempts++;
    tickdelay(micros_to_ticks(KBC_DELAY_US));
  }
  return 1; // Timeout - não foi possível ler após várias tentativas
}

// Interrupt handler do teclado
void (kbc_ih)(void) {
  uint8_t status, byte;

  // Ler o byte do buffer de saída
  if(kbc_read_output(&sbyte, &status)!=0) return 1;

  // Processar o byte lido
  if (scancode_byte == SCANCODE_TWO_BYTE) {
    scancode_bytes[0] = byte;
    two_byte = true;
  } else if (two_byte) {
    scancode_bytes[1] = byte;
    two_byte = false;
    scancode_size = 2;
  } else {
    scancode_bytes[0] = byte;
    two_byte = false;
    scancode_size=1;
  }
}

// Escrever um comando para o registo de comandos do KBC
int (kbc_write_command)(uint8_t cmd) {
  int attempts = 0;
  uint8_t status;

  while(attempts < MAX_ATTEMPS) {
    // Ler o resgito de estado
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;
    sys_inb_counter++; // Incrementar contador de chamadas sys_inb

    //Verificar se o buffer de entrada está vazio (pronto a escrever)
    if ((status & KBC_INPUT_BUF) == 0) {
      // Escrever o comando no registo de comandos
      if ((sys_outb(KBC_CMD_REG, cmd)) != 0) return 1;
      return 0;
    }
    attempts++;
    tickdelay(micros_to_ticks(KBC_DELAY_US));
  }
  return 1; // Timeout - não foi possível escrever após várias tentativas
}
// Escrever um argumento para o buffer de entrada do KBC
int (kbc_write_argument)(uint8_t arg) {
  int attempts = 0;
  uint8_t status;

  while (attempts < MAX_ATTEMPTS) {
    // Ler o registo de estado
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;
    sys_inb_counter++; // Incrementar contador de chamadas sys_inb

    // Verificar se o buffer de entrada está vazio (pronto para escrever)
    if ((status & KBC_INPUT_BUFFER_FULL) == 0) {
      // Escrever o argumento no buffer de entrada
      if (sys_outb(KBC_INPUT_BUF, arg) != 0) return 1;
      return 0;
    }

    attempts++;
    tickdelay(micros_to_ticks(KBC_DELAY_US));
  }

  return 1; // Timeout - não foi possível escrever após várias tentativas
}

// Ler o byte de comando atual do KBC
int (kbc_get_command_byte)(uint8_t *cmd_byte) {
  // Enviar comando para ler o byte de comando
  if (kbc_write_command(KBC_READ_CMD) != 0) return 1;

  // Ler o byte de comando do buffer de saída
  uint8_t status;
  if (kbc_read_output(cmd_byte, &status) != 0) return 1;

  return 0;
}

// Definir um novo byte de comando para o KBC
int (kbc_set_command_byte)(uint8_t cmd_byte) {
  // Enviar comando para escrever o byte de comando
  if (kbc_write_command(KBC_WRITE_CMD) != 0) return 1;

  // Escrever o novo byte de comando como argumento
  if (kbc_write_argument(cmd_byte) != 0) return 1;

  return 0;
}