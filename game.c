#include "game.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define PIPE_INTERVAL_FRAMES 20  /* quantos frames entre canos */
#define GRAVITY 1                /* incremento de velocidade por frame */
#define FLAP_VELOCITY -3         /* impulso do flap */

/* cria novo cano ao final da fila */
void add_pipe(GameState *g) {
    if (!g) return;
    if (g->pipe_count >= PIPE_MAX) return;

    Pipe *p = &g->pipes[g->pipe_count++];
    p->x = SCREEN_WIDTH - 1;
    p->gap_size = 5;
    p->gap_y = 1 + (rand() % (SCREEN_HEIGHT - p->gap_size - 2));
}

/* jogador faz flap: aplicar impulso negativo */
void game_flap(GameState *g) {
    if (!g) return;
    g->bird.velocity = FLAP_VELOCITY;
}

/* atualiza fisica, move canos, gera canos e checa colisao */
void game_update(GameState *g) {
    if (!g || g->game_over) return;

    g->frame++;

    /* gravidade */
    g->bird.velocity += GRAVITY;
    g->bird.y += g->bird.velocity;

    /* limites vertical */
    if (g->bird.y < 0) g->bird.y = 0;
    if (g->bird.y >= SCREEN_HEIGHT) {
        g->bird.y = SCREEN_HEIGHT - 1;
        g->game_over = 1;
    }

    /* mover canos para esquerda e remover os que sairam */
    for (int i = 0; i < g->pipe_count; ++i) {
        g->pipes[i].x--;
    }

    /* remover canos com x < 0 e incrementar score */
    for (int i = 0; i < g->pipe_count; ++i) {
        if (g->pipes[i].x < 0) {
            /* shift left */
            for (int j = i; j < g->pipe_count - 1; ++j) g->pipes[j] = g->pipes[j+1];
            g->pipe_count--;
            g->score++;
            i--;
        }
    }

    /* gerar novo cano em intervalos */
    if (g->frame % PIPE_INTERVAL_FRAMES == 0) {
        add_pipe(g);
    }

    /* colisao: se existir cano na coluna do passaro, verificar gap */
    for (int i = 0; i < g->pipe_count; ++i) {
        Pipe *p = &g->pipes[i];
        if (p->x == BIRD_X) {
            if (!(g->bird.y >= p->gap_y && g->bird.y <= p->gap_y + p->gap_size)) {
                g->game_over = 1;
            }
        }
    }
}

/* desenha tela (usa util_clear_screen) */
void draw_game(const GameState *g) {
    if (!g) return;
    util_clear_screen();

    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            /* desenha passaro */
            if (x == BIRD_X && y == g->bird.y) {
                putchar('>');
                continue;
            }

            int drew = 0;
            /* desenha canos */
            for (int i = 0; i < g->pipe_count; ++i) {
                const Pipe *p = &g->pipes[i];
                if (p->x == x) {
                    if (y < p->gap_y || y > p->gap_y + p->gap_size) {
                        putchar('|');
                        drew = 1;
                    }
                    break;
                }
            }

            if (!drew) putchar(' ');
        }
        putchar('\n');
    }

    printf("\nSCORE: %d\n", g->score);
    if (g->game_over) printf("GAME OVER\n");
}
