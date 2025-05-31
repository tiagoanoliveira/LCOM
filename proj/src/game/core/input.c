#include "include/input.h"
#include "../../drivers/keyboardMouse/keyboard.h"

static bool action_states[INPUT_ESCAPE + 1] = {false};

void input_init(void) {
    // Reset all action states
    for(int i = 0; i <= INPUT_ESCAPE; i++) {
        action_states[i] = false;
    }
}

InputEvent input_process_scancode(uint8_t scancode[2], const bool twoByte) {
    InputEvent event = {INPUT_TYPE_KEYBOARD, INPUT_NONE, 0, false, '\0'};
    bool is_break_code = false;

    if (twoByte && scancode[0] == 0xE0) {
        is_break_code = (scancode[1] & 0x80) != 0;
        const uint8_t key = scancode[1] & 0x7F;

        switch (key) {
            case 0x4B: event.action = INPUT_LEFT; break;
            case 0x4D: event.action = INPUT_RIGHT; break;
            case 0x50: event.action = INPUT_DOWN; break;
            case 0x48: event.action = INPUT_ROTATE_LEFT; break;
        }
        event.pressed = !is_break_code;
    } else {
        is_break_code = (scancode[0] & 0x80) != 0;
        const uint8_t key = scancode[0] & 0x7F;

        switch (key) {
            case 0x39: // Space
                event.action = INPUT_DROP;
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
            case 0x10: // Q
                event.action = INPUT_Q;
                event.pressed = !is_break_code;
                break;
                // NOVAS TECLAS WASD:
            case 0x1E: // A - move left
                event.action = INPUT_LEFT;
                event.pressed = !is_break_code;
                break;
            case 0x20: // D - move right
                event.action = INPUT_RIGHT;
                event.pressed = !is_break_code;
                break;
            case 0x1F: // S - move down
                event.action = INPUT_DOWN;
                event.pressed = !is_break_code;
                break;
            case 0x11: // W - rotate
                event.action = INPUT_ROTATE_LEFT;
                event.pressed = !is_break_code;
                break;
            case 0x0E: // Backspace
                event.action = INPUT_CHAR;
                event.character = '\b';
                break;
            default:
                // Letters A-Z
                if ((key >= 0x10 && key <= 0x19) ||
                    (key >= 0x1E && key <= 0x26) ||
                    (key >= 0x2C && key <= 0x32)) {
                    static const char scancode_to_char[256] = {
                        [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T',
                        [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P',
                        [0x1E] = 'A', [0x1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G',
                        [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L',
                        [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B',
                        [0x31] = 'N', [0x32] = 'M'
                    };
                    event.action = INPUT_CHAR;
                    event.character = scancode_to_char[key];
                }
                // Numbers 0–9
                else if (key >= 0x02 && key <= 0x0B) {
                    static const char scancode_to_num[256] = {
                        [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5',
                        [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0'
                    };
                    event.action = INPUT_CHAR;
                    event.character = scancode_to_num[key];
                }
                break;
        }
        event.pressed = !is_break_code;
    }

    if (event.action != INPUT_NONE && event.action != INPUT_CHAR) {
        action_states[event.action] = event.pressed;
    }

    return event;
}

InputEvent input_process_mouse_packet(uint8_t packet[3]) {
    InputEvent event = {INPUT_TYPE_MOUSE, INPUT_NONE, 0, false, '\0'};

    if (packet[0] & BIT(0)) {           // Left click
        event.action = INPUT_ROTATE_LEFT;
        event.pressed = true;
        event.button = MOUSE_LEFT;
    } else if (packet[0] & BIT(1)) {    // Right click
        event.action = INPUT_ROTATE_RIGHT;
        event.pressed = true;
        event.button = MOUSE_RIGHT;
    } else if (packet[0] & BIT(2)) {    // Middle click
        event.action = INPUT_DROP;      // Hard drop
        event.pressed = true;
        event.button = MOUSE_MIDDLE;
    }

    return event;
}

bool input_is_action_pressed(const InputAction action) {
    return action_states[action];
}
