#ifndef TABULEIRO_H
#define TABULEIRO_H

// tabuleiro.h
// Estrutura do tabuleiro dinamico e suas operacoes.

#include <stdlib.h>

// Tipos de casas ao longo do tabuleiro.

typedef int tp_tipo_casa;

// Representa uma celula individual do tabuleiro.
typedef struct tp_casa {
    int numero; // Numero da casa no tabuleiro.
    int unidade; // Unidade correspondente a posicao.
    tp_tipo_casa tipo; // Tipo logico da casa.
    int efeito; // Avanco/recuo das casas especiais.
    struct tp_casa *proxima_casa; // Ponteiro para a proxima casa do trajeto.
    struct tp_casa *casa_anterior; // Ponteiro para a casa anterior do trajeto.
} tp_casa;

// Estrutura principal que gerencia o fluxo de casas do jogo (Lista Duplamente Encadeada).
typedef struct {
    tp_casa *inicio; // Primeira casa.
    tp_casa *fim; // Ultima casa.
    int total; // Total de casas.
} tp_tabuleiro;

// Inicializa estrutura do tabuleiro.
void inicializar_tabuleiro(tp_tabuleiro *tabuleiro_referencia) {
    // Inicializa ponteiros e contador.
    tabuleiro_referencia->inicio = NULL;
    tabuleiro_referencia->fim = NULL;
    tabuleiro_referencia->total = 0;
}

// Calcula a unidade correspondente a posicao.
int unidade_por_posicao(int posicao_alvo) {
    // Garante unidade minima para posicoes invalidas.
    if (posicao_alvo <= 0) {
        return 1;
    }
    // Divide em blocos de 10 casas.
    return ((posicao_alvo - 1) / 10) + 1;
}

// Define o tipo de casa de acordo com a posicao.
tp_tipo_casa tipo_casa_por_posicao(int posicao_alvo) {
    int deslocamento;

    // Posicoes invalidas sao tratadas como normais.
    if (posicao_alvo <= 0) {
        return 0;
    }

    // Deslocamento dentro da unidade para decidir o tipo.
    deslocamento = ((posicao_alvo - 1) % 10) + 1;

    if (deslocamento == 2 || deslocamento == 5 || deslocamento == 8) {
        return 1;
    }

    if (deslocamento == 3) {
        return 2;
    }

    if (deslocamento == 7) {
        return 3;
    }

    return 0;
}

// Retorna o efeito numerico de uma casa especial.
int efeito_casa(tp_tipo_casa tipo) {
    // Retorna valores predefinidos para bonus/penalidade.
    if (tipo == 2) {
        return 2;
    }
    if (tipo == 3) {
        return 2;
    }
    return 0;
}

// Cria e encadeia uma nova casa no fim do tabuleiro.
int adicionar_casa(tp_tabuleiro *tabuleiro_referencia, int numero) {
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
    nova->proxima_casa = NULL;
    nova->casa_anterior = tabuleiro_referencia->fim;

    // Encadeia no fim da lista.
    if (tabuleiro_referencia->fim != NULL) {
        tabuleiro_referencia->fim->proxima_casa = nova;
    } else {
        tabuleiro_referencia->inicio = nova;
    }

    tabuleiro_referencia->fim = nova;
    tabuleiro_referencia->total++;
    return 1;
}

// Libera toda memoria do tabuleiro.
void destruir_tabuleiro(tp_tabuleiro *tabuleiro_referencia);

// Cria o tabuleiro completo com 30 casas.
int criar_tabuleiro(tp_tabuleiro *tabuleiro_referencia) {
    int i;

    // Comeca com estrutura vazia.
    inicializar_tabuleiro(tabuleiro_referencia);

    // Cria todas as casas em ordem.
    for (i = 1; i <= 30; i++) {
        if (!adicionar_casa(tabuleiro_referencia, i)) {
            // Em caso de falha, libera recursos parciais.
            destruir_tabuleiro(tabuleiro_referencia);
            return 0;
        }
    }

    return 1;
}

// Libera as casas e reinicializa o tabuleiro.
void destruir_tabuleiro(tp_tabuleiro *tabuleiro_referencia) {
    tp_casa *atual = tabuleiro_referencia->inicio;
    tp_casa *proxima_temporaria;

    // Libera casa por casa.
    while (atual != NULL) {
        proxima_temporaria = atual->proxima_casa;
        free(atual);
        atual = proxima_temporaria;
    }

    // Reinicializa ponteiros.
    inicializar_tabuleiro(tabuleiro_referencia);
}

// Busca ponteiro da casa pela posicao.
tp_casa *obter_casa(tp_tabuleiro *tabuleiro_referencia, int posicao_alvo) {
    tp_casa *atual;
    int i;

    // Valida faixa e ponteiro.
    if (tabuleiro_referencia == NULL || posicao_alvo < 1 || posicao_alvo > tabuleiro_referencia->total) {
        return NULL;
    }

    // Percorre a lista ate a posicao solicitada.
    atual = tabuleiro_referencia->inicio;
    for (i = 1; i < posicao_alvo && atual != NULL; i++) {
        atual = atual->proxima_casa;
    }

    return atual;
}

// Move para frente/tras respeitando os limites do tabuleiro.
int mover_posicao(tp_tabuleiro *tabuleiro_referencia, int posicao_atual, int passos) {
    tp_casa *casa;
    int restante;

    // Caso o tabuleiro esteja vazio, retorna a posicao atual.
    if (tabuleiro_referencia == NULL || tabuleiro_referencia->inicio == NULL) {
        return posicao_atual;
    }

    // Busca a casa atual.
    casa = obter_casa(tabuleiro_referencia, posicao_atual);
    if (casa == NULL) {
        return posicao_atual;
    }

    // Move passo a passo respeitando os limites.
    restante = passos;

    // Avanca pelo tabuleiro.
    while (restante > 0 && casa->proxima_casa != NULL) {
        casa = casa->proxima_casa;
        restante--;
    }

    // Recua pelo tabuleiro.
    while (restante < 0 && casa->casa_anterior != NULL) {
        casa = casa->casa_anterior;
        restante++;
    }

    return casa->numero;
}

#endif
