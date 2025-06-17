#ifndef PAUSE_UI_H
#define PAUSE_UI_H

#include "../../core/include/game.h"

void pause_ui_draw(const GameLogic* game);
void pause_ui_draw_overlay(void);
void pause_ui_draw_menu(const GameLogic* game);

#endif //PAUSE_UI_H
