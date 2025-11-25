#include "io.h"
#include <stdio.h>
#include <stdlib.h>

int salvar_jogo(const EstadoJogo *g)
{
    FILE *f = fopen(ARQ_SAVE, "w");
    if (!f) {
        perror("Erro ao abrir arquivo de salvamento");
        return 0;
    }

    // passaro
    fprintf(f, "%d %d\n", g->passaro.y, g->passaro.velocidade);

    // canos
    fprintf(f, "%d\n", g->total_canos);
    for (int i = 0; i < g->total_canos; i++)
    {
        fprintf(f, "%d %d %d\n",
            g->canos[i].x,
            g->canos[i].gap_y,
            g->canos[i].gap_tam
        );
    }

    // pontuacao e estado geral
    fprintf(f, "%d %d %d\n",
        g->pontuacao,
        g->quadro,
        g->jogo_acabou
    );

    fclose(f);
    return 1;
}

int carregar_jogo(EstadoJogo *g)
{
    FILE *f = fopen(ARQ_SAVE, "r");
    if (!f) {
        perror("Erro ao abrir arquivo de carregamento");
        return 0;
    }

    fscanf(f, "%d %d",
        &g->passaro.y,
        &g->passaro.velocidade
    );

    // canos
    fscanf(f, "%d", &g->total_canos);

    for (int i = 0; i < g->total_canos; i++)
    {
        fscanf(f, "%d %d %d",
            &g->canos[i].x,
            &g->canos[i].gap_y,
            &g->canos[i].gap_tam
        );
    }

    // pontuacoa e estado
    fscanf(f, "%d %d %d",
        &g->pontuacao,
        &g->quadro,
        &g->jogo_acabou
    );

    fclose(f);
    return 1;
}

int salvar_melhor_pontuacao(int melhor)
{
    FILE *f = fopen(ARQ_MELHOR, "w");
    if (!f) {
        perror("Erro ao abrir arquivo de melhor pontuação");
        return 0;
    }
    fprintf(f, "%d", melhor);
    fclose(f);
    return 1;
}

int carregar_melhor_pontuacao(int *melhor)
{
    FILE *f = fopen(ARQ_MELHOR, "r");
    if (!f) {
        perror("Erro ao abrir arquivo de melhor pontuação");
        return 0;
    }
    fscanf(f, "%d", melhor);
    fclose(f);
    return 1;
}

void adicionar_ranking(const char *nome, int pontuacao)
{
    FILE *f = fopen(ARQ_RANKING, "a");
    if (!f) {
        perror("Erro ao abrir arquivo de ranking");
        return;
    }

    fprintf(f, "%s %d\n", nome, pontuacao);

    fclose(f);
}

void mostrar_ranking(void)
{
    FILE *f = fopen(ARQ_RANKING, "r");
    if (!f)
    {
        printf("Nenhum ranking salvo ainda.\n");
        return;
    }

    char nome[64];
    int score;
    int pos = 1;

    while (fscanf(f, "%s %d", nome, &score) == 2)
    {
        printf("%d  %-10s  %d pontos\n", pos++, nome, score);
    }

    fclose(f);
}
