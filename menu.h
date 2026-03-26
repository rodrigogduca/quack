#ifndef MENU_H
#define MENU_H

#include <stdio.h>

int menu_princiapal(int *opcao_menu){

    printf("================================\n");
    printf("            QUACK QUIZ         \n");
    
    //FAZER O PATO DO CLUBE
    
    printf("================================\n");
    printf("1. Novo jogo\n");
    printf("2. Sair\n");
    printf("================================\n");
    printf("Digite alguma opcao: ");
    scanf("%d\n", &opcao_menu);

}
 
void menu_personagem(int *personagem) {

    printf("================================\n\n");
    printf("SELECIONE SEU PERSONAGEM!\n\n");
    printf("================================\n\n");
    printf("1. Patolino\n");
    printf("2. Pato Donald\n");
    printf("3. Tio Patinhas\n");
    printf("4. Pato\n\n");
    
    printf("--------------------------------\n");
    printf("0. Voltar\n");
    printf("--------------------------------\n");
    printf("Digite alguma opcao: ");
    scanf("%d", &personagem);
    
}

void menu_pause(int *pause) {
    int opcao;
    printf("\n================================\n");
    printf("              PAUSE              \n");
    printf("================================\n\n");
    
    printf("         1. Continuar\n");
    printf("  2. Sair para o menu principal\n");
    printf("--------------------------------\n");
    printf("        3. Sair do jogo\n");
    printf("--------------------------------\n");
    printf("    Digite a opcao desejada:"); 
    scanf("%d", &pause); 
    printf("\n\n================================");

    
}

#endif