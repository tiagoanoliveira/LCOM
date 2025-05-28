//
// Created by tiago-oliveira on 27-05-2025.
//

#include "include/score.h"

#define LINES_PER_LEVEL 10

void scoreInit(GameScore* score) {
    score->score = 0;
    score->lines = 0;
    score->level = 1;
}

void scoreAddLines(GameScore* score, int linesCleared) {
    static const uint32_t lineScores[] = { 0, 40, 100, 300, 1200 };
    if (linesCleared < 1 || linesCleared > 4) return;

    score->lines += linesCleared;
    score->score += lineScores[linesCleared] * score->level;

    // Level up every 10 lines
    score->level = score->lines / LINES_PER_LEVEL + 1;
}

uint32_t scoreGetDropSpeed(const GameScore* score) {
    // Drop speed increases with level
    // Higher level = lower delay (faster drop)
    uint32_t baseSpeed = 1000; // in ms
    uint32_t speed = baseSpeed > score->level * 75 ? baseSpeed - score->level * 75 : 100;
    return speed;
}
