#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "model.h"
#include "game.h"
#include "io.h"
#include "util.h"

// Menu principal
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
    if (scanf("%d", &op) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    return op;
}

int main(void) {

    util_init();

    if (!util_garantir_pasta_data()) {
        fprintf(stderr, "Aviso: nao foi possível criar/abrir a pasta 'data'. Salvamento pode falhar.\n");
    }

    EstadoJogo jogo;
    char nome_jogador[64] = "Jogador";

    jogo_iniciar(&jogo);

    int executando = 1;

    while (executando) {

        int opcao = menu_principal();

        // novo jogo
        if (opcao == 1) {

            jogo_iniciar(&jogo);
            util_clear_screen();

            printf("Digite seu nome: ");
            if (fgets(nome_jogador, sizeof(nome_jogador), stdin)) {
                size_t ln = strlen(nome_jogador);
                if (ln && nome_jogador[ln - 1] == '\n')
                    nome_jogador[ln - 1] = '\0';
            }

            util_clear_screen();
            printf("Iniciando...\n");
            printf("Use ESPACO ou W para pular.\n");
            printf("Pressione ENTER para comecar...");
            getchar();

            jogo.jogo_acabou = 0;

            // loop principal
            while (!jogo.jogo_acabou) {

                desenhar_jogo(&jogo);

                // entrada
                if (util_kbhit()) {

                    int tecla = util_getch();

                    if (tecla == ' ' || tecla == 'w' || tecla == 'W')
                        pular(&jogo);

                    else if (tecla == 'q' || tecla == 'Q') {
                        jogo.jogo_acabou = 1;
                        break;
                    }
                }

                atualizar_jogo(&jogo);
                util_sleep_ms(80);
            }

            desenhar_jogo(&jogo);
            printf("\nFim do jogo — Pontos: %d\n", jogo.pontuacao);

            // recorde
            int recorde = 0;
            carregar_melhor_pontuacao(&recorde);

            if (jogo.pontuacao > recorde) {
                salvar_melhor_pontuacao(jogo.pontuacao);
                printf("Novo recorde!\n");
            } else {
                printf("Recorde atual: %d\n", recorde);
            }

            // ranking
            printf("\nDigite seu nome para ranking (vazio = nao salvar): ");
            char nick[64] = "";

            if (fgets(nick, sizeof(nick), stdin)) {
                size_t ln = strlen(nick);
                if (ln && nick[ln - 1] == '\n')
                    nick[ln - 1] = '\0';

                if (nick[0] != '\0')
                    adicionar_ranking(nick, jogo.pontuacao);
            }

            printf("Pressione ENTER para voltar...");
            getchar();
        }

            //carrega
        else if (opcao == 2) {

            if (carregar_jogo(&jogo)) {

                printf("Jogo carregado! Pressione ENTER...");
                getchar();

                while (!jogo.jogo_acabou) {

                    desenhar_jogo(&jogo);

                    if (util_kbhit()) {
                        int tecla = util_getch();

                        if (tecla == ' ' || tecla == 'w' || tecla == 'W')
                            pular(&jogo);

                        else if (tecla == 'q' || tecla == 'Q') {
                            jogo.jogo_acabou = 1;
                            break;
                        }
                    }

                    atualizar_jogo(&jogo);
                    util_sleep_ms(80);
                }

                desenhar_jogo(&jogo);
                printf("\nFim — Pontos: %d\n", jogo.pontuacao);
                printf("Pressione ENTER para voltar...");
                getchar();

            } else {
                printf("Nenhum save encontrado. ENTER...");
                getchar();
            }
        }

        //salva
        else if (opcao == 3) {

            if (salvar_jogo(&jogo))
                printf("Jogo salvo!\n");
            else
                printf("Erro ao salvar!\n");

            printf("ENTER...");
            getchar();
        }

        //ranking 
        else if (opcao == 4) {

            util_clear_screen();
            printf("RANKING\n");
            mostrar_ranking();
            printf("\nENTER...");
            getchar();
        }

        //sair
        else {
            executando = 0;
        }
    }

    return 0;
}
