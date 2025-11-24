#ifndef IO_H
#define IO_H

#include "model.h"

#define SAVE_FILE "data/flappy_save.txt"
#define BEST_FILE "data/flappy_best.txt"
#define RANK_FILE "data/flappy_ranking.txt"

int io_save_state(const GameState *g);
int io_load_state(GameState *g);

int io_save_best_score(int best);
int io_load_best_score(int *best);

void io_add_ranking(const char *name, int score);
void io_show_ranking(void);

#endif
