#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int hook_id = 0;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if(bit_no == NULL) return 1;
  *bit_no = BIT(hook_id);

  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK) return 1;

  return hook_id;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id) != 0) return 1;

  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t read_back_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  if (sys_outb(TIMER_CTRL, read_back_command) != 0) return 1;

  if(timer == 0) {
    util_sys_inb(TIMER_0, st);
  } else if(timer == 1) {
    util_sys_inb(TIMER_1, st);
  } else if(timer == 2) {
    util_sys_inb(TIMER_2, st);
  }

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val data;

  switch (field) {
    case tsf_all: 
      data.byte = st; 
      break;

    case tsf_initial:                                       
      data.in_mode = (st & TIMER_LSB_MSB) >> 4;
      break;

    case tsf_mode:
      st = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;

      if(st == 6) {
        data.count_mode = 2;
      } else if(st == 7) {
        data.count_mode = 3;
      } else data.count_mode = st;

      break;

    case tsf_base:
      data.bcd = (st & BIT(0));
      break;        

    default:
      return 1;
  }

  if (timer_print_config(timer, field, data) != 0) return 1;
  return 0;
}
