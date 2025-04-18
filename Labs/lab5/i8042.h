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
#define KBC_WRITE_TO_MOUSE 0xD4        // Escrever para o rato
#define KBC_DISABLE_MOUSE 0xA7         // Desativar interface do rato
#define KBC_ENABLE_MOUSE 0xA8          // Ativar interface do rato

// KBC Command Byte bits
#define KBC_CMD_INT BIT(0)             // Ativar interrupções do teclado
#define KBC_CMD_INT_MOUSE BIT(1)       // Ativar interrupções do rato
#define KBC_CMD_SYS BIT(2)             // System flag
#define KBC_CMD_INHIBIT BIT(4)         // Inibir teclado
#define KBC_CMD_AUX_INHIBIT BIT(5)     // Inibir rato

// Comandos do Rato
#define MOUSE_RESET 0xFF                // Reset ao rato
#define MOUSE_RESEND 0xFE               // Reenviar último byte
#define MOUSE_SET_DEFAULTS 0xF6         // Definir valores padrão
#define MOUSE_DISABLE_DATA_REPORTING 0xF5 // Desativar envio de dados (stream mode)
#define MOUSE_ENABLE_DATA_REPORTING 0xF4  // Ativar envio de dados (stream mode)
#define MOUSE_SET_SAMPLE_RATE 0xF3      // Definir taxa de amostragem
#define MOUSE_SET_REMOTE_MODE 0xF0      // Definir modo remoto
#define MOUSE_READ_DATA 0xEB            // Ler dados (apenas em modo remoto)
#define MOUSE_SET_STREAM_MODE 0xEA      // Definir modo stream
#define MOUSE_STATUS_REQUEST 0xE9       // Pedido de estado

// Respostas do Rato
#define MOUSE_ACK 0xFA                  // Reconhecimento (comando aceito)
#define MOUSE_NACK 0xFE                 // Não reconhecido (erro, reenviar)
#define MOUSE_ERROR 0xFC                // Erro (requer reset)

// Máscaras dos bits do primeiro byte do pacote do rato
#define MOUSE_LB BIT(0)                 // Botão esquerdo premido
#define MOUSE_RB BIT(1)                 // Botão direito premido
#define MOUSE_MB BIT(2)                 // Botão do meio premido
#define MOUSE_FIRST_BYTE_CHECK BIT(3)   // Sempre 1 no primeiro byte do pacote
#define MOUSE_X_SIGN BIT(4)             // Sinal do deslocamento em X
#define MOUSE_Y_SIGN BIT(5)             // Sinal do deslocamento em Y
#define MOUSE_X_OVERFLOW BIT(6)         // Overflow no deslocamento em X
#define MOUSE_Y_OVERFLOW BIT(7)         // Overflow no deslocamento em Y

// Others
#define KBC_IRQ 1                      // IRQ do teclado
#define MOUSE_IRQ 2                    // IRQ do mouse
#define MAX_ATTEMPTS 10                // Número máximo de tentativas para ler/escrever
#define KBC_DELAY_US 20000             // Delay entre tentativas (20ms)

#endif
