#ifndef GAME_H
#define GAME_H

#include "model.h"

// atualizaaa fisica movimento canos  colisao
void atualizar_jogo(EstadoJogo *g);

// faz o passaro pular
void pular(EstadoJogo *g);

// desenhaa o jogo
void desenhar_jogo(const EstadoJogo *g);

// adcionaa um cano
void adicionar_cano(EstadoJogo *g);

#endif
