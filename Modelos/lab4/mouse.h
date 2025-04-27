#ifndef MOUSE_H
#define MOUSE_H

#include <minix/sysutil.h>
#include <stdint.h>
#include <stdio.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h"
#include "utils.c"

int (mouse_subscribe_int)(uint8_t *bit_no);
int (mouse_unsubscribe_int)();
void (mouse_ih)();
int (mouse_sync_bytes)();
void (mouse_bytes_to_packet)();
int (mouse_write_cmd)(uint8_t cmd);

#endif //MOUSE_H
