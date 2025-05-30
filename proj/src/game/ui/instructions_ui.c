#include <string.h>
#include <stdio.h>
#include "include/instructions_ui.h"
#include "include/menu_ui.h"
#include "include/font.h"
#include "../../drivers/graphics/graphics.h"
#include "../core/include/config.h"

// Textos das instruções
static const char* INSTRUCTIONS_TITLE = "INSTRUCTIONS";
static const char* INSTRUCTIONS_TEXT[] = {
    "CONTROLS:",
    "",
    "         LEFT/RIGHT ARROW - Move piece left/right        ",
    "               DOWN ARROW - Move piece down              ",
    "                 UP ARROW - Rotate piece                 ",
    "SPACE/MIDDLE MOUSE BOTTOM - Drop piece                   ",
    "  LEFT/RIGHT MOUSE BOTTOM - Rotate piece to left or right",
    "",
    "GAME RULES:",
    "",
    "Fill complete horizontal lines to clean that line",
    "Game ends when pieces reach top",
    "Score and drop velocity increases with lines cleared",
    "",
    "PAUSE:",
    "",
    "Press ESC or Q to pause game",
};

#define INSTRUCTIONS_LINES (sizeof(INSTRUCTIONS_TEXT) / sizeof(INSTRUCTIONS_TEXT[0]))
static const char* BACK_OPTION = "BACK TO MENU";

void instructions_ui_draw(const InstructionsState* state) {
    if (!state || !state->active) return;

    // Limpar ecrã
    vg_clear_screen(COLOR_BACKGROUND);

    // Desenhar elementos
    draw_moldure();
    instructions_ui_draw_title();
    instructions_ui_draw_content();
    instructions_ui_draw_back_option();
}

void instructions_ui_draw_title(void) {
    const char* title = INSTRUCTIONS_TITLE;
    int title_width = strlen(title) * TITLE_CHAR_WIDTH;
    int title_x = (SCREEN_WIDTH - title_width) / 2;

    draw_text(true, title_x, TITLE_INSTRUCTIONS_ORIGIN_Y, title, COLOR_TITLE, COLOR_BACKGROUND);
}

void instructions_ui_draw_content(void) {
    int current_y = INSTRUCTIONS_ORIGIN_Y;

    for (int i = 0; i < (int)INSTRUCTIONS_LINES; i++) {
        int text_width = strlen(INSTRUCTIONS_TEXT[i]) * CHAR_WIDTH;
        int text_x = (SCREEN_WIDTH - text_width) / 2;

        uint32_t color = COLOR_NORMAL;
        if (strstr(INSTRUCTIONS_TEXT[i], "CONTROLS:") ||
            strstr(INSTRUCTIONS_TEXT[i], "GAME RULES:") ||
            strstr(INSTRUCTIONS_TEXT[i], "PAUSE:")) {
            color = COLOR_SELECTED;
            }

        draw_text(false, text_x, current_y, INSTRUCTIONS_TEXT[i], color, COLOR_BACKGROUND);

        // Calcular próxima posição Y
        current_y += SPACE_BETWEEN_EACH_LINE_OF_INSTRUCTIONS;

        // Extra space after titles and empty lines
        if (strlen(INSTRUCTIONS_TEXT[i]) == 0 ||  // Linha vazia
            strstr(INSTRUCTIONS_TEXT[i], "CONTROLS:") ||
            strstr(INSTRUCTIONS_TEXT[i], "GAME RULES:") ||
            strstr(INSTRUCTIONS_TEXT[i], "PAUSE:")) {
            current_y += 5;  // Extra space
            }
    }
}

void instructions_ui_draw_back_option(void) {
    draw_menu_option(SCREEN_WIDTH / 2, BUTTON_EXIT_INSTRUCTIONS_ORIGIN_Y, BACK_OPTION, true);
}
