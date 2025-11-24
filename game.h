#ifndef GAME_H
#define GAME_H

#include "model.h"

/* atualiza um frame do jogo (fisica, canos, colisao, pontuacao) */
void game_update(GameState *g);

/* acao do jogador: flap */
void game_flap(GameState *g);

/* desenha tela no terminal (usa putchar/printf) */
void draw_game(const GameState *g);

/* gera um novo cano */
void add_pipe(GameState *g);

#endif
