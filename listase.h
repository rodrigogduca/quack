#ifndef LISTASE_H
#define LISTASE_H

/* Implementacao de lista simplesmente encadeada. */

#include <stdio.h>
#include <stdlib.h>

#ifndef TP_ITEM_DEFINED
#define TP_ITEM_DEFINED
typedef int tp_item;
#endif

/* No da lista: guarda o valor e ponteiro para o proximo no. */
typedef struct tp_no {
    tp_item info;
    struct tp_no *prox;
} tp_no;

/* Estrutura principal da lista com inicio, fim e tamanho atual. */
typedef struct {
    tp_no *inicio;
    tp_no *fim;
    int tamanho;
} tp_listase;

/* Inicializa a lista em estado vazio. */
void inicializa_listase(tp_listase *lista) {
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
}

/* Retorna 1 se a lista estiver vazia; caso contrario, retorna 0. */
int listase_vazia(tp_listase *lista) {
    return lista->inicio == NULL;
}

/* Insere um elemento no inicio da lista. */
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

/* Insere um elemento no fim da lista. */
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

/* Remove o primeiro elemento da lista e devolve em *e. */
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

/* Busca um valor na lista; retorna 1 se encontrar, senao 0. */
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

/* Retorna a quantidade de elementos atualmente armazenados. */
int tamanho_listase(tp_listase *lista) {
    return lista->tamanho;
}

/* Imprime os elementos na ordem em que aparecem na lista. */
void imprime_listase(tp_listase *lista) {
    tp_no *atual = lista->inicio;

    printf("[ ");
    while (atual != NULL) {
        printf("%d ", atual->info);
        atual = atual->prox;
    }
    printf("]\n");
}

/* Libera toda memoria alocada para os nos e reinicializa a lista. */
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