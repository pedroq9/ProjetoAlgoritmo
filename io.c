#include "io.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* formato simples de save em texto */
int io_save_state(const GameState *g) {
    if (!g) return 0;
    FILE *f = fopen(SAVE_FILE, "w");
    if (!f) return 0;
    fprintf(f, "%d %d %d %d %d\n",
            g->bird.y, g->bird.velocity,
            g->pipe_count, g->score, g->frame);
    for (int i = 0; i < g->pipe_count; ++i) {
        const Pipe *p = &g->pipes[i];
        fprintf(f, "%d %d %d\n", p->x, p->gap_y, p->gap_size);
    }
    fprintf(f, "%d\n", g->game_over ? 1 : 0);
    fclose(f);
    return 1;
}

int io_load_state(GameState *g) {
    if (!g) return 0;
    FILE *f = fopen(SAVE_FILE, "r");
    if (!f) return 0;
    int by, bv, pc, sc, fr;
    if (fscanf(f, "%d %d %d %d %d", &by, &bv, &pc, &sc, &fr) != 5) { fclose(f); return 0; }
    g->bird.y = by;
    g->bird.velocity = bv;
    g->pipe_count = (pc < 0) ? 0 : (pc > PIPE_MAX ? PIPE_MAX : pc);
    g->score = sc;
    g->frame = fr;
    for (int i = 0; i < g->pipe_count; ++i) {
        int x, gy, gs;
        if (fscanf(f, "%d %d %d", &x, &gy, &gs) != 3) { fclose(f); return 0; }
        g->pipes[i].x = x;
        g->pipes[i].gap_y = gy;
        g->pipes[i].gap_size = gs;
    }
    int go;
    if (fscanf(f, "%d", &go) == 1) g->game_over = go ? 1 : 0;
    else g->game_over = 0;
    fclose(f);
    return 1;
}

int io_save_best_score(int best) {
    FILE *f = fopen(BEST_FILE, "w");
    if (!f) return 0;
    fprintf(f, "%d\n", best);
    fclose(f);
    return 1;
}

int io_load_best_score(int *best) {
    if (!best) return 0;
    FILE *f = fopen(BEST_FILE, "r");
    if (!f) { *best = 0; return 0; }
    if (fscanf(f, "%d", best) != 1) { *best = 0; fclose(f); return 0; }
    fclose(f);
    return 1;
}

void io_add_ranking(const char *name, int score) {
    FILE *f = fopen(RANK_FILE, "a");
    if (!f) return;
    char ts[64];
    util_timestamp(ts, sizeof(ts));
    fprintf(f, "%s %d %s\n", name, score, ts);
    fclose(f);
}

void io_show_ranking(void) {
    FILE *f = fopen(RANK_FILE, "r");
    if (!f) { printf("Nenhum ranking encontrado.\n"); return; }
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        printf("%s", line);
    }
    fclose(f);
}
