#include "include/input.h"
#include "../../drivers/keyboardMouse/keyboard.h"

static bool action_states[INPUT_ESCAPE + 1] = {false};

void input_init(void) {
    // Reset all action states
    for(int i = 0; i <= INPUT_ESCAPE; i++) {
        action_states[i] = false;
    }
}

InputEvent input_process_scancode(uint8_t scancode[2], bool twoByte) {
    InputEvent event = {INPUT_NONE, false, '\0'};

    if(twoByte && scancode[0] == 0xE0) {
        switch(scancode[1]) {
            case 0x4B: // Left arrow
                event.action = INPUT_LEFT;
                event.pressed = true;
                break;
            case 0x4D: // Right arrow
                event.action = INPUT_RIGHT;
                event.pressed = true;
                break;
            case 0x50: // Down arrow
                event.action = INPUT_DOWN;
                event.pressed = true;
                break;
            case 0x48: // Up arrow
                event.action = INPUT_UP;
                event.pressed = true;
                break;
        }
    } else {
        switch(scancode[0]) {
            case 0x39: // Space
                event.action = INPUT_ROTATE;
                event.pressed = true;
                break;
            case 0x1C: // Enter
                event.action = INPUT_ENTER;
                event.pressed = true;
                break;
            case 0x01: // Escape
                event.action = INPUT_ESCAPE;
                event.pressed = true;
                break;
        }
    }

    if(event.action != INPUT_NONE) {
        action_states[event.action] = event.pressed;
    }

    return event;
}

bool input_is_action_pressed(InputAction action) {
    return action_states[action];
}
