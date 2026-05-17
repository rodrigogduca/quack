#ifndef PILHA_H
#define PILHA_H

// pilha.h
// Implementacao de pilha estatica.
// Regra da pilha: LIFO (Last In, First Out).

#include <stdio.h>

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

// top - Consulta (sem remover) o elemento no topo.
int top(tp_pilha *p, tp_item *e) {
    // Apenas consulta o topo sem alterar a pilha.
    if (pilha_vazia(p)) return 0;
    *e = p->item[p->topo];
    return 1;
}

// Retorna a altura atual da pilha.
int altura_pilha(tp_pilha *p) {
    return p->topo + 1;
}

// Imprime uma copia da pilha para depuracao.
void imprime_pilha(tp_pilha p) {
    tp_item e;
    printf("\n");
    // Desempilha copia para imprimir sem alterar a pilha original.
    while (!pilha_vazia(&p)) {
        pop(&p, &e);
        printf("%d ", e);
    }
}

// Remove todos os elementos impares de uma pilha.
void retira_impares(tp_pilha *p) {
    tp_pilha paux;
    tp_item e;

    // Usa pilha auxiliar para filtrar os pares.
    inicializa_pilha(&paux);

    while (!pilha_vazia(p)) {
        pop(p, &e);
        if (e % 2 == 0) {
            push(&paux, e);
        }
    }

    // Devolve os pares para a pilha original na ordem correta.
    while (!pilha_vazia(&paux)) {
        pop(&paux, &e);
        push(p, e);
    }
}

// Compara se duas pilhas possuem os mesmos elementos na mesma ordem.
int pilhas_iguais(tp_pilha p1, tp_pilha p2) {
    tp_item e1, e2;

    // Se as alturas diferem, nao sao iguais.
    if (altura_pilha(&p1) != altura_pilha(&p2)) {
        return 0;
    }

    while (!pilha_vazia(&p1)) {
        pop(&p1, &e1);
        pop(&p2, &e2);
        if (e1 != e2) return 0;
    }

    return 1;
}

// empilha_pilhas - Copia os elementos de p2 para o topo de p1.
// Retorna 1 em sucesso, 0 se nao houver capacidade.
int empilha_pilhas(tp_pilha *p1, tp_pilha *p2) {
    tp_item e;
    tp_pilha paux;

    // Pilha auxiliar garante a ordem correta ao empilhar.
    inicializa_pilha(&paux);

    // Verifica se ha espaco suficiente.
    if (altura_pilha(p1) + altura_pilha(p2) > MAX) {
        return 0;
    }

    while (!pilha_vazia(p2)) {
        pop(p2, &e);
        push(&paux, e);
    }

    while (!pilha_vazia(&paux)) {
        pop(&paux, &e);
        push(p1, e);
    }

    return 1;
}

#endif
