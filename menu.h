#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <string.h>

#include "fila.h"
#include "dados.h"

void limpar_buffer_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* limpa o que sobrou na entrada */
    }
}

int menu_principal() {
    int opcao;

    printf("\n================================\n");
    printf("         QUACK QUIZ             \n");
    printf("================================\n");
    printf("1. Novo jogo\n");
    printf("2. Sair\n");
    printf("================================\n");
    printf("Digite alguma opcao: ");
    if (scanf("%d", &opcao) != 1) {
        opcao = 2;
    }
    limpar_buffer_entrada();

    return opcao;
}

int menu_num_jogadores() {
    printf("\n================================\n");
    printf("      MODO FIXO DE PARTIDA      \n");
    printf("================================\n");
    printf("Esta partida sempre usa %d jogadores.\n", MAX_JOGADORES);

    return MAX_JOGADORES;
}

int menu_personagem(int jogador_num) {
    int personagem;

    printf("\n================================\n");
    printf("  JOGADOR %d - SELECIONE SEU PERSONAGEM!\n", jogador_num);
    printf("================================\n");
    printf("1. Patolino\n");
    printf("2. Pato Donald\n");
    printf("3. Tio Patinhas\n");
    printf("4. Pato\n");
    printf("--------------------------------\n");
    printf("Digite sua opcao: ");
    if (scanf("%d", &personagem) != 1) {
        personagem = 1;
    }
    limpar_buffer_entrada();

    if (personagem < 1 || personagem > 4) personagem = 1;

    return personagem;
}

void nome_personagem(int id, char *buffer) {
    switch(id) {
        case 1: snprintf(buffer, 50, "Patolino"); break;
        case 2: snprintf(buffer, 50, "Pato Donald"); break;
        case 3: snprintf(buffer, 50, "Tio Patinhas"); break;
        case 4: snprintf(buffer, 50, "Pato"); break;
        default: snprintf(buffer, 50, "Desconhecido"); break;
    }
}

int menu_pause() {
    int opcao;

    printf("\n================================\n");
    printf("              PAUSE              \n");
    printf("================================\n");
    printf("1. Continuar\n");
    printf("2. Sair para o menu principal\n");
    printf("3. Sair do jogo\n");
    printf("--------------------------------\n");
    printf("Digite a opcao desejada: ");
    if (scanf("%d", &opcao) != 1) {
        opcao = 1;
    }
    limpar_buffer_entrada();

    if (opcao < 1 || opcao > 3) {
        opcao = 1;
    }

    return opcao;
}

void exibir_placar(tp_player jogadores[], int num_jogadores) {
    int i;

    printf("\n--- PLACAR ---\n");
    for (i = 0; i < num_jogadores; i++) {
        printf("%s: Casa %d\n", jogadores[i].nome, jogadores[i].posicao);
    }
}

void configurar_jogadores(tp_player jogadores[], tp_fila *fila_turnos, int num_jogadores) {
    int i, personagem;
    char nome_pers[50];

    printf("\n================================\n");
    printf("   SELECIONANDO PERSONAGENS     \n");
    printf("================================\n");

    inicializar_jogadores(jogadores, MAX_JOGADORES);
    inicializa_fila(fila_turnos);

    for (i = 0; i < num_jogadores; i++) {
        personagem = menu_personagem(i + 1);
        nome_personagem(personagem, nome_pers);
        snprintf(jogadores[i].nome, sizeof(jogadores[i].nome), "Jogador %d (%s)", i + 1, nome_pers);
        printf("\n%s selecionado!\n", jogadores[i].nome);
        insere_fila(fila_turnos, jogadores[i].id_player);
    }
}

void exibir_inicio_partida(tp_player jogadores[], int num_jogadores, tp_pilha *pilha_cartas) {
    int i;

    printf("\n================================\n");
    printf("        JOGO INICIADO!          \n");
    printf("================================\n");
    printf("Jogadores participando:\n");
    for (i = 0; i < num_jogadores; i++) {
        printf("  - %s (Casa %d)\n", jogadores[i].nome, jogadores[i].posicao);
    }
    printf("\nCartas carregadas: %d cartas\n", tamanho_pilha_cartas(pilha_cartas));
}

int executar_partida() {
    tp_player jogadores[MAX_JOGADORES];
    tp_fila fila_turnos;
    tp_pilha pilha_cartas;
    tp_item id_jogador;
    int num_jogadores, jogador_atual, rodada, resultado, pausa;
    int em_jogo = 1;

    num_jogadores = menu_num_jogadores();

    configurar_jogadores(jogadores, &fila_turnos, num_jogadores);
    carregar_perguntas(&pilha_cartas);
    exibir_inicio_partida(jogadores, num_jogadores, &pilha_cartas);

    rodada = 1;

    while (em_jogo) {
        if (!remove_fila(&fila_turnos, &id_jogador)) {
            break;
        }

        jogador_atual = id_jogador - 1;
        insere_fila(&fila_turnos, id_jogador);

        printf("\n========================================\n");
        printf("           RODADA %d                     \n", rodada);
        printf("========================================\n");
        printf("Vez de: %s\n", jogadores[jogador_atual].nome);
        printf("Posicao atual: Casa %d\n", jogadores[jogador_atual].posicao);
        printf("----------------------------------------\n");
        printf("Pressione ENTER para sortear uma pergunta...");
        getchar();

        resultado = fazer_pergunta(&pilha_cartas);

        if (resultado > 0) {
            jogadores[jogador_atual].posicao += resultado;
            printf(
                "\n%s avancou para a casa %d!\n",
                jogadores[jogador_atual].nome,
                jogadores[jogador_atual].posicao
            );

            if (jogadores[jogador_atual].posicao >= 20) {
                printf("\n========================================\n");
                printf("        VENCEDOR!!!                      \n");
                printf("========================================\n");
                printf("%s VENCEU O JOGO!\n", jogadores[jogador_atual].nome);
                printf("========================================\n");
                em_jogo = 0;
                continue;
            }
        }

        exibir_placar(jogadores, num_jogadores);
        pausa = menu_pause();

        if (pausa == 2) {
            em_jogo = 0;
        } else if (pausa == 3) {
            return 0;
        }

        if (jogador_atual == num_jogadores - 1) {
            rodada++;
        }
    }

    return 1;
}

#endif