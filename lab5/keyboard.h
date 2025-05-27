#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>
#include "i8042.h"
#include "KBC.h" 

uint8_t scancode;
bool two_type;
int error_flag;

int (kbd_subscribe_int)(uint8_t *bit_no); // Subscreve as interrupções do teclado

int (kbd_unsubscribe_int)(); // Cancela a subscrição das interrupções do teclado

void (kbc_ih)(); // Manipulador de interrupções do teclado

int (kbd_restore_interrupts)(); // Restaura a configuração do teclado, reativando as interrupções após polling

int wait_for_esc();

#endif
