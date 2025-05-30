#ifndef HIGHSCORE_UI_H
#define HIGHSCORE_UI_H

#include "font.h"
#include "menu_ui.h"
#include "../../objects/include/highscore.h"
#include "../../states/include/highscore_state.h"
#include "../../states/include/gameover_state.h"
#include "../../core/include/config.h"
#include <string.h>
#include <stdio.h>



// Draws the highscore input UI
void highscore_ui_draw(const HighscoreState* state);

// Helper functions for drawing various UI components
void draw_highscore_title(int box_x, int box_y);
void draw_highscore_box(int x, int y, int w, int h);
void draw_highscore_score(int box_x, int box_y, unsigned int score);
void draw_highscore_name_prompt(int box_x, int box_y, int score_y);
void draw_highscore_name_input(int box_x, int box_y, int prompt_y, const char* name_input);
void draw_highscore_continue_prompt(int box_x, int box_y);

#endif // HIGHSCORE_UI_H
