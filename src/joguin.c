#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#include <windows.h>
#include <direct.h>
#include <sys/stat.h>
#define MKDIR(path) _mkdir(path)
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

// ====================== MODELO.H ======================

#define MAX_CANO 16
#define ALTURA_TELA 20
#define LARGURA_TELA 40
#define PASSARO_X 5

typedef struct {
    int y;
    int velocidade;
} Passaro;

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

// ====================== GAME.C ======================

void jogo_iniciar(EstadoJogo *g) {
    if (!g) return;
    g->pontuacao = 0;
    g->quadro = 0;
    g->jogo_acabou = 0;
    g->total_canos = 0;
    g->passaro.y = ALTURA_TELA / 2;
    g->passaro.velocidade = 0;
}

void passaro_resetar(EstadoJogo *g) {
    if (!g) return;
    g->passaro.y = ALTURA_TELA / 2;
    g->passaro.velocidade = 0;
}

// ====================== UTIL.C ======================

void util_init(void) {
    srand((unsigned)time(NULL));
}

void util_sleep_ms(int ms) {
#if defined(_WIN32) || defined(_WIN64)
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void util_clear_screen(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

int util_kbhit(void) {
#if defined(_WIN32) || defined(_WIN64)
    return _kbhit();
#else
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
#endif
}

int util_getch(void) {
#if defined(_WIN32) || defined(_WIN64)
    return _getch();
#else
    return getchar();
#endif
}

char *util_timestamp(char *buf, size_t n) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    snprintf(buf, n, "%02d/%02d/%04d %02d:%02d:%02d",
             tm_info->tm_mday,
             tm_info->tm_mon + 1,
             tm_info->tm_year + 1900,
             tm_info->tm_hour,
             tm_info->tm_min,
             tm_info->tm_sec);
    return buf;
}

int util_garantir_pasta_data(void) {
    struct stat st;
    if (stat("data", &st) == 0) {
#ifndef _WIN32
        if (S_ISDIR(st.st_mode)) return 1;
#else
        if ((st.st_mode & _S_IFDIR) != 0) return 1;
#endif
        return 0;
    }
    if (MKDIR("data") == 0) return 1;
    if (errno == EEXIST) return 1;
    return 0;
}

// ====================== IO.C ======================

#define ARQ_SAVE    "data/salvamento.txt"
#define ARQ_MELHOR  "data/melhor_pontuacao.txt"
#define ARQ_RANKING "data/ranking.txt"

int salvar_jogo(const EstadoJogo *g) {
    FILE *f = fopen(ARQ_SAVE, "w");
    if (!f) { perror("Erro ao abrir arquivo de salvamento"); return 0; }
    fprintf(f, "%d %d\n", g->passaro.y, g->passaro.velocidade);
    fprintf(f, "%d\n", g->total_canos);
    for (int i = 0; i < g->total_canos; i++)
        fprintf(f, "%d %d %d\n", g->canos[i].x, g->canos[i].gap_y, g->canos[i].gap_tam);
    fprintf(f, "%d %d %d\n", g->pontuacao, g->quadro, g->jogo_acabou);
    fclose(f);
    return 1;
}

int carregar_jogo(EstadoJogo *g) {
    FILE *f = fopen(ARQ_SAVE, "r");
    if (!f) { perror("Erro ao abrir arquivo de carregamento"); return 0; }
    fscanf(f, "%d %d", &g->passaro.y, &g->passaro.velocidade);
    fscanf(f, "%d", &g->total_canos);
    for (int i = 0; i < g->total_canos; i++)
        fscanf(f, "%d %d %d", &g->canos[i].x, &g->canos[i].gap_y, &g->canos[i].gap_tam);
    fscanf(f, "%d %d %d", &g->pontuacao, &g->quadro, &g->jogo_acabou);
    fclose(f);
    return 1;
}

int salvar_melhor_pontuacao(int melhor) {
    FILE *f = fopen(ARQ_MELHOR, "w");
    if (!f) { perror("Erro ao abrir arquivo de melhor pontuação"); return 0; }
    fprintf(f, "%d", melhor);
    fclose(f);
    return 1;
}

int carregar_melhor_pontuacao(int *melhor) {
    FILE *f = fopen(ARQ_MELHOR, "r");
    if (!f) { perror("Erro ao abrir arquivo de melhor pontuação"); return 0; }
    fscanf(f, "%d", melhor);
    fclose(f);
    return 1;
}

void adicionar_ranking(const char *nome, int pontuacao) {
    FILE *f = fopen(ARQ_RANKING, "a");
    if (!f) { perror("Erro ao abrir arquivo de ranking"); return; }
    fprintf(f, "%s %d\n", nome, pontuacao);
    fclose(f);
}

void mostrar_ranking(void) {
    FILE *f = fopen(ARQ_RANKING, "r");
    if (!f) { printf("Nenhum ranking salvo ainda.\n"); return; }
    char nome[64]; int score; int pos = 1;
    while (fscanf(f, "%s %d", nome, &score) == 2)
        printf("%d°  %-10s  %d pontos\n", pos++, nome, score);
    fclose(f);
}

// ====================== GAME LOGIC ======================

void atualizar_jogo(EstadoJogo *g) {
    if (!g || g->jogo_acabou) return;
    g->quadro++;
    g->passaro.velocidade += 1;
    g->passaro.y += g->passaro.velocidade;
    if (g->passaro.y >= ALTURA_TELA) { g->passaro.y = ALTURA_TELA - 1; g->jogo_acabou = 1; }
    if (g->passaro.y < 0) { g->passaro.y = 0; g->jogo_acabou = 1; }
    for (int i = 0; i < g->total_canos; i++) g->canos[i].x--;
    int novo_total = 0;
    for (int i = 0; i < g->total_canos; i++) if (g->canos[i].x >= 0) g->canos[novo_total++] = g->canos[i];
    g->total_canos = novo_total;
    if (g->quadro % 15 == 0) {
        if (g->total_canos < MAX_CANO) {
            Cano *c = &g->canos[g->total_canos++];
            c->x = LARGURA_TELA - 1;
            c->gap_tam = 5;
            c->gap_y = rand() % (ALTURA_TELA - c->gap_tam);
        }
    }
    for (int i = 0; i < g->total_canos; i++) {
        Cano *c = &g->canos[i];
        if (c->x == PASSARO_X) {
            if (!(g->passaro.y >= c->gap_y && g->passaro.y < c->gap_y + c->gap_tam)) g->jogo_acabou = 1;
            else g->pontuacao++;
        }
    }
}

void pular(EstadoJogo *g) { if (!g || g->jogo_acabou) return; g->passaro.velocidade = -3; }

void desenhar_jogo(const EstadoJogo *g) {
    if (!g) return;
    util_clear_screen();
    for (int y = 0; y < ALTURA_TELA; y++) {
        for (int x = 0; x < LARGURA_TELA; x++) {
            if (x == PASSARO_X && y == g->passaro.y) { putchar('>'); continue; }
            int desenhou = 0;
            for (int i = 0; i < g->total_canos; i++) {
                Cano c = g->canos[i];
                if (x == c.x && !(y >= c.gap_y && y < c.gap_y + c.gap_tam)) { putchar('|'); desenhou = 1; break; }
            }
            if (!desenhou) putchar(' ');
        }
        putchar('\n');
    }
    printf("\nPontos: %d\n", g->pontuacao);
    if (g->jogo_acabou) printf("FIM DE JOGO!\n");
}

// ====================== MAIN.C ======================

int menu_principal(void) {
    util_clear_screen();
    printf("FLAPPY \n");
    printf("1 - Novo jogo\n");
    printf("2 - Continuar (carregar)\n");
    printf("3 - Salvar jogo\n");
    printf("4 - Ranking\n");
    printf("5 - Sair\n");
    printf("Escolha: ");
    int op;
    if (scanf("%d", &op) != 1) { int c; while ((c = getchar()) != '\n' && c != EOF); return 0; }
    int c; while ((c = getchar()) != '\n' && c != EOF);
    return op;
}

int main(void) {
    util_init();
    if (!util_garantir_pasta_data()) fprintf(stderr, "Aviso: nao foi possível criar/abrir a pasta 'data'. Salvamento pode falhar.\n");

    EstadoJogo jogo;
    char nome_jogador[64] = "Jogador";
    jogo_iniciar(&jogo);
    int executando = 1;

    while (executando) {
        int opcao = menu_principal();

        if (opcao == 1) {
            jogo_iniciar(&jogo);
            util_clear_screen();
            printf("Digite seu nome: ");
            if (fgets(nome_jogador, sizeof(nome_jogador), stdin)) {
                size_t ln = strlen(nome_jogador);
                if (ln && nome_jogador[ln - 1] == '\n') nome_jogador[ln - 1] = '\0';
            }
            util_clear_screen();
            printf("Iniciando...\nUse ESPACO ou W para pular.\nPressione ENTER para comecar...");
            getchar();
            jogo.jogo_acabou = 0;
            while (!jogo.jogo_acabou) {
                desenhar_jogo(&jogo);
                if (util_kbhit()) {
                    int tecla = util_getch();
                    if (tecla == ' ' || tecla == 'w' || tecla == 'W') pular(&jogo);
                    else if (tecla == 'q' || tecla == 'Q') { jogo.jogo_acabou = 1; break; }
                }
                atualizar_jogo(&jogo);
                util_sleep_ms(80);
            }
            desenhar_jogo(&jogo);
            printf("\nFim do jogo — Pontos: %d\n", jogo.pontuacao);

            int recorde = 0;
            carregar_melhor_pontuacao(&recorde);
            if (jogo.pontuacao > recorde) { salvar_melhor_pontuacao(jogo.pontuacao); printf("Novo recorde!\n"); }
            else printf("Recorde atual: %d\n", recorde);

            printf("\nDigite seu nome para ranking (vazio = nao salvar): ");
            char nick[64] = "";
            if (fgets(nick, sizeof(nick), stdin)) {
                size_t ln = strlen(nick);
                if (ln && nick[ln - 1] == '\n') nick[ln - 1] = '\0';
                if (nick[0] != '\0') adicionar_ranking(nick, jogo.pontuacao);
            }
            printf("Pressione ENTER para voltar...");
            getchar();
        }
        else if (opcao == 2) {
            if (carregar_jogo(&jogo)) {
                printf("Jogo carregado! Pressione ENTER...");
                getchar();
                while (!jogo.jogo_acabou) {
                    desenhar_jogo(&jogo);
                    if (util_kbhit()) {
                        int tecla = util_getch();
                        if (tecla == ' ' || tecla == 'w' || tecla == 'W') pular(&jogo);
                        else if (tecla == 'q' || tecla == 'Q') { jogo.jogo_acabou = 1; break; }
                    }
                    atualizar_jogo(&jogo);
                    util_sleep_ms(80);
                }
                desenhar_jogo(&jogo);
                printf("\nFim — Pontos: %d\n", jogo.pontuacao);
                printf("Pressione ENTER para voltar...");
                getchar();
            } else { printf("Nenhum save encontrado. ENTER..."); getchar(); }
        }
        else if (opcao == 3) {
            if (salvar_jogo(&jogo)) printf("Jogo salvo!\n"); else printf("Erro ao salvar!\n");
            printf("ENTER..."); getchar();
        }
        else if (opcao == 4) {
            util_clear_screen();
            printf("RANKING\n");
            mostrar_ranking();
            printf("\nENTER..."); getchar();
        }
        else executando = 0;
    }

    return 0;
}
