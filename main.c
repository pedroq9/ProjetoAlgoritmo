#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"
#include "game.h"
#include "io.h"
#include "util.h"

int main_menu(void) {
    int op;
    util_clear_screen();
    printf("=== FLIP BIRD (texto) ===\n");
    printf("1 - Jogar (novo)\n");
    printf("2 - Continuar (carregar)\n");
    printf("3 - Salvar estado\n");
    printf("4 - Ranking\n");
    printf("5 - Sair\n");
    printf("Escolha: ");
    if (scanf("%d", &op) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    int c; while ((c = getchar()) != '\n' && c != EOF);
    return op;
}

int main(void) {
    util_init();
    GameState g;
    char player_name[64] = "Player";

    init_game(&g);

    int running = 1;
    while (running) {
        int op = main_menu();
        if (op == 1) {
            init_game(&g);
            util_clear_screen();
            printf("Digite seu nome (ENTER): ");
            util_kbhit(); /* limpar possivel estado */
            if (fgets(player_name, sizeof(player_name), stdin)) {
                size_t ln = strlen(player_name);
                if (ln && player_name[ln-1] == '\n') player_name[ln-1] = '\0';
            }
            /* brief instruction */
            util_clear_screen();
            printf("Jogo iniciando. Aperte ESPACO + ENTER para subir (modo printf).\n");
            printf("Se quiser sair durante o jogo digite 'q' e ENTER.\n");
            printf("Aperte ENTER para comecar...");
            getchar();

            /* game loop */
            g.game_over = 0;
            while (!g.game_over) {
                draw_game(&g);

                /* ler input nao-bloqueante: aqui usamos check kbhit e getch */
                if (util_kbhit()) {
                    int ch = util_getch();
                    if (ch == ' ' || ch == 'w' || ch == 'W') {
                        game_flap(&g);
                    } else if (ch == 'q' || ch == 'Q') {
                        g.game_over = 1;
                        break;
                    }
                }

                game_update(&g);
                util_sleep_ms(80); /* controla velocidade do jogo */
            }

            draw_game(&g);
            printf("\nFim de jogo. Score: %d\n", g.score);
            /* salvar best e ranking simples */
            int best = 0;
            io_load_best_score(&best);
            if (g.score > best) {
                io_save_best_score(g.score);
                printf("Novo record: %d\n", g.score);
            } else {
                printf("Record atual: %d\n", best);
            }
            printf("Digite seu nome para ranking (ENTER salva, vazio nao salva): ");
            char nick[64] = {0};
            if (fgets(nick, sizeof(nick), stdin)) {
                size_t ln = strlen(nick);
                if (ln && nick[ln-1] == '\n') nick[ln-1] = '\0';
                if (nick[0] != '\0') io_add_ranking(nick, g.score);
            }
            printf("Pressione ENTER para voltar ao menu...");
            getchar();
        }
        else if (op == 2) {
            if (io_load_state(&g)) {
                printf("Estado carregado. Aperte ENTER para continuar...");
                getchar();
                /* resume jogo loop */
                while (!g.game_over) {
                    draw_game(&g);
                    if (util_kbhit()) {
                        int ch = util_getch();
                        if (ch == ' ' || ch == 'w' || ch == 'W') game_flap(&g);
                        else if (ch == 'q' || ch == 'Q') { g.game_over = 1; break; }
                    }
                    game_update(&g);
                    util_sleep_ms(80);
                }
                draw_game(&g);
                printf("\nFim. Score: %d\n", g.score);
                printf("Pressione ENTER para voltar...");
                getchar();
            } else {
                printf("Nenhum save encontrado. Pressione ENTER...");
                getchar();
            }
        }
        else if (op == 3) {
            if (io_save_state(&g)) printf("Estado salvo.\n");
            else printf("Erro ao salvar.\n");
            printf("Pressione ENTER...");
            getchar();
        }
        else if (op == 4) {
            util_clear_screen();
            printf("=== RANKING ===\n");
            io_show_ranking();
            printf("\nPressione ENTER...");
            getchar();
        }
        else {
            running = 0;
        }
    }

    return 0;
}
