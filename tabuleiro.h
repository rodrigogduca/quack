#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdio.h>
#include <stdlib.h>

#include "config.h"

typedef enum {
    CASA_NORMAL = 0,
    CASA_PERGUNTA,
    CASA_AVANCO,
    CASA_RECUO
} tp_tipo_casa;

typedef struct tp_casa {
    int numero;
    int unidade;
    tp_tipo_casa tipo;
    int efeito;
    struct tp_casa *prox;
    struct tp_casa *ant;
} tp_casa;

typedef struct {
    tp_casa *inicio;
    tp_casa *fim;
    int total;
} tp_tabuleiro;

void inicializar_tabuleiro(tp_tabuleiro *tab) {
    tab->inicio = NULL;
    tab->fim = NULL;
    tab->total = 0;
}

int unidade_por_posicao(int pos) {
    if (pos <= 0) {
        return 1;
    }
    return ((pos - 1) / CASAS_POR_UNIDADE) + 1;
}

tp_tipo_casa tipo_casa_por_posicao(int pos) {
    int offset;

    if (pos <= 0) {
        return CASA_NORMAL;
    }

    offset = ((pos - 1) % CASAS_POR_UNIDADE) + 1;

    if (offset == 2 || offset == 5 || offset == 8) {
        return CASA_PERGUNTA;
    }

    if (offset == 3) {
        return CASA_AVANCO;
    }

    if (offset == 7) {
        return CASA_RECUO;
    }

    return CASA_NORMAL;
}

int efeito_casa(tp_tipo_casa tipo) {
    if (tipo == CASA_AVANCO) {
        return AVANCO_CASA_ESPECIAL;
    }
    if (tipo == CASA_RECUO) {
        return RECUO_CASA_ESPECIAL;
    }
    return 0;
}

int adicionar_casa(tp_tabuleiro *tab, int numero) {
    tp_casa *nova = (tp_casa *)malloc(sizeof(tp_casa));
    if (nova == NULL) {
        return 0;
    }

    nova->numero = numero;
    nova->unidade = unidade_por_posicao(numero);
    nova->tipo = tipo_casa_por_posicao(numero);
    nova->efeito = efeito_casa(nova->tipo);
    nova->prox = NULL;
    nova->ant = tab->fim;

    if (tab->fim != NULL) {
        tab->fim->prox = nova;
    } else {
        tab->inicio = nova;
    }

    tab->fim = nova;
    tab->total++;
    return 1;
}

void destruir_tabuleiro(tp_tabuleiro *tab);

int criar_tabuleiro(tp_tabuleiro *tab) {
    int i;

    inicializar_tabuleiro(tab);

    for (i = 1; i <= TOTAL_CASAS; i++) {
        if (!adicionar_casa(tab, i)) {
            destruir_tabuleiro(tab);
            return 0;
        }
    }

    return 1;
}

void destruir_tabuleiro(tp_tabuleiro *tab) {
    tp_casa *atual = tab->inicio;
    tp_casa *prox;

    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    inicializar_tabuleiro(tab);
}

tp_casa *obter_casa(tp_tabuleiro *tab, int pos) {
    tp_casa *atual;
    int i;

    if (tab == NULL || pos < 1 || pos > tab->total) {
        return NULL;
    }

    atual = tab->inicio;
    for (i = 1; i < pos && atual != NULL; i++) {
        atual = atual->prox;
    }

    return atual;
}

int mover_posicao(tp_tabuleiro *tab, int pos_atual, int passos) {
    tp_casa *casa;
    int restante;

    if (tab == NULL || tab->inicio == NULL) {
        return pos_atual;
    }

    casa = obter_casa(tab, pos_atual);
    if (casa == NULL) {
        return pos_atual;
    }

    restante = passos;

    while (restante > 0 && casa->prox != NULL) {
        casa = casa->prox;
        restante--;
    }

    while (restante < 0 && casa->ant != NULL) {
        casa = casa->ant;
        restante++;
    }

    return casa->numero;
}

#endif
