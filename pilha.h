// pilha.h
// Declaracoes da estrutura Pilha Estatica (LIFO - Last In, First Out).
// Usada no gerenciamento das cartas-pergunta, organizadas por unidade e dificuldade.

#ifndef PILHA_H
#define PILHA_H

#ifndef MAX
#define MAX 100
#endif

#ifndef TP_ITEM_DEFINED
#define TP_ITEM_DEFINED
typedef int tp_item;  // Tipo generico dos itens armazenados na pilha
#endif

// Estrutura de uma pilha estatica (vetor + indice de topo)
typedef struct {
    int topo;          // Indice do elemento no topo (-1 = vazia)
    tp_item item[MAX]; // Vetor que armazena os elementos
} tp_pilha;

// Inicializa a pilha (topo = -1)
void inicializa_pilha(tp_pilha *p);

// Retorna 1 se a pilha esta vazia, 0 caso contrario
int pilha_vazia(tp_pilha *p);

// Retorna 1 se a pilha esta cheia, 0 caso contrario
int pilha_cheia(tp_pilha *p);

// Insere um elemento no topo da pilha. Retorna 1 se sucesso, 0 se pilha cheia.
int push(tp_pilha *p, tp_item e);

// Remove o elemento do topo da pilha e o armazena em *e. Retorna 1 se sucesso, 0 se vazia.
int pop(tp_pilha *p, tp_item *e);

#endif
