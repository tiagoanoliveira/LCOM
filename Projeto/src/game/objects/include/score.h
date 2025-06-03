#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

typedef struct {
    unsigned int score;
    unsigned int lines_cleared;
    unsigned int level;
} GameScore;

void scoreInit(GameScore* score);
void scoreAddLines(GameScore* score, int linesCleared);
uint32_t scoreGetDropSpeed(const GameScore* score);

#endif //SCORE_H
