// main.c
// Ponto de entrada do jogo; delega o fluxo principal para o menu.

#include <stdlib.h>
#include <time.h>

#include "menu.h" 

// Executa o loop principal do jogo.
int main() {
    int opcao_menu;
    int jogo_ativo = 1; // Booleano primitivo que controla se a aplicacao deve continuar ligada

    // Inicializa o gerador padrao para evitar sequencias repetidas.
    srand((unsigned)time(NULL)); // Inicializa a semente aleatoria com o horario atual.

    // Loop principal do programa: menu inicial e controle de novas partidas.
    while (jogo_ativo) {
        // Mostra o menu principal e captura a opcao.
        opcao_menu = menu_principal();

        if (opcao_menu == 1) {
            // Inicia a maquina de estados de uma nova partida. 
            // O retorno desta funcao define se o jogo deve voltar ao menu principal ou fechar.
            jogo_ativo = executar_partida();
        } else {
            // Encerra o programa.
            exibir_mensagem_saida();
            jogo_ativo = 0;
        }
    }

    return 0;
}
