#include "include/font.h"
#include "../../drivers/graphics/graphics.h"
#include "../core/include/config.h"

#include <stdbool.h>
#include <string.h>
#include <ctype.h>

void draw_character(const bool is_title, const int x, const int y, const char c, const uint32_t fg_color, const uint32_t bg_color) {
    draw_rectangle(x, y, CHAR_WIDTH, CHAR_HEIGHT, bg_color); // Draw character background
    int letter_size=1;
    if (is_title) letter_size=TITLE_SCALE;
        switch (c) {
        case 'A':
            draw_rectangle(x + 5 * letter_size, y, 6 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 2 * letter_size, y + 3 * letter_size, 3 * letter_size, 18 * letter_size, fg_color);  // Left vertical line
            draw_rectangle(x + 11 * letter_size, y + 3 * letter_size, 3 * letter_size, 18 * letter_size, fg_color); // Right vertical line
            draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal line
            break;
        case 'B':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 5 * letter_size, y, 7 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal
            draw_rectangle(x + 5 * letter_size, y + 8 * letter_size, 7 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 7 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal
            draw_rectangle(x + 11 * letter_size, y + 2 * letter_size, 3 * letter_size, 7 * letter_size, fg_color);  // Right top vertical
            draw_rectangle(x + 11 * letter_size, y + 11 * letter_size, 3 * letter_size, 7 * letter_size, fg_color); // Right bottom vertical
            break;
        case 'C':
            draw_rectangle(x + 5 * letter_size, y, 9 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 2 * letter_size, y + 3 * letter_size, 3 * letter_size, 14 * letter_size, fg_color);  // Left vertical line
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 9 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal line
            break;
        case 'D':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 5 * letter_size, y, 6 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal
            draw_rectangle(x + 11 * letter_size, y + 3 * letter_size, 3 * letter_size, 14 * letter_size, fg_color); // Right vertical curve
            break;
        case 'E':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 5 * letter_size, y, 9 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal line
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 9 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal line
            break;
        case 'F':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 5 * letter_size, y, 9 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal line
            break;
        case 'G':
            draw_rectangle(x + 4 * letter_size, y, 10 * letter_size, 3 * letter_size, fg_color);                    // Top horizontal line
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 3 * letter_size, 16 * letter_size, fg_color);  // Left vertical line
            draw_rectangle(x + 4 * letter_size, y + 17 * letter_size, 10 * letter_size, 3 * letter_size, fg_color); // Bottom horizontal line
            draw_rectangle(x + 11 * letter_size, y + 13 * letter_size, 3 * letter_size, 5 * letter_size, fg_color); // Right vertical
            draw_rectangle(x + 8 * letter_size, y + 10 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);  // Middle horizontal (open G)
            break;
        case 'H':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical
            draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                   // Right vertical
            draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal
            break;
        case 'I':
            draw_rectangle(x + 2 * letter_size, y, 12 * letter_size, 3 * letter_size, fg_color);                    // Top horizontal line
            draw_rectangle(x + 6 * letter_size, y + 3 * letter_size, 3 * letter_size, 14 * letter_size, fg_color);  // Middle vertical line
            draw_rectangle(x + 2 * letter_size, y + 17 * letter_size, 12 * letter_size, 3 * letter_size, fg_color); // Bottom horizontal line
            break;
        case 'J':
            draw_rectangle(x + 5 * letter_size, y, 9 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal
            draw_rectangle(x + 11 * letter_size, y + 3 * letter_size, 3 * letter_size, 14 * letter_size, fg_color); // Middle vertical
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);  // Bottom curve
            draw_rectangle(x + 2 * letter_size, y + 10 * letter_size, 3 * letter_size, 7 * letter_size, fg_color);  // Left curve
            break;
        case 'K':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical
            draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 7 * letter_size, fg_color);                    // Right top vertical
            draw_rectangle(x + 11 * letter_size, y + 14 * letter_size, 3 * letter_size, 6 * letter_size, fg_color);                    // Right down vertical
            draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 4 * letter_size, 3 * letter_size, fg_color);   // Middle connector
            draw_rectangle(x + 9 * letter_size, y + 6 * letter_size, 3 * letter_size, 3 * letter_size, fg_color);   // Diagonal upper
            draw_rectangle(x + 9 * letter_size, y + 12 * letter_size, 3 * letter_size, 3 * letter_size, fg_color);  // Diagonal lower
            break;
        case 'L':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 9 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal line
            break;
        case 'M':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical
            draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                   // Right vertical
            draw_rectangle(x + 5 * letter_size, y + 3 * letter_size, 2 * letter_size, 3 * letter_size, fg_color);   // Middle left diagonal
            draw_rectangle(x + 7 * letter_size, y + 5 * letter_size, 2 * letter_size, 3 * letter_size, fg_color);   // Middle
            draw_rectangle(x + 9 * letter_size, y + 3 * letter_size, 2 * letter_size, 3 * letter_size, fg_color);   // Middle right diagonal
            break;
        case 'N':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                   // Right vertical line
            // Diagonal line
            draw_rectangle(x + 5 * letter_size, y + 4 * letter_size, 3 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 7 * letter_size, y + 8 * letter_size, 3 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 9 * letter_size, y + 12 * letter_size, 3 * letter_size, 4 * letter_size, fg_color);
            break;
        case 'O':
            draw_rectangle(x + 5 * letter_size, y, 6 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 3 * letter_size, 16 * letter_size, fg_color);  // Left vertical line
            draw_rectangle(x + 11 * letter_size, y + 2 * letter_size, 3 * letter_size, 16 * letter_size, fg_color); // Right vertical line
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal line
            break;
        case 'P':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 5 * letter_size, y, 6 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal line
            draw_rectangle(x + 11 * letter_size, y + 3 * letter_size, 3 * letter_size, 6 * letter_size, fg_color);  // Right top vertical line
            break;
        case 'Q':
            draw_rectangle(x + 5 * letter_size, y, 6 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 3 * letter_size, 14 * letter_size, fg_color);  // Left vertical line
            draw_rectangle(x + 11 * letter_size, y + 2 * letter_size, 3 * letter_size, 14 * letter_size, fg_color); // Right vertical line
            draw_rectangle(x + 5 * letter_size, y + 15 * letter_size, 7 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal line
            // Tail (diagonal)
            draw_rectangle(x + 7 * letter_size, y + 11 * letter_size, 2 * letter_size, 2 * letter_size, fg_color);
            draw_rectangle(x + 9 * letter_size, y + 13 * letter_size, 3 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 12 * letter_size, y + 17 * letter_size, 2 * letter_size, 3 * letter_size, fg_color);
            break;
        case 'R':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 5 * letter_size, y, 6 * letter_size, 3 * letter_size, fg_color);                     // Top horizontal line
            draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal line
            draw_rectangle(x + 11 * letter_size, y + 3 * letter_size, 3 * letter_size, 6 * letter_size, fg_color);  // Right top vertical line
            draw_rectangle(x + 11 * letter_size, y + 12 * letter_size, 3 * letter_size, 8 * letter_size, fg_color); // Right bottom diagonal
            break;
        case 'S':
            draw_rectangle(x + 2 * letter_size, y, 12 * letter_size, 3 * letter_size, fg_color);                    // Top horizontal line
            draw_rectangle(x + 2 * letter_size, y + 3 * letter_size, 3 * letter_size, 5 * letter_size, fg_color);   // Left top vertical line
            draw_rectangle(x + 2 * letter_size, y + 8 * letter_size, 12 * letter_size, 3 * letter_size, fg_color);  // Middle horizontal line
            draw_rectangle(x + 11 * letter_size, y + 11 * letter_size, 3 * letter_size, 6 * letter_size, fg_color); // Right bottom vertical line
            draw_rectangle(x + 2 * letter_size, y + 17 * letter_size, 12 * letter_size, 3 * letter_size, fg_color); // Bottom horizontal line
            break;
        case 'T':
            draw_rectangle(x + 2 * letter_size, y, 12 * letter_size, 3 * letter_size, fg_color);                    // Top horizontal line
            draw_rectangle(x + 6 * letter_size, y + 3 * letter_size, 3 * letter_size, 17 * letter_size, fg_color);  // Middle vertical line
            break;
        case 'U':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 18 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 18 * letter_size, fg_color);                   // Right vertical line
            draw_rectangle(x + 5 * letter_size, y + 17 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);  // Bottom horizontal line
            break;
        case 'V':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 15 * letter_size, fg_color);                    // Left vertical line
            draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 15 * letter_size, fg_color);                   // Right vertical line
            draw_rectangle(x + 5 * letter_size, y + 13 * letter_size, 2 * letter_size, 4 * letter_size, fg_color);  // Left bottom connector
            draw_rectangle(x + 9 * letter_size, y + 13 * letter_size, 2 * letter_size, 4 * letter_size, fg_color);  // Right bottom connector
            draw_rectangle(x + 7 * letter_size, y + 17 * letter_size, 2 * letter_size, 4 * letter_size, fg_color);  // Bottom connector
            break;
        case 'W':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                    // Left vertical
            draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 20 * letter_size, fg_color);                   // Right vertical
            draw_rectangle(x + 5 * letter_size, y + 15 * letter_size, 2 * letter_size, 3 * letter_size, fg_color);   // Middle left diagonal
            draw_rectangle(x + 7 * letter_size, y + 13 * letter_size, 2 * letter_size, 3 * letter_size, fg_color);   // Middle
            draw_rectangle(x + 9 * letter_size, y + 15 * letter_size, 2 * letter_size, 3 * letter_size, fg_color);   // Middle right diagonal
            break;
        case 'X':
                draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 9 * letter_size, fg_color);                     // Left top vertical line
                draw_rectangle(x + 2 * letter_size, y + 12 * letter_size, 3 * letter_size, 8 * letter_size, fg_color);  // Left bottom vertical line
                draw_rectangle(x + 5 * letter_size, y + 9 * letter_size, 6 * letter_size, 3 * letter_size, fg_color);   // Middle horizontal line
                draw_rectangle(x + 11 * letter_size, y, 3 * letter_size, 9 * letter_size, fg_color);                    // Right top vertical line
                draw_rectangle(x + 11 * letter_size, y + 12 * letter_size, 3 * letter_size, 8 * letter_size, fg_color); // Right bottom diagonal
            break;
        case 'Y':
            draw_rectangle(x + 2 * letter_size, y, 3 * letter_size, 5 * letter_size, fg_color);                         // Left top diagonal
            draw_rectangle(x + 10 * letter_size, y, 3 * letter_size, 5 * letter_size, fg_color);                        // Right top diagonal
            draw_rectangle(x + 4 * letter_size, y + 4 * letter_size, 2 * letter_size, 4 * letter_size, fg_color);      // Left diagonal connector
            draw_rectangle(x + 9 * letter_size, y + 4 * letter_size, 2 * letter_size, 4 * letter_size, fg_color);      // Right diagonal connector
            draw_rectangle(x + 6 * letter_size, y + 7 * letter_size, 3 * letter_size, 13 * letter_size, fg_color);      // Middle bottom vertical
            break;
        case 'Z':
            draw_rectangle(x + 2 * letter_size, y, 12 * letter_size, 3 * letter_size, fg_color);                        // Top horizontal
            // Diagonal
            draw_rectangle(x + 11 * letter_size, y + 3 * letter_size, 3 * letter_size, 3 * letter_size, fg_color);
            draw_rectangle(x + 9 * letter_size, y + 6 * letter_size, 3 * letter_size, 3 * letter_size, fg_color);
            draw_rectangle(x + 6 * letter_size, y + 8 * letter_size, 3 * letter_size, 3 * letter_size, fg_color);
            draw_rectangle(x + 4 * letter_size, y + 11 * letter_size, 3 * letter_size, 3 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 14 * letter_size, 3 * letter_size, 3 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 17 * letter_size, 12 * letter_size, 3 * letter_size, fg_color);     // Bottom horizontal
            break;
        case '0':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);      // Top
            draw_rectangle(x + 2 * letter_size, y + 18 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);     // Bottom
            draw_rectangle(x + 2 * letter_size, y + 6 * letter_size, 4 * letter_size, 12 * letter_size, fg_color);      // Left
            draw_rectangle(x + 10 * letter_size, y + 6 * letter_size, 4 * letter_size, 12 * letter_size, fg_color);     // Right
            break;
        case '1':
            draw_rectangle(x + 6 * letter_size, y + 2 * letter_size, 4 * letter_size, 20 * letter_size, fg_color);
            break;
        case '2':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);      // Top
            draw_rectangle(x + 10 * letter_size, y + 6 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);      // Top right
            draw_rectangle(x + 2 * letter_size, y + 10 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);     // Middle
            draw_rectangle(x + 2 * letter_size, y + 14 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);      // Bottom left
            draw_rectangle(x + 2 * letter_size, y + 18 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);     // Bottom
            break;
        case '3':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 6 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 6 * letter_size, y + 10 * letter_size, 6 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 14 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 18 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            break;
        case '4':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 4 * letter_size, 12 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 2 * letter_size, 4 * letter_size, 20 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 10 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            break;
        case '5':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 4 * letter_size, 10 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 10 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 14 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 18 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            break;
        case '6':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 6 * letter_size, 4 * letter_size, 16 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 10 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 14 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 18 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            break;
        case '7':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 6 * letter_size, 4 * letter_size, 16 * letter_size, fg_color);
            break;
        case '8':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 6 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 6 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 10 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 14 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 14 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 18 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            break;
        case '9':
            draw_rectangle(x + 2 * letter_size, y + 2 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 6 * letter_size, 4 * letter_size, 6 * letter_size, fg_color);
            draw_rectangle(x + 10 * letter_size, y + 6 * letter_size, 4 * letter_size, 12 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 10 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 18 * letter_size, 12 * letter_size, 4 * letter_size, fg_color);
            break;

        case '-':
            // Horizontal line
            draw_rectangle(x + 3 * letter_size, y + 8 * letter_size, 10 * letter_size, 3 * letter_size, fg_color);
            break;

        case '/':
            // Diagonal line (simplified as stepped blocks)
            draw_rectangle(x + 10 * letter_size, y, 3 * letter_size, 4 * letter_size, fg_color);
            draw_rectangle(x + 8 * letter_size, y + 3 * letter_size, 3 * letter_size, 5 * letter_size, fg_color);
            draw_rectangle(x + 6 * letter_size, y + 7 * letter_size, 3 * letter_size, 5 * letter_size, fg_color);
            draw_rectangle(x + 4 * letter_size, y + 11 * letter_size, 3 * letter_size, 5 * letter_size, fg_color);
            draw_rectangle(x + 2 * letter_size, y + 15 * letter_size, 3 * letter_size, 5 * letter_size, fg_color);
            break;
        case ':':
            draw_rectangle(x + 4, y + 7, 3, 3, fg_color);
            draw_rectangle(x + 4, y + 13, 3, 3, fg_color);
            break;
        case '.':
            draw_rectangle(x + 4, y + 16, 4, 4, fg_color);
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

void draw_text(const bool is_title, const int x, const int y, const char* text, const uint32_t fg_color, const uint32_t bg_color) {
    if (!text) return;
    int current_x = x;
    for (int i = 0; i < (int)strlen(text); i++) {
        if (!is_title) {
            const char upper = toupper((unsigned char)text[i]);
            draw_character(is_title, current_x, y, upper, fg_color, bg_color);
            current_x += CHAR_WIDTH;
        }
        else {
            draw_character(true, current_x, y, text[i], fg_color, bg_color);
            current_x += TITLE_CHAR_WIDTH;
        }
    }
}

void draw_moldure(void) {
    // Desenhar moldura
    draw_rectangle(SCREEN_BORDER_WIDTH, SCREEN_BORDER_WIDTH, SCREEN_WIDTH - (SCREEN_BORDER_WIDTH * 2), FRAME_THICKNESS, COLOR_BORDER); // linha superior
    draw_rectangle(SCREEN_BORDER_WIDTH, SCREEN_HEIGHT - (SCREEN_BORDER_WIDTH + FRAME_THICKNESS), SCREEN_WIDTH - (SCREEN_BORDER_WIDTH * 2), FRAME_THICKNESS, COLOR_BORDER); // linha inferior
    draw_rectangle(SCREEN_BORDER_WIDTH, SCREEN_BORDER_WIDTH, FRAME_THICKNESS, SCREEN_HEIGHT - (SCREEN_BORDER_WIDTH * 2), COLOR_BORDER); // linha esquerda
    draw_rectangle(SCREEN_WIDTH - (SCREEN_BORDER_WIDTH + FRAME_THICKNESS), SCREEN_BORDER_WIDTH, FRAME_THICKNESS, SCREEN_HEIGHT - (SCREEN_BORDER_WIDTH * 2), COLOR_BORDER); // linha direita
}

void draw_decoration() {
    // Blocos decorativos
    for (int i = 0; i < 8; i++) {
        int x = 40 + i * (SCREEN_WIDTH - 100) / 7;
        int y = SCREEN_HEIGHT - 80;
        draw_rectangle(x, y, 25, 25, COLOR_BORDER);
        draw_rectangle(x + 2, y + 2, 21, 21, COLOR_SELECTED);
    }
}
