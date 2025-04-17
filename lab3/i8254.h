#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

#define CPU_FREQ 1193182   // clock frequency for timer in PC and AT
#define TIMER0_IRQ 0         // Timer 0 IRQ line

// I/O port addresses

#define TIMER_0    0x40     // Timer 0 count register
#define TIMER_1    0x41     // Timer 1 count register
#define TIMER_2    0x42     // Timer 2 count register
#define TIMER_CTRL 0x43     // Control register

#define SPEAKER_CTRL 0x61   // Register for speaker control

// Timer control

// Timer selection: bits 7 and 6

#define TIMER_SEL0   0x00              // Control Word for Timer 0
#define TIMER_SEL1   BIT(6)            // Control Word for Timer 1
#define TIMER_SEL2   BIT(7)            // Control Word for Timer 2
#define TIMER_RB_CMD (BIT(7) | BIT(6)) // Read Back Command

// Register selection: bits 5 and 4

#define TIMER_LSB     BIT(4)                  // Initialize Counter LSB only
#define TIMER_MSB     BIT(5)                  // Initialize Counter MSB only
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) // Initialize LSB first and MSB afterwards

// Operating mode: bits 3, 2 and 1

#define TIMER_SQR_WAVE (BIT(2) | BIT(1))     // Mode 3: square wave generator
#define TIMER_RATE_GEN BIT(2)                // Mode 2: rate generator

// Counting mode: bit 0

#define TIMER_BCD 0x01     // Count in BCD
#define TIMER_BIN 0x00     // Count in binary

// READ-BACK COMMAND FORMAT

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

#endif
