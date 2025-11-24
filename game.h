#ifndef GAME_H
#define GAME_H

#include "model.h"

// atualiza fisica movimento canos  colisao
void atualizar_jogo(EstadoJogo *g);

// faz o passaro pular
void pular(EstadoJogo *g);

// desenha o jogo
void desenhar_jogo(const EstadoJogo *g);

// adciona um cano
void adicionar_cano(EstadoJogo *g);

#endif
