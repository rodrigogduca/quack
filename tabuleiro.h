#ifndef TABULEIRO_H
#define TABULEIRO_H

// tabuleiro.h
// ---------------------------------------------------------------------------
// Estrutura do tabuleiro dinamico e suas operacoes.

#include <stdio.h>
#include <stdlib.h>

#include "config.h"

// Tipos de casas ao longo do tabuleiro.
typedef enum {
    CASA_NORMAL = 0,
    CASA_PERGUNTA,
    CASA_AVANCO,
    CASA_RECUO
} tp_tipo_casa;

typedef struct tp_casa {
    int numero; // Numero da casa no tabuleiro.
    int unidade; // Unidade correspondente a posicao.
    tp_tipo_casa tipo; // Tipo logico da casa.
    int efeito; // Avanco/recuo das casas especiais.
    struct tp_casa *prox; // Proxima casa.
    struct tp_casa *ant; // Casa anterior.
} tp_casa;

typedef struct {
    tp_casa *inicio; // Primeira casa.
    tp_casa *fim; // Ultima casa.
    int total; // Total de casas.
} tp_tabuleiro;

// Inicializa estrutura do tabuleiro.
void inicializar_tabuleiro(tp_tabuleiro *tab) {
    // Inicializa ponteiros e contador.
    tab->inicio = NULL;
    tab->fim = NULL;
    tab->total = 0;
}

// Calcula a unidade correspondente a posicao.
int unidade_por_posicao(int pos) {
    // Garante unidade minima para posicoes invalidas.
    if (pos <= 0) {
        return 1;
    }
    // Divide em blocos de CASAS_POR_UNIDADE.
    return ((pos - 1) / CASAS_POR_UNIDADE) + 1;
}

// Define o tipo de casa de acordo com a posicao.
tp_tipo_casa tipo_casa_por_posicao(int pos) {
    int offset;

    // Posicoes invalidas sao tratadas como normais.
    if (pos <= 0) {
        return CASA_NORMAL;
    }

    // Offset dentro da unidade para decidir o tipo.
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

// Retorna o efeito numerico de uma casa especial.
int efeito_casa(tp_tipo_casa tipo) {
    // Retorna valores predefinidos para bonus/penalidade.
    if (tipo == CASA_AVANCO) {
        return AVANCO_CASA_ESPECIAL;
    }
    if (tipo == CASA_RECUO) {
        return RECUO_CASA_ESPECIAL;
    }
    return 0;
}

// Cria e encadeia uma nova casa no fim do tabuleiro.
int adicionar_casa(tp_tabuleiro *tab, int numero) {
    // Aloca a nova casa.
    tp_casa *nova = (tp_casa *)malloc(sizeof(tp_casa));
    if (nova == NULL) {
        return 0;
    }
    // Preenche metadados da casa.
    nova->numero = numero;
    nova->unidade = unidade_por_posicao(numero);
    nova->tipo = tipo_casa_por_posicao(numero);
    nova->efeito = efeito_casa(nova->tipo);
    nova->prox = NULL;
    nova->ant = tab->fim;

    // Encadeia no fim da lista.
    if (tab->fim != NULL) {
        tab->fim->prox = nova;
    } else {
        tab->inicio = nova;
    }

    tab->fim = nova;
    tab->total++;
    return 1;
}

// Libera toda memoria do tabuleiro.
void destruir_tabuleiro(tp_tabuleiro *tab);

// Cria o tabuleiro completo com TOTAL_CASAS.
int criar_tabuleiro(tp_tabuleiro *tab) {
    int i;

    // Comeca com estrutura vazia.
    inicializar_tabuleiro(tab);

    // Cria todas as casas em ordem.
    for (i = 1; i <= TOTAL_CASAS; i++) {
        if (!adicionar_casa(tab, i)) {
            // Em caso de falha, libera recursos parciais.
            destruir_tabuleiro(tab);
            return 0;
        }
    }

    return 1;
}

// Libera as casas e reinicializa o tabuleiro.
void destruir_tabuleiro(tp_tabuleiro *tab) {
    tp_casa *atual = tab->inicio;
    tp_casa *prox;

    // Libera casa por casa.
    while (atual != NULL) {
        prox = atual->prox;
        free(atual);
        atual = prox;
    }

    // Reinicializa ponteiros.
    inicializar_tabuleiro(tab);
}

// Busca ponteiro da casa pela posicao.
tp_casa *obter_casa(tp_tabuleiro *tab, int pos) {
    tp_casa *atual;
    int i;

    // Valida faixa e ponteiro.
    if (tab == NULL || pos < 1 || pos > tab->total) {
        return NULL;
    }

    // Percorre a lista ate a posicao solicitada.
    atual = tab->inicio;
    for (i = 1; i < pos && atual != NULL; i++) {
        atual = atual->prox;
    }

    return atual;
}

// Move para frente/tras respeitando os limites do tabuleiro.
int mover_posicao(tp_tabuleiro *tab, int pos_atual, int passos) {
    tp_casa *casa;
    int restante;

    // Caso o tabuleiro esteja vazio, retorna a posicao atual.
    if (tab == NULL || tab->inicio == NULL) {
        return pos_atual;
    }

    // Busca a casa atual.
    casa = obter_casa(tab, pos_atual);
    if (casa == NULL) {
        return pos_atual;
    }

    // Move passo a passo respeitando os limites.
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
