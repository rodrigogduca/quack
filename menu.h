#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include "dados.h"

#define MENU_MAX_JOGADORES 4


static void limpar_terminal(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}


static void aguardar_enter(void) {
    char linha[8];
    printf("\nPressione ENTER para continuar...");
    fgets(linha, sizeof(linha), stdin);
}


static int ler_inteiro_intervalo(const char *mensagem, int minimo, int maximo) {
    char linha[64];
    int valor;
    char extra;

    while (1) {
        printf("%s", mensagem);

        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (sscanf(linha, " %d %c", &valor, &extra) == 1 && valor >= minimo && valor <= maximo) {
            return valor;
        }

        printf("Entrada invalida. Digite um numero entre %d e %d.\n", minimo, maximo);
    }
}


static int menu_principal(void) {

    printf("================================\n");
    printf("            QUACK QUIZ          \n");
    printf("================================\n");
    printf("1. Novo jogo\n");
    printf("2. Instrucoes\n");
    printf("3. Sair\n");
    printf("================================\n");
    return ler_inteiro_intervalo("Digite uma opcao: ", 1, 3);

}


static void menu_instrucoes(void) {
    limpar_terminal();
    printf("===============================================================\n");
    printf("                          INSTRUCOES          \n");
    printf("===============================================================\n");
    printf("         - O jogo suporta de 2 a 4 jogadores.\n");
    printf("         - Cada jogador escolhe um personagem diferente.\n");
    printf("         - No turno, uma carta de pergunta e exibida.\n");
    printf("         - Se acertar, avanca as casas indicadas na carta.\n");
    printf("         - O primeiro a chegar na casa 25 vence.\n");
    printf("         - Digite 0 na resposta para abrir o menu de pausa.\n");
    printf("===============================================================\n");
    aguardar_enter();
}


static int menu_num_jogadores(void) {
    printf("================================\n");
    printf("        QUANTOS JOGADORES?      \n");
    printf("================================\n");
    return ler_inteiro_intervalo("\nDigite o numero de jogadores (2 a 4): ", 2, 4);
}


static void exibir_personagens_disponiveis(const int usados[MENU_MAX_JOGADORES]) {
    printf("================================\n");
    printf("      SELECIONE O PERSONAGEM    \n");
    printf("================================\n");

    for (int i = 0; i < MENU_MAX_JOGADORES; i++) {
        printf("%d. %s", i + 1, player[i].personagem);
        if (usados[i]) {
            printf(" (indisponivel)");
        }
        printf("\n");
    }

    printf("0. Voltar para selecao anterior\n");
    printf("================================\n");
}


static int menu_personagem(const int usados[MENU_MAX_JOGADORES], int indice_jogador) {
    int personagem;

    while (1) {
        printf("\n      PLAYER %d\n", indice_jogador + 1);
        exibir_personagens_disponiveis(usados);
        personagem = ler_inteiro_intervalo("Opcao: ", 0, 4);

        if (personagem == 0) {
            return 0;
        }

        if (usados[personagem - 1]) {
            printf("Esse personagem ja foi escolhido.\n");
            continue;
        }

        return personagem;
    }
}


static int menu_pause(void) {
    printf("\n================================\n");
    printf("              PAUSE            \n");
    printf("================================\n");
    printf("1. Continuar turno\n");
    printf("2. Sair para menu principal\n");
    printf("--------------------------------\n");
    printf("3. Encerrar jogo\n");
    printf("================================\n");
    return ler_inteiro_intervalo("Escolha: ", 1, 3);
}


#endif