#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>
#include <stdint.h>
#include "i8042.h"

int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();
void (mouse_ih)();
int (mouse_enable_data_reporting)();
int mouse_disable_data_reporting();
int mouse_write_cmd(uint8_t cmd);
int mouse_sync_bytes();
void mouse_parse_packet(struct packet *pp);
int mouse_detect_gesture(struct packet *pp, uint8_t x_len, uint8_t tolerance);

#endif //MOUSE_H
