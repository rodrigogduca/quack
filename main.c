// main.c
// Ponto de entrada do QUACK Boardgame.
// Inicializa o gerador de numeros aleatorios e delega o fluxo
// principal do jogo para a interface grafica (gui_main).

#include <stdlib.h>
#include <time.h>

void gui_main(void);

// Funcao principal: inicializa semente aleatoria e inicia a GUI
int main() {
    srand((unsigned)time(NULL));  // Semente baseada no tempo para numeros aleatorios
    gui_main();                    // Loop principal do jogo (interface grafica)
    return 0;
}
