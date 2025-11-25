#ifndef IO_H
#define IO_H

#include "model.h"

#define ARQ_SAVE    "data/salvamento.txt"
#define ARQ_MELHOR  "data/melhor_pontuacao.txt"
#define ARQ_RANKING "data/ranking.txt"


// salva o jogo
int salvar_jogo(const EstadoJogo *g);

// carrega o estado de jogo
int carregar_jogo(EstadoJogo *g);

// salva M pontuacao
int salvar_melhor_pontuacao(int melhor);

// carrega M pontuaca
int carregar_melhor_pontuacao(int *melhor);

// adciona entrad rankin
void adicionar_ranking(const char *nome, int pontuacao);

// mostra o rankinf
void mostrar_ranking(void);

#endif
