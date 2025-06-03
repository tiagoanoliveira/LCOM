#ifndef _INPUT_H_
#define _INPUT_H_


#include <stdint.h>
#include <stdbool.h>

#define MAX_MOUSE_TICKS 20

typedef enum {
    INPUT_TYPE_KEYBOARD,
    INPUT_TYPE_MOUSE
} InputType;

typedef enum {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE,
    MOUSE_NONE
} MouseButton;

typedef enum {
    INPUT_NONE,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_DOWN,
    INPUT_ROTATE_LEFT,
    INPUT_ROTATE_RIGHT,
    INPUT_DROP,
    INPUT_UP,
    INPUT_ENTER,
    INPUT_ESCAPE,
    INPUT_Q,
    INPUT_CHAR
} InputAction;

typedef struct {
    InputType type;
    InputAction action;
    MouseButton button;
    bool pressed;
    char character;  // Used for INPUT_CHAR action
} InputEvent;

void input_init(void);
bool input_is_action_pressed(InputAction action);
InputEvent input_process_mouse_packet(uint8_t packet[3]);
InputEvent input_process_scancode(uint8_t scancode[2], bool twoByte);

#endif /* _INPUT_H_ */
