#ifndef HIGHSCORE_UI_H
#define HIGHSCORE_UI_H

#include "font.h"
#include "menu_ui.h"
#include "../../objects/include/highscore.h"
#include "../../states/include/highscore_state.h"
#include "../../states/include/gameover_state.h"


// Draws the highscore input UI
void highscore_ui_draw(const HighscoreState* state);

#endif // HIGHSCORE_UI_H
