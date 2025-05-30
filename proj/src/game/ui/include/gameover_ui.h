#ifndef GAMEOVER_UI_H
#define GAMEOVER_UI_H

#include "font.h"
#include "../../states/include/gameover_state.h"
#include "../../states/include/highscore_state.h"
#include "menu_ui.h"

void gameover_ui_draw(const GameOverState* state);
void gameover_ui_draw_background(void);
void gameover_ui_draw_title(void);
void gameover_ui_draw_score(int score, int lines);
void gameover_ui_draw_options(const GameOverState* state);
void draw_menu_option(int x, int y, const char* text, bool selected);
//void highscore_ui_draw(const HighscoreState* state);

#endif
