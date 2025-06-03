#include <stdio.h>
#include <string.h>
#include "include/highscore.h"

void load_high_scores(HighScore scores[], int* count) {
    FILE* file = fopen(HIGHSCORE_FILE, "rb");
    if (!file) {
        *count = 0;
        return;
    }

    *count = fread(scores, sizeof(HighScore), MAX_HIGH_SCORES, file);
    fclose(file);
}

void save_high_scores(HighScore scores[], const int count) {
    FILE* file = fopen(HIGHSCORE_FILE, "wb");
    if (!file) return;

    fwrite(scores, sizeof(HighScore), count, file);
    fclose(file);
}

void try_add_high_score(const char* name, const unsigned int new_score) {
    HighScore scores[MAX_HIGH_SCORES + 1];
    int count;
    load_high_scores(scores, &count);

    HighScore new_entry;
    strncpy(new_entry.name, name, MAX_NAME_LENGTH - 1);
    new_entry.name[MAX_NAME_LENGTH - 1] = '\0';
    new_entry.score = new_score;

    scores[count++] = new_entry;

    // Sort scores 
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[j].score > scores[i].score) {
                HighScore tmp = scores[i];
                scores[i] = scores[j];
                scores[j] = tmp;
            }
        }
    }

    // Clamp to MAX
    if (count > MAX_HIGH_SCORES) count = MAX_HIGH_SCORES;

    save_high_scores(scores, count);
}
