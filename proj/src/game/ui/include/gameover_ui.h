#ifndef GAMEOVER_UI_H
#define GAMEOVER_UI_H

#include "font.h"
#include "../../states/include/gameover_state.h"
#include "menu_ui.h"

void gameover_ui_draw(const GameOverState* state);
void gameover_ui_draw_background(void);
void gameover_ui_draw_title(void);
void gameover_ui_draw_score(int score, int lines);
void gameover_ui_draw_options(const GameOverState* state);
void draw_menu_option(int x, int y, const char* text, bool selected);
void draw_highscore_input(const GameOverState* state);
//void highscore_ui_draw(const GameOverState* state);

#endif
