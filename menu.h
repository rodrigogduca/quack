#ifndef MENU_H
#define MENU_H

/*
 * menu.h
 * -----------------------------------------------------------------------------
 * Biblioteca de interface textual do jogo.
 * Centraliza:
 * - limpeza de tela;
 * - leitura segura de inteiros com validacao;
 * - exibicao de menus (principal, pausa, selecao de jogadores/personagens).
 */

/*
 * Bibliotecas nativas
 * -----------------------------------------------------------------------------
 * stdio.h: printf, fgets, sscanf, fflush e clearerr para E/S no terminal.
 */
#include <stdio.h>

/*
 * Bibliotecas do projeto
 * -----------------------------------------------------------------------------
 * dados.h: acesso ao vetor global player[] para mostrar personagens disponiveis.
 */
#include "dados.h"

/* Limite de personagens/jogadores exibidos no menu de selecao. */
#define MENU_MAX_JOGADORES 4

/*
 * As funcoes deste cabecalho sao static e foram projetadas para serem
 * usadas dentro da unidade de compilacao principal do jogo.
 */

/*
 * limpar_terminal
 * -----------------------------------------------------------------------------
 * Limpa a tela e posiciona o cursor no canto superior esquerdo usando
 * sequencias ANSI. Funciona em terminais que suportam esse padrao.
 */
static void limpar_terminal(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

/*
 * aguardar_enter
 * -----------------------------------------------------------------------------
 * Pausa o fluxo para o usuario visualizar mensagens antes de continuar.
 */
static void aguardar_enter(void) {
    char linha[8];
    printf("\nPressione ENTER para continuar...");
    fgets(linha, sizeof(linha), stdin);
}

/*
 * ler_inteiro_intervalo
 * -----------------------------------------------------------------------------
 * Le um inteiro dentro de [minimo, maximo].
 * - Usa fgets para evitar problemas de buffer do scanf.
 * - Usa sscanf para validar formato numerico.
 * - Repete ate receber um valor valido.
 */
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

/*
 * menu_principal
 * -----------------------------------------------------------------------------
 * Exibe o menu inicial e retorna a opcao escolhida:
 * 1 = Novo jogo | 2 = Instrucoes | 3 = Sair.
 */
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

/*
 * menu_instrucoes
 * -----------------------------------------------------------------------------
 * Mostra as regras principais do jogo.
 */
static void menu_instrucoes(void) {
    limpar_terminal();
    printf("===============================================================\n");
    printf("                          INSTRUCOES          \n");
    printf("===============================================================\n");
    printf("         - O jogo suporta de 2 a 4 jogadores.\n");
    printf("         - Cada jogador escolhe um personagem diferente.\n");
    printf("         - No turno, voce escolhe rolar dado ou abrir pause.\n");
    printf("         - No turno, o jogador rola um dado de 1 a 6.\n");
    printf("         - Carta aparece ao cair em casa multipla de 3.\n");
    printf("         - Se acertar a carta, avanca as casas de bonus.\n");
    printf("         - O primeiro a chegar na casa 25 vence.\n");
    printf("         - Digite 0 na resposta para abrir o menu de pausa.\n");
    printf("===============================================================\n");
    aguardar_enter();
}

/*
 * menu_num_jogadores
 * -----------------------------------------------------------------------------
 * Solicita quantidade de jogadores permitida pelo jogo (2 a 4).
 */
static int menu_num_jogadores(void) {
    printf("================================\n");
    printf("        QUANTOS JOGADORES?      \n");
    printf("================================\n");
    return ler_inteiro_intervalo("\nDigite o numero de jogadores (2 a 4): ", 2, 4);
}

/*
 * exibir_personagens_disponiveis
 * -----------------------------------------------------------------------------
 * Lista os personagens e marca quais ja foram escolhidos.
 */
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

/*
 * menu_personagem
 * -----------------------------------------------------------------------------
 * Permite ao jogador escolher um personagem ainda disponivel.
 * Retorna:
 * - 0 para voltar;
 * - 1..4 para personagem valido.
 */
static int menu_personagem(const int usados[MENU_MAX_JOGADORES], int indice_jogador) {
    int personagem;

    while (1) {
        printf("\n              PLAYER %d\n", indice_jogador + 1);
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

/*
 * menu_pause
 * -----------------------------------------------------------------------------
 * Exibe opcoes de pausa do turno.
 * Retorno:
 * 1 = continuar | 2 = voltar ao menu principal | 3 = encerrar jogo.
 */
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