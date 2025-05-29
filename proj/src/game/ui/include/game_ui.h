#ifndef GAME_UI_H
#define GAME_UI_H

#include "../../objects/include/score.h"
#include "../../objects/include/highscore.h"
#include "../../core/include/tetris.h"
#include "font.h"

// Draws the Tetris grid
void draw_grid();

// Draws the backgroud of Tetris grid
void draw_grid_background();

// Draws the score, lines, and level information
void draw_score_info();

// Draws the high scores
void draw_highscores(void);

#endif //GAME_UI_H
