#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "mouse.h"

int hook_id_mouse = 3;
struct packet mouse_packet;
uint8_t packet_byte_index = 0;
uint8_t packet_bytes[3];
uint8_t current_byte;

void (mouse_ih)(){
    if(read_KBC_output(KBC_OUTPUT_BUF, &current_byte, 1)) printf("Error in reading byte from mouse\n");
}

int (mouse_write)(uint8_t command) {
  uint8_t attemps = MAX_ATTEMPS;
  uint8_t mouse_response;

  do {
    attemps--;
    if (write_KBC_command(KBC_INPUT_BUF, KBC_WRITE_TO_MOUSE)) return 1;
    if (write_KBC_command(KBC_OUTPUT_BUF, command)) return 1;
    tickdelay(micros_to_ticks(KBC_DELAY_US));
    if (util_sys_inb(KBC_OUTPUT_BUF, &mouse_response)) return 1;
    if (mouse_response == MOUSE_ACK) return 0;
  } while (mouse_response != MOUSE_ACK && attemps);

  return 1;
}

int (mouse_disable_data_reporting)() {
    uint8_t status, answer;
    int attempts = 5;

    while (attempts > 0) {
        // Esperar que input buffer esteja livre antes de enviar 0xD4
        if(util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;
        if(status & KBC_INPUT_BUFFER_FULL) {
            tickdelay(micros_to_ticks(KBC_DELAY_US));
            continue;
        }

        if(sys_outb(KBC_CMD_REG, KBC_WRITE_TO_MOUSE) != 0) return 1;

        // Esperar que input buffer esteja livre antes de enviar 0xF4
        if(util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;
        if(status & KBC_INPUT_BUFFER_FULL) {
            tickdelay(micros_to_ticks(KBC_DELAY_US));
            continue;
        }

        if(sys_outb(KBC_INPUT_BUF, MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;

        // Esperar por resposta do rato (ACK, NACK ou ERROR)
        if(util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;

        // Esperar até haver dado no buffer e que venha do rato
        if(!(status & KBC_OUTPUT_BUFFER_FULL) || !(status & KBC_STATUS_AUX)) {
            tickdelay(micros_to_ticks(KBC_DELAY_US));
            continue;
        }

        if(util_sys_inb(KBC_OUTPUT_BUF, &answer) != 0) return 1;

        if(answer == MOUSE_ACK) {
            return 0;
        } else if(answer == MOUSE_RESEND) {
            attempts--;
            tickdelay(micros_to_ticks(KBC_DELAY_US));
            continue;
        } else if(answer == MOUSE_ERROR) {
            return 1;
        }
    }

    return 1; // Se esgotar as tentativas todas
}

void mouse_sync_bytes() {
  if (packet_byte_index == 0 && (current_byte & FIRST_BYTE)) { // é o byte CONTROL, o bit 3 está ativo
    packet_bytes[packet_byte_index]= current_byte;
    packet_byte_index++;
  }
  else if (packet_byte_index > 0) {                            // recebe os deslocamentos em X e Y
    packet_bytes[packet_byte_index] = current_byte;
    packet_byte_index++;
  }
}

void (parse_packet)(){
  for (int i = 0 ; i < 3 ; i++) {
    mouse_packet.bytes[i] = packet_bytes[i];
  }

  mouse_packet.lb = packet_bytes[0] & MOUSE_LB;
  mouse_packet.mb = packet_bytes[0] & MOUSE_MB;
  mouse_packet.rb = packet_bytes[0] & MOUSE_RB;
  mouse_packet.x_ov = packet_bytes[0] & MOUSE_X_OVERFLOW;
  mouse_packet.y_ov = packet_bytes[0] & MOUSE_Y_OVERFLOW;
  mouse_packet.delta_x = (packet_bytes[0] & MOUSE_X_SIGNAL) ? (0xFF00 | packet_bytes[1]) : packet_bytes[1];
  mouse_packet.delta_y = (packet_bytes[0] & MOUSE_Y_SIGNAL) ? (0xFF00 | packet_bytes[2]) : packet_bytes[2];
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
    if(bit_no == NULL) return 1;

    *bit_no = BIT(hook_id_mouse);

    if((sys_irqsetpolicy(IRQ_MOUSE, (IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id_mouse)) != 0) return 1;

    return 0;
}

int (mouse_unsubscribe_int)() {
    if((sys_irqrmpolicy(&hook_id_mouse)) != 0) return 1;

    return 0;
}
