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
            case 0x4B: event.action = INPUT_LEFT; event.pressed = true; break;
            case 0x4D: event.action = INPUT_RIGHT; event.pressed = true; break;
            case 0x50: event.action = INPUT_DOWN; event.pressed = true; break;
            case 0x48: event.action = INPUT_UP; event.pressed = true; break;
        }
    } else {
        switch(scancode[0]) {
            case 0x39: event.action = INPUT_ROTATE; event.pressed = true; break;
            case 0x1C: event.action = INPUT_ENTER; event.pressed = true; break;
            case 0x01: event.action = INPUT_ESCAPE; event.pressed = true; break;
            // Alphanumeric keys (A-Z, 0-9)
            case 0x0E: // Backspace
                event.action = INPUT_CHAR;
                event.pressed = true;
                event.character = '\b';
                break;
            default:
                // Letters (A-Z)
                if ((scancode[0] >= 0x10 && scancode[0] <= 0x19) || // QWERTYUIOP
                    (scancode[0] >= 0x1E && scancode[0] <= 0x26) || // ASDFGHJKL
                    (scancode[0] >= 0x2C && scancode[0] <= 0x32))   // ZXCVBNM
                {
                    event.action = INPUT_CHAR;
                    event.pressed = true;
                    // Map scancode to ASCII uppercase letter
                    static const char scancode_to_char[256] = {
                        [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T',
                        [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P',
                        [0x1E] = 'A', [0x1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G',
                        [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L',
                        [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B',
                        [0x31] = 'N', [0x32] = 'M'
                    };
                    event.character = scancode_to_char[scancode[0]];
                }
                // Numbers (0-9)
                else if ((scancode[0] >= 0x02 && scancode[0] <= 0x0B)) {
                    event.action = INPUT_CHAR;
                    event.pressed = true;
                    static const char scancode_to_num[256] = {
                        [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5',
                        [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0'
                    };
                    event.character = scancode_to_num[scancode[0]];
                }
                break;
        }
    }

    if(event.action != INPUT_NONE && event.action != INPUT_CHAR) {
        action_states[event.action] = event.pressed;
    }

    return event;
}

bool input_is_action_pressed(InputAction action) {
    return action_states[action];
}
