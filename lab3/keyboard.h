#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "i8042.h"

// Global variables shared with handler
extern uint8_t scancode[2];     // Para guardar os bytes do scancode atual
extern bool two_byte;           // Indica se o scancode é de 2 bytes
extern uint32_t sys_inb_counter; // Contador de chamadas a sys_inb (LAB3)

// Interrupt handler
void (kbc_ih)(void);

// Auxiliar functions to interact with KBC
int (kbc_read_status)(uint8_t *status);
int (kbc_read_output)(uint8_t *data, uint8_t *status);

bool (kbc_has_parity_error)(uint8_t status);
bool (kbc_has_timeout_error)(uint8_t status);

int (kbc_write_command)(uint8_t cmd);

int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)(void);

#endif /* _LCOM_KEYBOARD_H_ */
