// pilha.c
// Implementacao da Pilha Estatica (LIFO).
// Operacoes: push (inserir no topo), pop (remover do topo).

#include "pilha.h"

// Inicializa a pilha definindo topo como -1 (vazia)
void inicializa_pilha(tp_pilha *p) {
    p->topo = -1;
}

// Retorna 1 se topo == -1 (nenhum elemento), 0 caso contrario
int pilha_vazia(tp_pilha *p) {
    return p->topo == -1;
}

// Retorna 1 se topo alcancou o ultimo indice do vetor
int pilha_cheia(tp_pilha *p) {
    return p->topo == MAX - 1;
}

// Insere 'e' no topo da pilha. Nao insere se estiver cheia.
int push(tp_pilha *p, tp_item e) {
    if (pilha_cheia(p)) return 0;
    p->topo++;
    p->item[p->topo] = e;
    return 1;
}

// Remove o elemento do topo e o copia para *e. Nao altera *e se vazia.
int pop(tp_pilha *p, tp_item *e) {
    if (pilha_vazia(p)) return 0;
    *e = p->item[p->topo];
    p->topo--;
    return 1;
}
