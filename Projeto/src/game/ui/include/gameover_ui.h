#ifndef GAMEOVER_UI_H
#define GAMEOVER_UI_H

#include "../../states/include/gameover_state.h"
#include "../../states/include/highscore_state.h"

void gameover_ui_draw(const GameOverState* state);
void gameover_ui_draw_title(void);
void gameover_ui_draw_score(int score, int lines);
void gameover_ui_draw_options(const GameOverState* state);
void draw_menu_option(int x, int y, const char* text, bool selected);
void gameover_ui_draw_background(void);

#endif
