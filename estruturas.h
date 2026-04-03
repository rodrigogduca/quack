#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

/*
 * estruturas.h
 * -----------------------------------------------------------------------------
 * Biblioteca unificada com implementacoes de pilha e fila.
 * Este arquivo concentra as duas estruturas para simplificar manutencao.
 */

#include <stdio.h>

/* Capacidade maxima compartilhada entre pilha e fila. */
#define MAX 100



//=================================================================================================================



/*
 * PILHA
 * -----------------------------------------------------------------------------
 * Implementacao de pilha estatica.
 * Regra da pilha: LIFO (Last In, First Out).
 *
 * No jogo, a pilha e usada para guardar historico de posicoes por jogador.
 */

/* Tipo base armazenado nas estruturas lineares deste arquivo. */
typedef int tp_item;

/*
 * tp_pilha
 * -----------------------------------------------------------------------------
 * topo: indice do elemento no topo da pilha.
 * item: vetor que armazena os elementos.
 */
typedef struct {
    int topo;
    tp_item item[MAX];
} tp_pilha;

/* Inicializa pilha vazia (topo = -1). */
void inicializa_pilha(tp_pilha *p) {
    p->topo = -1;
}

/* Retorna 1 se a pilha estiver vazia, senao 0. */
int pilha_vazia(tp_pilha *p) {
    if (p->topo == -1) return 1;
    return 0;
}

/* Retorna 1 se a pilha estiver cheia, senao 0. */
int pilha_cheia(tp_pilha *p) {
    if (p->topo == MAX - 1) {
        return 1;
    }
    return 0;
}

/*
 * push
 * -----------------------------------------------------------------------------
 * Insere elemento no topo.
 * Retorna 1 em sucesso, 0 se a pilha estiver cheia.
 */
int push(tp_pilha *p, tp_item e) {
    if (pilha_cheia(p)) return 0;

    p->topo++;
    p->item[p->topo] = e;
    return 1;
}

/*
 * pop
 * -----------------------------------------------------------------------------
 * Remove elemento do topo e escreve em *e.
 * Retorna 1 em sucesso, 0 se a pilha estiver vazia.
 */
int pop(tp_pilha *p, tp_item *e) {
    if (pilha_vazia(p)) return 0;
    *e = p->item[p->topo];
    p->topo--;
    return 1;
}

/*
 * top
 * -----------------------------------------------------------------------------
 * Consulta (sem remover) o elemento no topo.
 */
int top(tp_pilha *p, tp_item *e) {
    if (pilha_vazia(p)) return 0;
    *e = p->item[p->topo];
    return 1;
}

/* Imprime uma copia da pilha para depuracao. */
void imprime_pilha(tp_pilha p) {
    tp_item e;
    printf("\n");
    while (!pilha_vazia(&p)) {
        pop(&p, &e);
        printf("%d ", e);
    }
}

/* Retorna a altura atual da pilha. */
int altura_pilha(tp_pilha *p) {
    return p->topo + 1;
}

/*
 * retira_impares
 * -----------------------------------------------------------------------------
 * Remove todos os elementos impares de uma pilha.
 */
void retira_impares(tp_pilha *p) {
    tp_pilha paux;
    tp_item e;

    inicializa_pilha(&paux);

    while (!pilha_vazia(p)) {
        pop(p, &e);
        if (e % 2 == 0) {
            push(&paux, e);
        }
    }

    while (!pilha_vazia(&paux)) {
        pop(&paux, &e);
        push(p, e);
    }
}

/* Compara se duas pilhas possuem os mesmos elementos na mesma ordem. */
int pilhas_iguais(tp_pilha p1, tp_pilha p2) {
    tp_item e1, e2;

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

/*
 * empilha_pilhas
 * -----------------------------------------------------------------------------
 * Copia os elementos de p2 para o topo de p1, preservando a ordem original.
 * Retorna 1 em sucesso, 0 se nao houver capacidade.
 */
int empilha_pilhas(tp_pilha *p1, tp_pilha *p2) {
    tp_item e;
    tp_pilha paux;

    inicializa_pilha(&paux);

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



/* =============================================================================================================== */



/*
 * FILA
 * -----------------------------------------------------------------------------
 * Implementacao de fila circular estatica.
 * Regra da fila: FIFO (First In, First Out).
 *
 * Esta estrutura e usada no jogo para controlar a ordem dos turnos.
 */

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