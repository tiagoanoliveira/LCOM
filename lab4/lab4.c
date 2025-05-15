// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab4.h>
#include <stdint.h>
#include <stdio.h>

#include "utils.h"
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"

typedef enum {
  START,
  UP,
  VERTEX,
  DOWN,
  END_STATE
} SystemState;

extern struct packet mouse_packet;
extern uint8_t packet_byte_index;
extern uint32_t idle_counter;

static SystemState state = START;
static int16_t x_len_total = 0;

static void update_state_machine(uint8_t x_len, uint8_t tolerance);

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t cnt) {
  uint8_t irq_set;
  int ipc_status, r;
  message msg;

  if(mouse_subscribe_int(&irq_set) != 0) return 1;

  if(mouse_enable_data_reporting() != 0) {
      mouse_unsubscribe_int();
      return 1;
  }

  while (cnt) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if (is_ipc_notify(ipc_status)) {
        switch (_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
              if (msg.m_notify.interrupts & irq_set) {
                mouse_ih();                               // Lemos mais um byte
                mouse_sync_bytes();                       // Sincronizamos esse byte no pacote respectivo

                if (packet_byte_index == 3) {             // Quando tivermos três bytes do mesmo pacote
                  parse_packet();                         // Formamos o pacote
                  mouse_print_packet(&mouse_packet);      // Mostramos o pacote
                  packet_byte_index = 0;
                  cnt--;
                }
              }
              break;
          default:
              break;
        }
    }
  }

  if(mouse_disable_data_reporting() != 0) {
      mouse_unsubscribe_int();
      return 1;
  }

  if(mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
  uint8_t irq_set_mouse, irq_set_timer;
  int ipc_status, r;
  message msg;
  bool done = false;

  if(timer_subscribe_int(&irq_set_timer) != 0) return 1;
  if(mouse_subscribe_int(&irq_set_mouse) != 0) return 1;

  if(mouse_enable_data_reporting() != 0) {
    mouse_unsubscribe_int();
    return 1;
  }

  while(!done) {
    if((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with: %d", r);
        continue;
    }

    if(is_ipc_notify(ipc_status)) {
        switch(_ENDPOINT_P(msg.m_source)) {
          case HARDWARE:
              if(msg.m_notify.interrupts & irq_set_mouse) {
                mouse_ih();                               // Lemos mais um byte
                mouse_sync_bytes();                       // Sincronizamos esse byte no pacote respectivo

                if(packet_byte_index == 3) {              // Quando tivermos três bytes do mesmo pacote
                  idle_counter = 0;
                  parse_packet();                         // Formamos o pacote
                  mouse_print_packet(&mouse_packet);      // Mostramos o pacote
                  packet_byte_index = 0;
                }
              } else if(msg.m_notify.interrupts & irq_set_timer) {
                timer_int_handler();
                if(idle_counter >= idle_time * 60){
                  done = true;
                }
              }
              break;
          default:
              break;
        }
    }
  }

  if(mouse_disable_data_reporting() != 0) {
    mouse_unsubscribe_int();
    return 1;
  }

  if(mouse_unsubscribe_int() != 0) return 1;
  if(timer_unsubscribe_int() != 0) return 1;

  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t irq_set;
  int ipc_status, r;
  message msg;

  if(mouse_subscribe_int(&irq_set) != 0) return 1;

  if(mouse_write(MOUSE_ENABLE_DATA_REPORTING) != 0) {
      mouse_unsubscribe_int();
      return 1;
  }

  printf("Subscribed on mouse IRQ mask = 0x%02X\n", irq_set);

  while (state != END_STATE) {
    printf("ENTROU NO LOOP\n");
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed: %d\n", r);
      continue;
    }

    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          mouse_ih();
          mouse_sync_bytes();

          if (packet_byte_index == 3) {
            parse_packet(&mouse_packet);
            mouse_print_packet(&mouse_packet);
            printf("delta_x = %d, delta_y = %d\n", mouse_packet.delta_x, mouse_packet.delta_y);
            packet_byte_index = 0;
            update_state_machine(x_len, tolerance);
          }
          break;

        default:
          break;
      }
    }
  }

  if(mouse_disable_data_reporting() != 0) {
    mouse_unsubscribe_int();
    return 1;
  }

  if(mouse_unsubscribe_int() != 0) return 1;

  return 0;
}

static void update_state_machine(uint8_t x_len, uint8_t tolerance) {
  struct mouse_ev *ev = mouse_detect_event(&mouse_packet);

  switch (state) {
    case START:
      if (ev->type == LB_PRESSED) {
        state = UP;
        x_len_total = 0;
      }
      break;

    case UP:
      if (ev->type == MOUSE_MOV &&
          ev->delta_y  >  0 &&                                // subindo
          abs(ev->delta_x) <= tolerance &&                    // desvio X dentro da tolerância
          abs(ev->delta_y) > abs(ev->delta_x)) {              // slope > 1
        x_len_total += abs(ev->delta_x);
      }
      else if (ev->type == LB_RELEASED) {
        if (x_len_total >= x_len)
          state = VERTEX;
        else
          state = START;
      }
      else {
        state = START;
      }
      break;

    case VERTEX:
      if (ev->type == RB_PRESSED) {
        state = DOWN;
        x_len_total = 0;
      }
      else if (ev->type == MOUSE_MOV && abs(ev->delta_x) <= tolerance && abs(ev->delta_y) <= tolerance) {
        /* permanece em VERTEX */
      }
      else {
        state = START;
      }
      break;

    case DOWN:
      if (ev->type == MOUSE_MOV &&
          ev->delta_y  <  0 &&                               // descendo
          abs(ev->delta_x) <= tolerance &&                   // desvio X dentro da tolerância
          abs(ev->delta_y) > abs(ev->delta_x)) {             // slope > 1
        x_len_total += abs(ev->delta_x);
      }
      else if (ev->type == RB_RELEASED) {
        if (x_len_total >= x_len)
          state = END_STATE;
        else
          state = START;
      }
      else {
        state = START;
      }
      break;

    case END_STATE:
      /* nada a fazer */
      break;
  }
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}
