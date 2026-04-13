#ifndef LISTASE_H
#define LISTASE_H

#include <stdio.h>
#include <stdlib.h>

#ifndef TP_ITEM_DEFINED
#define TP_ITEM_DEFINED
typedef int tp_item;
#endif

typedef struct tp_no {
    tp_item info;
    struct tp_no *prox;
} tp_no;

typedef struct {
    tp_no *inicio;
    tp_no *fim;
    int tamanho;
} tp_listase;

void inicializa_listase(tp_listase *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

int listase_vazia(tp_listase *lista) {
    return lista->inicio == NULL;
}

int insere_listase_no_inicio(tp_listase *lista, tp_item e) {
    tp_no *novo = (tp_no *)malloc(sizeof(tp_no));
    if (novo == NULL) {
        return 0;
    }

    novo->info = e;
    novo->prox = lista->inicio;
    lista->inicio = novo;

    if (lista->fim == NULL) {
        lista->fim = novo;
    }

    lista->tamanho++;
    return 1;
}

int insere_listase_no_fim(tp_listase *lista, tp_item e) {
    tp_no *novo = (tp_no *)malloc(sizeof(tp_no));
    if (novo == NULL) {
        return 0;
    }

    novo->info = e;
    novo->prox = NULL;

    if (listase_vazia(lista)) {
        lista->inicio = novo;
        lista->fim = novo;
    } else {
        lista->fim->prox = novo;
        lista->fim = novo;
    }

    lista->tamanho++;
    return 1;
}

int remove_listase_no_inicio(tp_listase *lista, tp_item *e) {
    tp_no *removido;

    if (listase_vazia(lista)) {
        return 0;
    }

    removido = lista->inicio;
    *e = removido->info;
    lista->inicio = removido->prox;

    if (lista->inicio == NULL) {
        lista->fim = NULL;
    }

    free(removido);
    lista->tamanho--;
    return 1;
}

int busca_listase(tp_listase *lista, tp_item valor) {
    tp_no *atual = lista->inicio;

    while (atual != NULL) {
        if (atual->info == valor) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

int tamanho_listase(tp_listase *lista) {
    return lista->tamanho;
}

void imprime_listase(tp_listase *lista) {
    tp_no *atual = lista->inicio;

    printf("[ ");
    while (atual != NULL) {
        printf("%d ", atual->info);
        atual = atual->prox;
    }
    printf("]\n");
}

void libera_listase(tp_listase *lista) {
    tp_no *atual = lista->inicio;
    tp_no *prox;

    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    inicializa_listase(lista);
}



#endif