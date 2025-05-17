#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>
#define MAX_ATTEMPTS 10

int (kbc_read_status)(uint8_t* status);

int (kbc_read_output)(uint8_t port, uint8_t *output);//, uint8_t mouse);

int (kbc_write_command)(uint8_t port, uint8_t commandByte);

#endif /* _LCOM_KBC_H_ */
