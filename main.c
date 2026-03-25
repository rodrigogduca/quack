#include "pilha.h"
#include "fila.h"
// #include <stdio.h> | Já tem nas outras bibliotecas, entao nao precisa incluir aqui, pois ja tem nas outras, e se tiver mais de uma vez nao tem problema, o compilador ignora as repeticoes por causa do #ifndef e #define que tem nas bibliotecas



int main() { // testando as funções de pilha e fila
    tp_pilha p1, p2;
    tp_fila f1;
    
    inicializa_pilha(&p1);
    inicializa_pilha(&p2);
    inicializa_fila(&f1);
    
    push(&p1, 10);
    push(&p1, 20);
    push(&p1, 30);
    
    push(&p2, 10);
    push(&p2, 20);
    push(&p2, 30);
    
    if (pilhas_iguais(p1, p2)) {
        printf("As pilhas são iguais.\n");
    } else {
        printf("As pilhas são diferentes.\n");
    }
    
    retira_impares(&p1);
    
    printf("Pilha p1 após retirar ímpares:\n");
    while (!pilha_vazia(&p1)) {
        tp_item e;
        pop(&p1, &e);
        printf("%d ", e);
    }
    printf("\n");
    
    return 0;
}