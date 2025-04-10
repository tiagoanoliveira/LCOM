#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard Controller (KBC)
 */

 // IRQ
#define IRQ_KEYBOARD 1

 // I/O Ports
#define KBC_OUTPUT_BUF 0x60
#define KBC_INPUT_BUF 0x60
#define KBC_CMD_REG 0x64
#define KBC_STATUS_REG 0x64

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

#endif /* _LCOM_I8042_H_ */
