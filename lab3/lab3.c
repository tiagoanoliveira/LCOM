#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include "utils.h"
#include "i8042.h"
#include "keyboard.h"

#include <stdbool.h>
#include <stdint.h>

extern int hook_id_timer;
extern int hook_id_kbd;
extern uint32_t counter;
extern uint8_t scancode[2];
extern bool two_byte;
extern uint32_t sys_inb_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t irq_set;
  int ipc_status, r;
  message msg;
  uint8_t size;
  bool make;
  bool done = false;

  if ((keyboard_subscribe_int(&irq_set)) != 0) return 1;

  while (!done) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
            kbc_ih();
            size = (scancode[0] == SCANCODE_TWO_BYTE) ? 2 : 1;
            make = (scancode[size - 1] & BIT(7)) ? false : true;
            kbd_print_scancode(make, size, scancode);
            if(scancode[0] == ESC_BREAK_CODE && size == 1) done = true;
          }
            break;
        default:
          break;
      }
    }
  }

  if((keyboard_unsubscribe_int()) != 0) return 1;
  kbd_print_no_sysinb(sys_inb_counter);

  return 0;
}

int(kbd_test_poll)() {
  uint8_t status, byte;
  uint8_t size;
  bool make;
  bool done = false;

  while (!done) {
    if ((kbc_read_output(&byte, &status)) != 0) continue;

    if (byte == SCANCODE_TWO_BYTE) {
      scancode[0] = byte;
      two_byte = true;
    } else if (two_byte) {
      scancode[1] = byte;
      two_byte = false;
    } else {
      scancode[0] = byte;
      two_byte = false;
    }

    size = (scancode[0] == SCANCODE_TWO_BYTE) ? 2 : 1;
    make = !(scancode[size - 1] & BIT(7));
    kbd_print_scancode(make, size, scancode);

    if (scancode[0] == ESC_BREAK_CODE && size == 1)
      done = true;

    tickdelay(micros_to_ticks(KBC_DELAY_US));
  }

  // Reativar interrupções
  if ((sys_outb(KBC_CMD_REG, KBC_READ_CMD_BYTE)) != 0) return 1;
  if ((util_sys_inb(KBC_OUTPUT_BUF, &byte)) != 0) return 1;

  byte |= KBC_CMD_BYTE_ENABLE_INT_KBD; // ativa a interrupção do teclado

  if ((sys_outb(KBC_CMD_REG, KBC_WRITE_CMD_BYTE)) != 0) return 1;
  if ((sys_outb(KBC_INPUT_BUF, byte)) != 0) return 1;

  kbd_print_no_sysinb(sys_inb_counter);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t irq_set_kbd, irq_set_timer;
  int ipc_status, r;
  message msg;
  uint8_t size;
  bool make;
  bool done = false;

  if ((keyboard_subscribe_int(&irq_set_kbd)) != 0) return 1;
  if ((timer_subscribe_int(&irq_set_timer)) != 0) return 1;

  while (!done) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbc_ih();
            size = (scancode[0] == SCANCODE_TWO_BYTE) ? 2 : 1;
            make = (scancode[size - 1] & BIT(7)) ? false : true;
            kbd_print_scancode(make, size, scancode);
            counter = 0;
            if(scancode[0] == ESC_BREAK_CODE && size == 1) done = true;
          } else if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (counter >= n * 60) done = true;
          }
            break;
        default:
          break;
      }
    }
  }

  if ((keyboard_unsubscribe_int()) != 0) return 1;
  if ((timer_unsubscribe_int()) != 0) return 1;
  kbd_print_no_sysinb(sys_inb_counter);

  return 0;
}
