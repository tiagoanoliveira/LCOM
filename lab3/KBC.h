#ifndef __KBC_H
#define __KBC_H

#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Lê o registo de estado do KBC
 *
 * @param status Apontador para a variável onde será armazenado o estado
 * @return int 0 em caso de sucesso, 1 caso contrário
 */
int kbc_read_status(uint8_t *status);

/**
 * @brief Lê dados do buffer de saída do KBC
 *
 * @param port Porta de onde ler (0x60 para dados)
 * @param data Apontador para a variável onde será armazenado o que for lido
 * @return int 0 em caso de sucesso, 1 caso contrário
 */
int kbc_read_output(uint8_t port, uint8_t *data);

/**
 * @brief Envia um comando para o KBC
 *
 * @param port Porta para a qual enviar o comando (0x64 para comandos)
 * @param cmd Comando a enviar
 * @return int 0 em caso de sucesso, 1 caso contrário
 */
int kbc_write_command(uint8_t port, uint8_t cmd);




#endif
