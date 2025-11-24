#ifndef MODEL_H
#define MODEL_H

#define PIPE_MAX 16
#define SCREEN_HEIGHT 20
#define SCREEN_WIDTH 40
#define BIRD_X 5

typedef struct {
    int y;          /* altura do passaro (0..SCREEN_HEIGHT-1) */
    int velocity;   /* velocidade vertical (inteiro simples) */
} Bird;

typedef struct {
    int x;          /* posicao horizontal */
    int gap_y;      /* posicao vertical do gap (top) */
    int gap_size;   /* tamanho do gap (altura) */
} Pipe;

typedef struct {
    Bird bird;
    Pipe pipes[PIPE_MAX];
    int pipe_count;
    int score;
    int frame;
    int game_over;
} GameState;

/* inicializa ou reseta o estado do jogo */
void init_game(GameState *g);
void reset_bird(GameState *g);

#endif
