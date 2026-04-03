/*
 * main.c
 * -----------------------------------------------------------------------------
 * Este arquivo contem o ponto de entrada do jogo (funcao main).
 * A principal responsabilidade dele e:
 * 1) Inicializar a semente de aleatoriedade.
 * 2) Exibir o menu principal em loop.
 * 3) Direcionar o fluxo para instrucoes, partida ou encerramento.
 */

/*
 * Bibliotecas do projeto
 * -----------------------------------------------------------------------------
 * estruturas.h: operacoes das estruturas Pilha/Fila.
 * dados.h  : structs e dados fixos do jogo (players e cartas).
 * menu.h   : funcoes de interface textual (menus e leitura validada).
 * funcoes.h: logica da partida (tabuleiro, turnos, dado e cartas).
 */
#include "estruturas.h"
#include "dados.h"
#include "menu.h"
#include "funcoes.h"

/*
 * Bibliotecas nativas da linguagem C
 * -----------------------------------------------------------------------------
 * stdio.h  : funcoes de entrada/saida, como printf e fgets.
 */
#include <stdio.h>

/*
 * main
 * -----------------------------------------------------------------------------
 * Orquestra o ciclo principal da aplicacao:
 * - inicializa a semente do gerador pseudoaleatorio interno;
 * - exibe o menu principal continuamente;
 * - direciona para instrucoes, partida ou encerramento total.
 */
int main(void) {
    int opcao_menu;
    /* 0 = continuar no loop principal | 1 = sair do programa. */
    int encerrar = 0;

    /* Define semente fixa do gerador interno sem uso de outras bibliotecas. */
    definir_semente(0xA5A5A5A5u);

    /*
     * Loop principal da aplicacao:
     * - 1: inicia nova partida
     * - 2: mostra instrucoes
     * - 3: encerra o programa
     */
    while (!encerrar) {
        limpar_terminal();
        opcao_menu = menu_principal();

        if (opcao_menu == 1) {
            /*
             * executar_partida retorna:
             * 0 -> volta para menu principal
             * 1 -> encerra totalmente o programa
             */
            int retorno_partida = executar_partida();
            if (retorno_partida == 1) {
                encerrar = 1;
            }
        } else if (opcao_menu == 2) {
            menu_instrucoes();
        } else {
            encerrar = 1;
        }
    }

    limpar_terminal();
    printf("\n\n\n\n\n\n\n======================================\n    Obrigado por jogar QUACK QUIZ!\n======================================\n\n\n\n\n\n\n");
    return 0;
}

