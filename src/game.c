#include "game.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

// atualizaaa estado do jogo: passaro,canos,pontuacao e colisao
void atualizar_jogo(EstadoJogo *g) {

    if (!g || g->jogo_acabou) return;

    g->quadro++;

    // gravidade simples
    g->passaro.velocidade += 1;
    g->passaro.y += g->passaro.velocidade;

    // se bate no chão
    if (g->passaro.y >= ALTURA_TELA) {
        g->passaro.y = ALTURA_TELA - 1;
        g->jogo_acabou = 1;
    }

    // se bate no teto
    if (g->passaro.y < 0) {
        g->passaro.y = 0;
        g->jogo_acabou = 1;
    }

    // move os canos para a esquerda
    for (int i = 0; i < g->total_canos; i++) {
        g->canos[i].x--;
    }

    // remove canos que sairam da tela
    int novo_total = 0;
    for (int i = 0; i < g->total_canos; i++) {
        if (g->canos[i].x >= 0) {
            g->canos[novo_total++] = g->canos[i];
        }
    }
    g->total_canos = novo_total;

    // cria novo cano a cada 15 frames
    if (g->quadro % 15 == 0) {
        adicionar_cano(g);
    }

    // verifica colisão com canos
    for (int i = 0; i < g->total_canos; i++) {
        Cano *c = &g->canos[i];

        if (c->x == PASSARO_X) {

            // se não passou pelo espaço
            if (!(g->passaro.y >= c->gap_y &&
                  g->passaro.y <  c->gap_y + c->gap_tam)) {

                g->jogo_acabou = 1;

            } else {
                g->pontuacao++;
            }
        }
    }
}

// faz o passaro subir
void pular(EstadoJogo *g) {
    if (!g || g->jogo_acabou) return;
    g->passaro.velocidade = -3;
}

// adiciona novo cano na extrema direita
void adicionar_cano(EstadoJogo *g) {

    if (g->total_canos >= MAX_CANO) return;

    Cano *c = &g->canos[g->total_canos++];
    c->x = LARGURA_TELA - 1;
    c->gap_tam = 5;
    c->gap_y = rand() % (ALTURA_TELA - c->gap_tam);
}

// desenha estado do jogo no terminal
void desenhar_jogo(const EstadoJogo *g) {

    if (!g) return;

    util_clear_screen();

    for (int y = 0; y < ALTURA_TELA; y++) {
        for (int x = 0; x < LARGURA_TELA; x++) {

            // desenhaa passaroo
            if (x == PASSARO_X && y == g->passaro.y) {
                putchar('>');
                continue;
         }

            int desenhou = 0;

            // desenha canoos
            for (int i = 0; i < g->total_canos; i++) {
                Cano c = g->canos[i];

                if (x == c.x) {
                    if (!(y >= c.gap_y && y < c.gap_y + c.gap_tam)) {
                        putchar('|');
                        desenhou = 1;
                        break;
                }
            }
            }

            if (!desenhou)
                putchar(' ');
        }
        putchar('\n');
    }

    printf("\nPontos: %d\n", g->pontuacao);

    if (g->jogo_acabou)
        printf("FIM DE JOGO!\n");
}
