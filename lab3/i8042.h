#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

// Portas I/O
#define KBC_CMD_REG       0x64         // Registo de comandos do KBC (escrita)
#define KBC_STATUS_REG    0x64         // Registo de estado do KBC (leitura)
#define KBC_OUTPUT_BUF    0x60         // Buffer de saída do KBC
#define KBC_INPUT_BUF     0x60         // Buffer de entrada do KBC

// Bits do registo de estado
#define KBC_OUTPUT_BUFFER_FULL  BIT(0) // Output Buffer Full - tem dados para ler
#define KBC_INPUT_BUFFER_FULL   BIT(1) // Input Buffer Full - não pode receber comandos
#define KBC_STATUS_SYS          BIT(2) // System Flag
#define KBC_STATUS_A2           BIT(3) // A2 Input Line
#define KBC_STATUS_INH          BIT(4) // Keyboard Inhibited
#define KBC_STATUS_AUX          BIT(5) // Mouse Data
#define KBC_STATUS_TIMEOUT_ERR  BIT(6) // Timeout Error
#define KBC_STATUS_PARITY_ERR   BIT(7) // Parity Error

// Comandos para o KBC
#define KBC_READ_CMD    0x20           // Ler byte de comando
#define KBC_WRITE_CMD   0x60           // Escrever byte de comando
#define KBC_CHECK_KBC   0xAA           // Verificação do KBC (self-test)
#define KBC_CHECK_KB    0xAB           // Verificação da interface de teclado
#define KBC_DISABLE_KB  0xAD           // Desativar teclado
#define KBC_ENABLE_KB   0xAE           // Ativar teclado

// KBC Command Byte bits
#define KBC_CMD_INT BIT(0)             // Ativar interrupções do teclado
#define KBC_CMD_INT_MOUSE BIT(1)       // Ativar interrupções do rato
#define KBC_CMD_SYS BIT(2)             // System flag
#define KBC_CMD_INHIBIT BIT(4)         // Inibir teclado
#define KBC_CMD_AUX_INHIBIT BIT(5)     // Inibir rato

/* Scancodes */
#define ESC_BREAKCODE 0x81             // Breakcode da tecla ESC
#define SCANCODE_TWO_BYTE 0xE0         // Prefixo para scancodes de 2 bytes

/* Others */
#define KBC_IRQ 1                      // IRQ do teclado
#define MAX_ATTEMPTS 3                 // Número máximo de tentativas para ler/escrever
#define KBC_DELAY_US 20000             // Delay entre tentativas (20ms)

#endif
