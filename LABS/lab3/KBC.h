#ifndef _KBC_H
#define _KBC_H

#include <lcom/lcf.h>
#include <stdint.h>

int kbc_read_status(uint8_t *status); // Lê o registo de estado do KBC

int kbc_read_output(uint8_t port, uint8_t *data); // Lê dados do buffer de saída do KBC (com verificação de erros)

int kbc_write_command(uint8_t port, uint8_t command); // Escreve um comando no KBC

#endif
