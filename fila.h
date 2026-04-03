#ifndef FILA_H
#define FILA_H

/*
 * fila.h
 * -----------------------------------------------------------------------------
 * Implementacao de fila circular estatica.
 * Regra da fila: FIFO (First In, First Out).
 *
 * Esta estrutura e usada no jogo para controlar a ordem dos turnos.
 */

/*
 * Biblioteca nativa
 * -----------------------------------------------------------------------------
 * stdio.h: utilizada nas funcoes de impressao para depuracao.
 */
#include <stdio.h>

/* Capacidade maxima da fila. */
#define MAX 100

/* Tipo de dado armazenado na fila (inteiro). */
typedef int tp_item;

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
    if (f->ini == f->fim) return 1;
    return 0;
}

/* Calcula proxima posicao no vetor circular. */
int proximo(int pos) {
    if (pos == MAX - 1) return 0;
    return ++pos;
}

/* Retorna 1 se a fila estiver cheia, senao 0. */
int fila_cheia(tp_fila *f) {
    if (proximo(f->fim) == f->ini) return 1;
    return 0;
}

/*
 * Insere elemento no fim da fila.
 * Retorna 1 em caso de sucesso, 0 se a fila estiver cheia.
 */
int insere_fila(tp_fila *f, tp_item e) {
    if (fila_cheia(f)) return 0;
    f->fim = proximo(f->fim);
    f->item[f->fim] = e;
    return 1;
}

/*
 * Remove elemento do inicio da fila.
 * Retorna 1 em caso de sucesso, 0 se a fila estiver vazia.
 */
int remove_fila(tp_fila *f, tp_item *e) {
    if (fila_vazia(f)) return 0;
    f->ini = proximo(f->ini);
    *e = f->item[f->ini];
    return 1;
}

/*
 * Imprime copia da fila sem alterar a estrutura original.
 * Funcao util para depuracao.
 */
void imprime_fila(tp_fila f) {
    tp_item e;
    while (!fila_vazia(&f)) {
        remove_fila(&f, &e);
        printf("\n %d", e);
    }
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

#endif