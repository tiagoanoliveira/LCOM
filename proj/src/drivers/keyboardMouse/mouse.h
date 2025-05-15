#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "KBC.h"
#include "i8042.h"
#include "i8254.h"

void (mouse_ih)();
int (mouse_write)(uint8_t command);
int (mouse_disable_data_reporting)();
void (parse_packet)();
int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)(void);
void (mouse_sync_bytes)();

#endif /* _LCOM_MOUSE_H_ */
