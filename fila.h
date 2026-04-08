#ifndef FILA_H
#define FILA_H

/*
 * fila.h
 * -----------------------------------------------------------------------------
 * Implementacao de fila circular estatica.
 * Regra da fila: FIFO (First In, First Out).
 */

#include <stdio.h>

#ifndef MAX
#define MAX 100
#endif

#ifndef TP_ITEM_DEFINED
#define TP_ITEM_DEFINED
typedef int tp_item;
#endif

/*
 * tp_fila
 * -----------------------------------------------------------------------------
 * item: vetor com os elementos.
 * ini : indice logico anterior ao primeiro elemento.
 * fim : indice do ultimo elemento inserido.
 */
typedef struct {
    tp_item item[MAX];
    int ini;
    int fim;
} tp_fila;

/* Inicializa fila vazia. */
void inicializa_fila(tp_fila *f) {
    f->ini = MAX - 1;
    f->fim = MAX - 1;
}

/* Retorna 1 se a fila estiver vazia, senao 0. */
int fila_vazia(tp_fila *f) {
    return f->ini == f->fim;
}

/* Calcula proxima posicao no vetor circular. */
int proximo(int pos) {
    if (pos == MAX - 1) return 0;
    return pos + 1;
}

/* Retorna 1 se a fila estiver cheia, senao 0. */
int fila_cheia(tp_fila *f) {
    return proximo(f->fim) == f->ini;
}

/*
 * insere_fila - Insere elemento no fim da fila.
 * Retorna 1 em caso de sucesso, 0 se a fila estiver cheia.
 */
int insere_fila(tp_fila *f, tp_item e) {
    if (fila_cheia(f)) return 0;
    f->fim = proximo(f->fim);
    f->item[f->fim] = e;
    return 1;
}

/*
 * remove_fila - Remove elemento do inicio da fila.
 * Retorna 1 em caso de sucesso, 0 se a fila estiver vazia.
 */
int remove_fila(tp_fila *f, tp_item *e) {
    if (fila_vazia(f)) return 0;
    f->ini = proximo(f->ini);
    *e = f->item[f->ini];
    return 1;
}

/* Retorna a quantidade de elementos da fila (em copia). */
int tamanho_fila(tp_fila f) {
    int cont = 0;
    tp_item e;
    while (!fila_vazia(&f)) {
        remove_fila(&f, &e);
        cont++;
    }
    return cont;
}

/* Imprime copia da fila para depuracao. */
void imprime_fila(tp_fila f) {
    tp_item e;
    while (!fila_vazia(&f)) {
        remove_fila(&f, &e);
        printf("\n %d", e);
    }
}

#endif
