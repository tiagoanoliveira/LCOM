#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>
#include "../utils/utils.h"
#include "../timer/i8254.h"
#include "KBC.h"
#include "i8042.h"

int mouse_init();
int mouse_exit();

void (mouse_ih)();
int (mouse_write)(uint8_t command);
int (mouse_disable_data_reporting)();
void (parse_packet)();
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);
void (mouse_sync_bytes)();

#endif /* _MOUSE_H_ */
