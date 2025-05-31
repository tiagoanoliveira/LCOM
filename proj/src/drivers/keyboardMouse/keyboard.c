#include <lcom/lcf.h>
#include <lcom/lab3.h>
#include <stdbool.h>
#include <stdint.h>
#include "keyboard.h"

uint8_t scancode[2];
int hook_id_kbd = 1;
bool two_byte = false;
uint32_t sys_inb_counter = 0;

int keyboard_init() {
  uint8_t bit_no;
  return keyboard_subscribe_int(&bit_no);
}

int keyboard_exit() {
  return keyboard_unsubscribe_int();
}

void (kbc_ih)(void) {
  uint8_t scancode_byte;
  uint8_t status;

  kbc_read_output(&scancode_byte, &status);

  if (scancode_byte == SCANCODE_TWO_BYTE) {
    scancode[0] = scancode_byte;
    two_byte = true;
  } else if (two_byte) {
      scancode[1] = scancode_byte;
      two_byte = false;
  } else {
    scancode[0] = scancode_byte;
    two_byte = false;
  }
}

int (kbc_read_output)(uint8_t *data, uint8_t *status) {
  int attempts = 0;

  while(attempts < MAX_ATTEMPTS) {
    if ((util_sys_inb(KBC_STATUS_REG, status)) != 0) return 1;

    if (*status & KBC_OUTPUT_BUFFER_FULL) {
      if ((*status & (KBC_STATUS_PARITY_ERR | KBC_STATUS_TIMEOUT_ERR)) != 0 ) return 1;
      if ((util_sys_inb(KBC_OUTPUT_BUF, data)) != 0) return 1;

      return 0;
    }
    attempts++;
    tickdelay(micros_to_ticks(KBC_DELAY_US));
  }
  return 1;
}

int (kbc_write_command)(uint8_t cmd) {
  int attempts = 0;
  uint8_t status;

  while(attempts < MAX_ATTEMPTS) {
    if ((util_sys_inb(KBC_STATUS_REG, &status)) != 0) return 1;
    if ((status & KBC_INPUT_BUFFER_FULL) == 0) {
      if ((sys_outb(KBC_CMD_REG, cmd)) != 0) return 1;
      return 0;
    }
    attempts++;
    tickdelay(micros_to_ticks(KBC_DELAY_US));
  }

  return 1;
}

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(hook_id_kbd);

  if ((sys_irqsetpolicy(IRQ_KEYBOARD, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_kbd)) != 0) return 1;

  return 0;
}

int (keyboard_unsubscribe_int)() {
  if ((sys_irqrmpolicy(&hook_id_kbd)) != 0) return 1;

  return 0;
}
