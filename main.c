#include "menu.h"
#include "dados.h"

int main() {
    int opcao_menu;
    int jogo_ativo = 1;

    /* Loop principal do programa: menu inicial e controle de novas partidas. */
    while (jogo_ativo) {
        opcao_menu = menu_principal();

        if (opcao_menu == 1) {
            jogo_ativo = executar_partida();
        } else {
            exibir_mensagem_saida();
            jogo_ativo = 0;
        }
    }

    return 0;
}
