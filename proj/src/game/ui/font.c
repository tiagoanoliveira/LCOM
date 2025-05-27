#include "font.h"
#include <string.h>

void draw_character(int x, int y, char c, uint32_t fg_color, uint32_t bg_color) {
    // Draw character background
    draw_rectangle(x, y, CHAR_WIDTH, CHAR_HEIGHT, bg_color);

    // Draw character foreground based on character
    switch (c) {
        case 'A':
            // Top horizontal line
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Left vertical line
            draw_rectangle(x + 2, y + 6, 4, 16, fg_color);
            // Right vertical line
            draw_rectangle(x + 10, y + 6, 4, 16, fg_color);
            // Middle horizontal line
            draw_rectangle(x + 6, y + 10, 4, 4, fg_color);
            break;
        case 'B':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Top horizontal
            draw_rectangle(x + 6, y + 2, 8, 4, fg_color);
            // Middle horizontal
            draw_rectangle(x + 6, y + 10, 8, 4, fg_color);
            // Bottom horizontal
            draw_rectangle(x + 6, y + 18, 8, 4, fg_color);
            // Right top vertical
            draw_rectangle(x + 12, y + 6, 2, 6, fg_color);
            // Right bottom vertical
            draw_rectangle(x + 12, y + 14, 2, 6, fg_color);
            break;
        case 'C':
            // Top horizontal line
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Left vertical line
            draw_rectangle(x + 2, y + 6, 4, 12, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 2, y + 18, 12, 4, fg_color);
            break;
        case 'D':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Top horizontal
            draw_rectangle(x + 6, y + 2, 8, 4, fg_color);
            // Bottom horizontal
            draw_rectangle(x + 6, y + 18, 8, 4, fg_color);
            // Right vertical curve
            draw_rectangle(x + 12, y + 6, 2, 12, fg_color);
            break;
        case 'E':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Top horizontal line
            draw_rectangle(x + 6, y + 2, 8, 4, fg_color);
            // Middle horizontal line
            draw_rectangle(x + 6, y + 10, 6, 4, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 6, y + 18, 8, 4, fg_color);
            break;
        case 'F':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Top horizontal
            draw_rectangle(x + 6, y + 2, 8, 4, fg_color);
            // Middle horizontal
            draw_rectangle(x + 6, y + 10, 6, 4, fg_color);
            break;
        case 'G':
            // Top horizontal
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Left vertical
            draw_rectangle(x + 2, y + 6, 4, 12, fg_color);
            // Bottom horizontal
            draw_rectangle(x + 2, y + 18, 12, 4, fg_color);
            // Right vertical
            draw_rectangle(x + 10, y + 10, 4, 8, fg_color);
            // Middle horizontal (open G)
            draw_rectangle(x + 8, y + 10, 6, 4, fg_color);
            break;

        case 'H':
            // Left vertical
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Right vertical
            draw_rectangle(x + 10, y + 2, 4, 20, fg_color);
            // Middle horizontal
            draw_rectangle(x + 6, y + 10, 4, 4, fg_color);
            break;
        case 'I':
            // Top horizontal line
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Middle vertical line
            draw_rectangle(x + 6, y + 6, 4, 12, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 2, y + 18, 12, 4, fg_color);
            break;
        case 'J':
            // Top horizontal
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Middle vertical
            draw_rectangle(x + 8, y + 6, 4, 12, fg_color);
            // Bottom curve
            draw_rectangle(x + 2, y + 18, 6, 4, fg_color);
            break;
        case 'K':
            // Left vertical
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Diagonal upper
            draw_rectangle(x + 6, y + 10, 8, 4, fg_color);
            // Diagonal lower
            draw_rectangle(x + 6, y + 14, 8, 4, fg_color);
            // Middle connector
            draw_rectangle(x + 8, y + 10, 4, 4, fg_color);
            break;
        case 'L':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 6, y + 18, 8, 4, fg_color);
            break;
        case 'M':
            // Left vertical
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Right vertical
            draw_rectangle(x + 10, y + 2, 4, 20, fg_color);
            // Middle left diagonal
            draw_rectangle(x + 6, y + 6, 4, 4, fg_color);
            // Middle right diagonal
            draw_rectangle(x + 8, y + 6, 4, 4, fg_color);
            break;
        case 'N':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Right vertical line
            draw_rectangle(x + 10, y + 2, 4, 20, fg_color);
            // Diagonal line (simplified as stepped blocks)
            draw_rectangle(x + 4, y + 6, 4, 4, fg_color);
            draw_rectangle(x + 6, y + 10, 4, 4, fg_color);
            draw_rectangle(x + 8, y + 14, 4, 4, fg_color);
            break;

        case 'O':
            // Top horizontal line
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Left vertical line
            draw_rectangle(x + 2, y + 6, 4, 12, fg_color);
            // Right vertical line
            draw_rectangle(x + 10, y + 6, 4, 12, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 2, y + 18, 12, 4, fg_color);
            break;

        case 'P':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Top horizontal line
            draw_rectangle(x + 6, y + 2, 8, 4, fg_color);
            // Middle horizontal line
            draw_rectangle(x + 6, y + 10, 8, 4, fg_color);
            // Right top vertical line
            draw_rectangle(x + 10, y + 6, 4, 4, fg_color);
            break;

        case 'Q':
            // Top horizontal line
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Left vertical line
            draw_rectangle(x + 2, y + 6, 4, 12, fg_color);
            // Right vertical line
            draw_rectangle(x + 10, y + 6, 4, 12, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 2, y + 18, 12, 4, fg_color);
            // Tail (diagonal)
            draw_rectangle(x + 8, y + 14, 6, 6, fg_color);
            break;

        case 'R':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 20, fg_color);
            // Top horizontal line
            draw_rectangle(x + 6, y + 2, 8, 4, fg_color);
            // Middle horizontal line
            draw_rectangle(x + 6, y + 10, 6, 4, fg_color);
            // Right top vertical line
            draw_rectangle(x + 10, y + 6, 4, 4, fg_color);
            // Right bottom diagonal
            draw_rectangle(x + 8, y + 14, 6, 8, fg_color);
            break;

        case 'S':
            // Top horizontal line
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Left top vertical line
            draw_rectangle(x + 2, y + 6, 4, 6, fg_color);
            // Middle horizontal line
            draw_rectangle(x + 6, y + 10, 4, 4, fg_color);
            // Right bottom vertical line
            draw_rectangle(x + 10, y + 14, 4, 4, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 2, y + 18, 12, 4, fg_color);
            break;

        case 'T':
            // Top horizontal line
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Middle vertical line
            draw_rectangle(x + 6, y + 6, 4, 16, fg_color);
            break;

        case 'U':
            // Left vertical line
            draw_rectangle(x + 2, y + 2, 4, 16, fg_color);
            // Right vertical line
            draw_rectangle(x + 10, y + 2, 4, 16, fg_color);
            // Bottom horizontal line
            draw_rectangle(x + 5, y + 18, 6, 4, fg_color);
            break;
        case 'V':
            // Left diagonal
            draw_rectangle(x + 2, y + 2, 4, 16, fg_color);
            // Right diagonal
            draw_rectangle(x + 10, y + 2, 4, 16, fg_color);
            // Bottom connector
            draw_rectangle(x + 6, y + 18, 4, 4, fg_color);
            break;

        case 'W':
            // Left vertical
            draw_rectangle(x + 2, y + 2, 4, 16, fg_color);
            // Right vertical
            draw_rectangle(x + 10, y + 2, 4, 16, fg_color);
            // Middle bottom left
            draw_rectangle(x + 6, y + 14, 2, 8, fg_color);
            // Middle bottom right
            draw_rectangle(x + 8, y + 14, 2, 8, fg_color);
            break;

        case 'X':
            // Diagonal from top-left to bottom-right
            draw_rectangle(x + 2, y + 2, 4, 8, fg_color);
            draw_rectangle(x + 10, y + 10, 4, 8, fg_color);
            // Diagonal from top-right to bottom-left
            draw_rectangle(x + 10, y + 2, 4, 8, fg_color);
            draw_rectangle(x + 2, y + 10, 4, 8, fg_color);
            break;
        case 'Y':
            // Left top diagonal
            draw_rectangle(x + 2, y + 2, 4, 8, fg_color);
            // Right top diagonal
            draw_rectangle(x + 10, y + 2, 4, 8, fg_color);
            // Middle bottom vertical
            draw_rectangle(x + 6, y + 10, 4, 12, fg_color);
            break;
        case 'Z':
            // Top horizontal
            draw_rectangle(x + 2, y + 2, 12, 4, fg_color);
            // Diagonal
            draw_rectangle(x + 10, y + 6, 4, 4, fg_color);
            draw_rectangle(x + 6, y + 10, 4, 4, fg_color);
            draw_rectangle(x + 2, y + 14, 4, 4, fg_color);
            // Bottom horizontal
            draw_rectangle(x + 2, y + 18, 12, 4, fg_color);
            break;
        case '-':
            // Horizontal line
            draw_rectangle(x + 4, y + 10, 8, 4, fg_color);
            break;

        case '/':
            // Diagonal line (simplified as stepped blocks)
            draw_rectangle(x + 10, y + 2, 4, 8, fg_color);
            draw_rectangle(x + 6, y + 8, 4, 8, fg_color);
            draw_rectangle(x + 2, y + 14, 4, 8, fg_color);
            break;

        case ' ':
            // Space - just background, no foreground
            break;

        default:
            // For unknown characters, draw a simple rectangle
            draw_rectangle(x + 4, y + 4, 8, 16, fg_color);
            break;
    }
}

void draw_text(int x, int y, const char* text, uint32_t fg_color, uint32_t bg_color) {
    if (!text) return;

    int current_x = x;
    for (int i = 0; i < (int)strlen(text); i++) {
        draw_character(current_x, y, text[i], fg_color, bg_color);
        current_x += CHAR_WIDTH;
    }
}
