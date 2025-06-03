#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#define MAX_NAME_LENGTH 6
#define MAX_HIGH_SCORES 5
#define HIGHSCORE_FILE "/home/lcom/labs/proj/src/game/objects/include/highscores.dat"

typedef struct {
    char name[MAX_NAME_LENGTH];
    unsigned int score;
} HighScore;

void load_high_scores(HighScore scores[], int* count);
void save_high_scores(HighScore scores[], int count);
void try_add_high_score(const char* name, unsigned int score);

#endif
