// main.c
// Ponto de entrada do jogo; delega o fluxo principal para a GUI.

#include <stdlib.h>
#include <time.h>

#include "interface.h"

int main() {
    srand((unsigned)time(NULL));
    gui_main();
    return 0;
}
