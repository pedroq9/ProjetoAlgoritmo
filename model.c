#include "model.h"
#include <stdlib.h>
#include <string.h>

// inicializaa
void jogo_iniciar(EstadoJogo *g) {
    if (!g) return;

    g->pontuacao = 0;
    g->quadro = 0;
    g->jogo_acabou = 0;
    g->total_canos = 0;

    g->passaro.y = ALTURA_TELA / 2;
    g->passaro.velocidade = 0;
}

// reseta
void passaro_resetar(EstadoJogo *g) {
    if (!g) return;

    g->passaro.y = ALTURA_TELA / 2;
    g->passaro.velocidade = 0;
}
