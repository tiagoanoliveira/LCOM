#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>

typedef struct {
    uint32_t score;
    uint32_t lines;
    uint32_t level;
} GameScore;

void scoreInit(GameScore* score);
void scoreAddLines(GameScore* score, int linesCleared);
uint32_t scoreGetDropSpeed(const GameScore* score);

#endif //SCORE_H
