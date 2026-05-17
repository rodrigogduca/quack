#ifndef PILHA_H
#define PILHA_H

// pilha.h
// Implementacao de pilha estatica.
// Regra da pilha: LIFO (Last In, First Out).


// Capacidade maxima da pilha (numero de itens).
#ifndef MAX
#define MAX 100
#endif

// Tipo base armazenado na pilha.
#ifndef TP_ITEM_DEFINED
#define TP_ITEM_DEFINED
typedef int tp_item;
#endif

// tp_pilha
// ---------------------------------------------------------------------------
// topo: indice do elemento no topo da pilha.
// item: vetor que armazena os elementos.
typedef struct {
    int topo;
    tp_item item[MAX];
} tp_pilha;

// Inicializa pilha vazia (topo = -1).
void inicializa_pilha(tp_pilha *p) {
    // Topo -1 indica pilha vazia.
    p->topo = -1;
}

// Retorna 1 se a pilha estiver vazia, senao 0.
int pilha_vazia(tp_pilha *p) {
    return p->topo == -1;
}

// Retorna 1 se a pilha estiver cheia, senao 0.
int pilha_cheia(tp_pilha *p) {
    return p->topo == MAX - 1;
}

// push - Insere elemento no topo.
// Retorna 1 em sucesso, 0 se a pilha estiver cheia.
int push(tp_pilha *p, tp_item e) {
    // Confere espaco antes de empilhar.
    if (pilha_cheia(p)) return 0;
    p->topo++;
    p->item[p->topo] = e;
    return 1;
}

// pop - Remove elemento do topo e escreve em *e.
// Retorna 1 em sucesso, 0 se a pilha estiver vazia.
int pop(tp_pilha *p, tp_item *e) {
    // Evita remover em pilha vazia.
    if (pilha_vazia(p)) return 0;
    *e = p->item[p->topo];
    p->topo--;
    return 1;
}


#endif
