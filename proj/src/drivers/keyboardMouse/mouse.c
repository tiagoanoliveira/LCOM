#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include "mouse.h"

int mouse_x = 512;
int mouse_y = 384;
int hook_id_mouse = 3;
struct packet mouse_packet;
uint8_t packet_byte_index = 0;
uint8_t packet_bytes[3];
uint8_t current_byte;

int mouse_init() {
  uint8_t bit_no;
  return mouse_subscribe_int(&bit_no);
}

int mouse_exit() {
  return mouse_unsubscribe_int();
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

void mouse_sync_bytes() {
  if (packet_byte_index == 0 && (current_byte & FIRST_BYTE)) { // Control byte
    packet_bytes[packet_byte_index]= current_byte;
    packet_byte_index++;
  }
  else if (packet_byte_index > 0) {
    packet_bytes[packet_byte_index] = current_byte;
    packet_byte_index++;
  }
}

void (mouse_ih)(){
    uint8_t status;
    if (util_sys_inb(KBC_STATUS_REG, &status) != 0) return;

    // Only proceed if output buffer is full and it's from mouse (AUX set)
    if ((status & KBC_OUTPUT_BUFFER_FULL) && (status & KBC_STATUS_AUX)) {
        if (read_KBC_output(KBC_OUTPUT_BUF, &current_byte, 1) != 0) {
            printf("Error in reading byte from mouse\n");
        }
    }
}

void (mouse_update_position)() {
    mouse_x += mouse_packet.delta_x;
    mouse_y -= mouse_packet.delta_y; // Inverted Y axle

    // Clamp to the scren limits
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_x > mode_info.XResolution - 1) mouse_x = mode_info.XResolution - 1;

    if (mouse_y < 0) mouse_y = 0;
    if (mouse_y > mode_info.YResolution - 1) mouse_y = mode_info.YResolution - 1;
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

int (mouse_write)(uint8_t command) {
  uint8_t attempts = MAX_ATTEMPTS;
  uint8_t mouse_response;

  while (attempts > 0) {
      // Send signal to KBC to write to the mouse
      if (write_KBC_command(KBC_CMD_REG, KBC_WRITE_TO_MOUSE) != 0){
        attempts--;
        continue;
      }

      // Send the actual command to the mouse
      if (write_KBC_command(KBC_INPUT_BUF, command) != 0) {
        attempts--;
        continue;
      }

      // Read the response from the mouse
      if (read_KBC_output(KBC_OUTPUT_BUF, &mouse_response, 1) != 0) {
        attempts--;
        continue;
      }

      // Check if response is ACK
      if (mouse_response == MOUSE_ACK) {
        return 0;
      } 

      // Retry if response is RESEND
      if (mouse_response == MOUSE_RESEND) {
        attempts--;
        continue;
      }
  }

  return 1;
}

int (mouse_disable_data_reporting)() {
    uint8_t status, answer;
    int attempts = 5;

    while (attempts > 0) {
        // Check if input buffer is free
        if(util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;
        if(status & KBC_INPUT_BUFFER_FULL) {
            tickdelay(micros_to_ticks(KBC_DELAY_US));
            attempts--;
            continue;
        }

        if(sys_outb(KBC_CMD_REG, KBC_WRITE_TO_MOUSE) != 0) return 1;

        if(util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;
        if(status & KBC_INPUT_BUFFER_FULL) {
            tickdelay(micros_to_ticks(KBC_DELAY_US));
            attempts--;
            continue;
        }

        if(sys_outb(KBC_INPUT_BUF, MOUSE_DISABLE_DATA_REPORTING) != 0) return 1;

        // Check mouse answer (ACK, NACK ou ERROR)
        if(util_sys_inb(KBC_STATUS_REG, &status) != 0) return 1;

        // Check if output buffer is full and is from mouse
        if(!(status & KBC_OUTPUT_BUFFER_FULL) || !(status & KBC_STATUS_AUX)) {
            printf("Timeout waiting for mouse response...\n");
            tickdelay(micros_to_ticks(KBC_DELAY_US));
            attempts--;
            continue;
        }

        if (read_KBC_output(KBC_OUTPUT_BUF, &answer, 1) != 0) return 1;

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

    return 1;
}
