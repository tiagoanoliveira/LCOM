#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <lcom/lcf.h>
#include "i8042.h"

/**
 * @brief Subscreve as interrupções do teclado
 *
 * @param bit_no Endereço da variável onde será guardada a máscara para detetar interrupções
 * @return int 0 em caso de sucesso, 1 caso contrário
 */
int keyboard_subscribe_int(uint8_t *bit_no);

/**
 * @brief Cancela a subscrição das interrupções do teclado
 *
 * @return int 0 em caso de sucesso, 1 caso contrário
 */
int keyboard_unsubscribe_int();

/**
 * @brief Manipulador de interrupções do teclado
 */
void kbc_ih();

/**
 * @brief Restaura a configuração do teclado, ativando as interrupções
 *
 * @return int 0 em caso de sucesso, 1 caso contrário
 */
int keyboard_restore();

/**
 * @brief Obtém o scancode mais recente recebido do teclado
 *
 * @return uint8_t Scancode recebido
 */
uint8_t keyboard_get_scancode();

/**
 * @brief Lê o próximo scancode do teclado em modo polling
 *
 * @param scancode Apontador para onde o scancode será escrito
 * @return int 0 em caso de sucesso, 1 caso contrário
 */
int keyboard_read_scancode(uint8_t *scancode);

#endif
