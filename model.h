#ifndef MODELO_H
#define MODELO_H

#define MAX_CANO 16
#define ALTURA_TELA 20
#define LARGURA_TELA 40
#define PASSARO_X 5

// representa o passáro 
typedef struct {
    int y;           
    int velocidade;  
} Passaro;

// representa o cano
typedef struct {
    int x;         
    int gap_y;     
    int gap_tam;   
} Cano;

typedef struct {
    Passaro passaro;
    Cano canos[MAX_CANO];
    int total_canos;
    int pontuacao;
    int quadro;
    int jogo_acabou;
} EstadoJogo;

   // inicializa e/ ou encerra o estado de jogo
void jogo_iniciar(EstadoJogo *g);
void passaro_resetar(EstadoJogo *g);

#endif
