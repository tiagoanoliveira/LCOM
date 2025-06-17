#ifndef INSTRUCTIONS_UI_H
#define INSTRUCTIONS_UI_H

#include "../../states/include/instructions_state.h"

void instructions_ui_draw(const InstructionsState* state);
void instructions_ui_draw_background(void);
void instructions_ui_draw_title(void);
void instructions_ui_draw_content(void);
void instructions_ui_draw_back_option(void);

#endif //INSTRUCTIONS_UI_H
