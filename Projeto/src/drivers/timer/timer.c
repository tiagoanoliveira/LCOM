#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"
#include "timer.h"
#include "../utils/utils.h"

int hook_id_timer = 0;
uint32_t idle_counter = 0;

int timer_init() {
  uint8_t bit_no;
  return timer_subscribe_int(&bit_no); // subscreve o timer (e inicializa bitmask)
}

int timer_exit() {
  return timer_unsubscribe_int(); // remove subscrição
}

int (timer_set_frequency)(const uint8_t timer, const uint32_t freq) {
  if (timer > 2) return 1;
  if (freq < 19 || freq > TIMER_FREQ) return 1;

  uint16_t initial_count = TIMER_FREQ / freq;

  uint8_t lsb, msb;
  util_get_LSB(initial_count, &lsb);
  util_get_MSB(initial_count, &msb);

  uint8_t st;
  if ((timer_get_conf(timer, &st)) != 0) return 1;

  st = (st & (BIT(3) | BIT(2) | BIT(1) | BIT(0)));

  uint8_t ctrl_word;

  switch (timer) {
  case 0:
    ctrl_word = TIMER_SEL0 | TIMER_LSB_MSB | st;
    break;

  case 1:
    ctrl_word = TIMER_SEL1 | TIMER_LSB_MSB | st;
    break;

  case 2:
    ctrl_word = TIMER_SEL2 | TIMER_LSB_MSB | st;
    break;
  }

  if ((sys_outb(TIMER_CTRL, ctrl_word)) != 0) return 1;

  if ((sys_outb(TIMER_0 + timer, lsb)) != 0) return 1;
  if ((sys_outb(TIMER_0 + timer, msb)) != 0) return 1;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1;

  *bit_no = BIT(hook_id_timer);

  if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer)) != 0) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if ((sys_irqrmpolicy(&hook_id_timer)) != 0) return 1;

  return 0;
}

void (timer_int_handler)() {
  idle_counter++;
}

int (timer_get_conf)(const uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2) return 1;

  uint8_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if ((sys_outb(TIMER_CTRL, cmd)) != 0) return 1;

  if(util_sys_inb(TIMER_0 + timer, st) != 0) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val config;

  switch(field) {
    case tsf_all:
      config.byte = st;
      break;

    case tsf_initial:
      config.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;

    case tsf_mode:
      st = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;  // Extrai os bits 3-1 para o modo de contagem

      if (st == 6) {
        config.count_mode = 2;  // Ajusta 6 para 2
      } else if (st == 7) {
        config.count_mode = 3;  // Ajusta 7 para 3
      } else config.count_mode = st;
      break;

    case tsf_base:
      config.bcd = st & BIT(0);  // Verifica o bit 0 para saber se é BCD ou binário
      break;

    default:
      return 1;
  }

  if ((timer_print_config(timer, field, config)) != 0) return 1;
  return 0;
}

uint32_t timer_get_ticks() {
  return idle_counter;
}
