//
// Created by tiago-oliveira on 27-05-2025.
//

#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    INPUT_NONE,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_DOWN,
    INPUT_ROTATE,
    INPUT_DROP,
    INPUT_UP,
    INPUT_ENTER,
    INPUT_ESCAPE
} InputAction;

typedef struct {
    InputAction action;
    bool pressed;
} InputEvent;

// Sistema de input
void input_init(void);
InputEvent input_process_scancode(uint8_t scancode[2], bool twoByte);
bool input_is_action_pressed(InputAction action);

#endif

