#include "model.h"
#include <stdlib.h>
#include <string.h>

void init_game(GameState *g) {
    if (!g) return;
    g->score = 0;
    g->frame = 0;
    g->game_over = 0;
    g->pipe_count = 0;
    g->bird.y = SCREEN_HEIGHT / 2;
    g->bird.velocity = 0;
}

void reset_bird(GameState *g) {
    if (!g) return;
    g->bird.y = SCREEN_HEIGHT / 2;
    g->bird.velocity = 0;
}
