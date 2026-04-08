#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <string.h>

int menu_principal() {
    int opcao;
    
    printf("\n================================\n");
    printf("         QUACK QUIZ             \n");
    printf("================================\n");
    printf("1. Novo jogo\n");
    printf("2. Sair\n");
    printf("================================\n");
    printf("Digite alguma opcao: ");
    scanf("%d", &opcao);
    
    return opcao;
}

int menu_num_jogadores() {
    int num;
    
    printf("\n================================\n");
    printf("   QUANTOS JOGADORES? (2-4)     \n");
    printf("================================\n");
    printf("Digite o numero de jogadores: ");
    scanf("%d", &num);
    
    if (num < 2) num = 2;
    if (num > 4) num = 4;
    
    return num;
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
    scanf("%d", &personagem);
    
    if (personagem < 1 || personagem > 4) personagem = 1;
    
    return personagem;
}

// Copia o nome do personagem para o buffer fornecido
void nome_personagem(int id, char *buffer) {
    switch(id) {
        case 1: sprintf(buffer, "Patolino"); break;
        case 2: sprintf(buffer, "Pato Donald"); break;
        case 3: sprintf(buffer, "Tio Patinhas"); break;
        case 4: sprintf(buffer, "Pato"); break;
        default: sprintf(buffer, "Desconhecido"); break;
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
    scanf("%d", &opcao);
    
    return opcao;
}

#endif