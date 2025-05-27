#ifndef _I8042_H_
#define _I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller (KBC)
 */

 // IRQ
#define IRQ_KEYBOARD 1
#define IRQ_MOUSE 12

 // I/O Ports
#define KBC_OUTPUT_BUF 0x60
#define KBC_INPUT_BUF 0x60
#define KBC_CMD_REG 0x64
#define KBC_STATUS_REG 0x64
#define KBC_WRITE_TO_MOUSE 0xD4

// Status Register Bits
#define KBC_STATUS_PARITY_ERR BIT(7)
#define KBC_STATUS_TIMEOUT_ERR BIT(6)
#define KBC_STATUS_AUX BIT(5) // 1 if from mouse, 0 if from keyboard
#define KBC_INPUT_BUFFER_FULL BIT(1)
#define KBC_OUTPUT_BUFFER_FULL BIT(0)

// KBC Commands
#define KBC_READ_CMD_BYTE 0x20
#define KBC_WRITE_CMD_BYTE 0x60

// Command Byte Bits
#define KBC_CMD_BYTE_ENABLE_INT_KBD BIT(0) // Enable keyboard interrupts
#define KBC_CMD_BYTE_ENABLE_INT_MOUSE BIT(1) // Enable mouse interrupts
#define KBC_CMD_BYTE_DISABLE_KBD BIT(4) // Disable keyboard interface
#define KBC_CMD_BYTE_DISABLE_MOUSE BIT(5) // Disable mouse interface

// Scancode Constants
#define ESC_BREAK_CODE 0x81
#define SCANCODE_TWO_BYTE 0xE0

// Delay between polling attempts (in microseconds)
#define KBC_DELAY_US 20000
#define MAX_ATTEMPS 10

// Mouse Commands
#define MOUSE_LB            BIT(0)
#define MOUSE_RB            BIT(1)
#define MOUSE_MB            BIT(2)
#define FIRST_BYTE          BIT(3)
#define MOUSE_X_SIGNAL      BIT(4)
#define MOUSE_Y_SIGNAL      BIT(5)
#define MOUSE_X_OVERFLOW    BIT(6)
#define MOUSE_Y_OVERFLOW    BIT(7)
#define MOUSE_RESET 0xFF
#define MOUSE_RESEND 0xFE
#define MOUSE_SET_DEFAULTS 0xF6
#define MOUSE_DISABLE_DATA_REPORTING 0xF5
#define MOUSE_ENABLE_DATA_REPORTING 0xF4
#define MOUSE_SET_SAMPLE_RATE 0xF3
#define MOUSE_SET_STREAM_MODE 0xEA
#define MOUSE_SET_REMOTE_MODE 0xF0
#define MOUSE_READ_DATA 0xEB
#define MOUSE_STATUS_REQUEST 0xE9
#define MOUSE_SET_RESOLUTION 0xE8
#define MOUSE_SET_SCALING_2_1 0xE7
#define MOUSE_SET_SCALING_1_1 0xE6

// Mouse acknowledgment bytes
#define MOUSE_ACK 0xFA
#define MOUSE_NACK 0xFE
#define MOUSE_ERROR 0xFC

#endif /* _I8042_H_ */
