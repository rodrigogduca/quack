// Bibliotecas do projeto
#include "pilha.h"
#include "fila.h"
#include "dados.h"
#include "menu.h"
#include "funcoes.h"




int main() {
    int opcao_menu;
    bool encerrar = false;

    srand((unsigned int)time(NULL));

    while (!encerrar) {
        limpar_terminal();
        opcao_menu = menu_principal();

        if (opcao_menu == 1) {
            int retorno_partida = executar_partida();
            if (retorno_partida == 1) {
                encerrar = true;
            }
        } else if (opcao_menu == 2) {
            menu_instrucoes();
        } else {
            encerrar = true;
        }
    }

    limpar_terminal();
    printf("\n\n\n\n\n\n\n======================================\n    Obrigado por jogar QUACK QUIZ!\n======================================\n\n\n\n\n\n\n");
    return 0;
}

