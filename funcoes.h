#ifndef FUNCOES_H
#define FUNCOES_H

#include "dados.h"
#include "menu.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_JOGADORES 4
#define TABULEIRO_LINHAS 5
#define TABULEIRO_COLUNAS 5
#define TABULEIRO_CASAS (TABULEIRO_LINHAS * TABULEIRO_COLUNAS)
#define TOTAL_CARTAS_UNIDADE1 12


void exibir_tabuleiro(tp_player jogadores[MAX_JOGADORES], int num_jogadores) {
    printf("================================\n");
    printf("            TABULEIRO           \n");
    printf("================================\n");

    for (int linha = 0; linha < TABULEIRO_LINHAS; linha++) {
        for (int coluna = 0; coluna < TABULEIRO_COLUNAS; coluna++) {
            int casa = (linha * TABULEIRO_COLUNAS) + coluna + 1;
            int qtd_na_casa = 0;
            int ultimo_jogador = -1;
            char celula[8];

            for (int i = 0; i < num_jogadores; i++) {
                if (jogadores[i].posicao == casa) {
                    qtd_na_casa++;
                    ultimo_jogador = i;
                }
            }

            if (qtd_na_casa == 0) {
                snprintf(celula, sizeof(celula), "%02d", casa);
            } else if (qtd_na_casa == 1) {
                celula[0] = 'J';
                celula[1] = (char)('0' + (ultimo_jogador + 1));
                celula[2] = '\0';
            } else {
                celula[0] = 'M';
                celula[1] = (char)('0' + qtd_na_casa);
                celula[2] = '\0';
            }

            printf("| %-3s ", celula);
        }
        printf("|\n");
    }
}


static void exibir_status(tp_player jogadores[MAX_JOGADORES], tp_pilha historico[MAX_JOGADORES], int num_jogadores) {
    printf("\n================================\n");
    printf("            STATUS              \n");
    printf("================================\n");

    for (int i = 0; i < num_jogadores; i++) {
        int acertos = altura_pilha(&historico[i]) - 1;
        if (acertos < 0) {
            acertos = 0;
        }

        printf("J%d - %-12s | Casa: %2d | Acertos: %d\n",
               i + 1,
               jogadores[i].personagem,
               jogadores[i].posicao,
               acertos);
    }
}

static void mostrar_carta(const tp_carta *carta) {
    printf("\nPergunta #%d (dificuldade %d)\n", carta->id_carta, carta->dificuldade);
    printf("%s", carta->pergunta);
    printf("%s", carta->alternativas);
}

/*
 * Retornos:
 * 0 -> voltar para menu principal
 * 1 -> encerrar programa
 */
static int executar_partida(void) {
    int num_jogadores;
    int personagens_usados[MAX_JOGADORES] = {0, 0, 0, 0};
    tp_player jogadores[MAX_JOGADORES];
    tp_pilha historico[MAX_JOGADORES];
    tp_fila fila_turnos;
    int indice_carta = 0;

    // Referencia os vetores ainda nao preenchidos para evitar warning de variavel nao usada.
    (void)unidade2;
    (void)unidade3;

    limpar_terminal();
    num_jogadores = menu_num_jogadores();

    for (int i = 0; i < num_jogadores; i++) {
        int personagem;

        while (1) {
            personagem = menu_personagem(personagens_usados, i);

            if (personagem == 0) {
                if (i == 0) {
                    printf("Nao existe jogador anterior para voltar.\n");
                    continue;
                }

                i--;
                personagens_usados[jogadores[i].id_player - 1] = 0;
                printf("Selecao do Jogador %d removida.\n", i + 1);
                continue;
            }

            jogadores[i] = player[personagem - 1];
            jogadores[i].id_player = personagem;
            jogadores[i].posicao = 1;
            personagens_usados[personagem - 1] = 1;
            inicializa_pilha(&historico[i]);
            push(&historico[i], 1);
            break;
        }
    }

    inicializa_fila(&fila_turnos);
    for (int i = 0; i < num_jogadores; i++) {
        insere_fila(&fila_turnos, i);
    }

    while (1) {
        int id_jogador;
        tp_carta carta;
        int resposta;
        bool turno_concluido = false;

        limpar_terminal();
        exibir_tabuleiro(jogadores, num_jogadores);
        exibir_status(jogadores, historico, num_jogadores);

        remove_fila(&fila_turnos, &id_jogador);
        carta = unidade1[indice_carta];
        indice_carta = (indice_carta + 1) % TOTAL_CARTAS_UNIDADE1;

        printf("\n================================\n");
        printf("Turno do Jogador %d (%s)\n", id_jogador + 1, jogadores[id_jogador].personagem);
        printf("================================\n");

        while (!turno_concluido) {
            int pause;

            mostrar_carta(&carta);
            resposta = ler_inteiro_intervalo("Resposta (1-5) ou 0 para pausar: ", 0, 5);

            if (resposta == 0) {
                pause = menu_pause();

                if (pause == 1) {
                    continue;
                }

                if (pause == 2) {
                    return 0;
                }

                return 1;
            }

            turno_concluido = true;
        }

        if (resposta == carta.resposta) {
            int nova_posicao = jogadores[id_jogador].posicao + carta.avanco;

            if (nova_posicao > TABULEIRO_CASAS) {
                nova_posicao = TABULEIRO_CASAS;
            }

            jogadores[id_jogador].posicao = nova_posicao;
            push(&historico[id_jogador], nova_posicao);

            printf("\nAcertou! Voce avancou %d casa(s).\n", carta.avanco);
        } else {
            printf("\nResposta incorreta. Nenhum avancou nesta rodada.\n");
        }

        if (jogadores[id_jogador].posicao >= TABULEIRO_CASAS) {
            printf("\n================================\n");
            printf("VENCEDOR: Jogador %d (%s)!\n", id_jogador + 1, jogadores[id_jogador].personagem);
            printf("================================\n");
            aguardar_enter();
            return 0;
        }

        insere_fila(&fila_turnos, id_jogador);
        aguardar_enter();
    }
}

#endif