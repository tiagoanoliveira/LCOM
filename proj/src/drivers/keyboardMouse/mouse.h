#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "KBC.h"
#include "i8042.h"
#include "../timer/i8254.h"
#include "../utils/utils.h"
#include "../graphics/graphics.h"

extern int mouse_x;
extern int mouse_y;
extern int hook_id_mouse;
extern struct packet mouse_packet;
extern uint8_t packet_byte_index ;
extern uint8_t packet_bytes[3];
extern uint8_t current_byte;

int mouse_init();
int mouse_exit();

void (mouse_ih)();
void (parse_packet)();
void (mouse_sync_bytes)();
void (mouse_update_position)();
int (mouse_write)(uint8_t command);
int (mouse_disable_data_reporting)();
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);

#endif /* _MOUSE_H_ */
