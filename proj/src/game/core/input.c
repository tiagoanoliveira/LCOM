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
    InputEvent event = {INPUT_NONE, false};
    bool is_break_code = false;

    if(twoByte && scancode[0] == 0xE0) {
        // Verificar se é break code (bit 7 = 1)
        is_break_code = (scancode[1] & 0x80) != 0;
        uint8_t key = scancode[1] & 0x7F; // Remover bit de break
        switch(key) {
            case 0x4B: // Left arrow
                event.action = INPUT_LEFT;
                event.pressed = !is_break_code;
                break;
            case 0x4D: // Right arrow
                event.action = INPUT_RIGHT;
                event.pressed = !is_break_code;
                break;
            case 0x50: // Down arrow
                event.action = INPUT_DOWN;
                event.pressed = !is_break_code;
                break;
            case 0x48: // Up arrow
                event.action = INPUT_UP;
                event.pressed = !is_break_code;
                break;
        }
    } else {
        // Verificar se é break code
        is_break_code = (scancode[0] & 0x80) != 0;
        uint8_t key = scancode[0] & 0x7F;

        switch(key) {
            case 0x39: // Space
                event.action = INPUT_ROTATE;
                event.pressed = !is_break_code;
                break;
            case 0x1C: // Enter
                event.action = INPUT_ENTER;
                event.pressed = !is_break_code;
                break;
            case 0x01: // Escape
                event.action = INPUT_ESCAPE;
                event.pressed = !is_break_code;
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
