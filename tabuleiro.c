// tabuleiro.c
// Implementacao do Tabuleiro como Lista Duplamente Encadeada.
// Logica de criacao das 30 casas, tipos (pergunta/bonus/penalidade) e movimento dos jogadores.

#include "tabuleiro.h"

// Inicializa os ponteiros como NULL e total como 0
void inicializar_tabuleiro(tp_tabuleiro *tabuleiro_referencia) {
    tabuleiro_referencia->inicio = NULL;
    tabuleiro_referencia->fim = NULL;
    tabuleiro_referencia->total = 0;
}

// Retorna a unidade (1, 2 ou 3). Cada unidade tem 10 casas.
int unidade_por_posicao(int posicao_alvo) {
    if (posicao_alvo <= 0) {
        return 1;
    }
    return ((posicao_alvo - 1) / 10) + 1;
}

// Determina o tipo da casa baseado no deslocamento dentro da unidade (1 a 10):
//   Posicoes 2, 5, 8 = pergunta (tipo 1)
//   Posicao 3        = bonus (tipo 2)
//   Posicao 7        = penalidade (tipo 3)
//   Demais           = normal (tipo 0)
tp_tipo_casa tipo_casa_por_posicao(int posicao_alvo) {
    int deslocamento;
    if (posicao_alvo <= 0) {
        return 0;
    }
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

// Define o efeito de cada tipo de casa especial:
//   tipo 2 (bonus)       = avanca 2 casas
//   tipo 3 (penalidade)  = volta 2 casas
//   tipo 0 ou 1 (normal/pergunta) = sem efeito
int efeito_casa(tp_tipo_casa tipo) {
    if (tipo == 2) {
        return 2;
    }
    if (tipo == 3) {
        return 2;
    }
    return 0;
}

// Cria e adiciona uma nova casa ao final da lista duplamente encadeada.
// Configura numero, unidade, tipo e efeito automaticamente.
int adicionar_casa(tp_tabuleiro *tabuleiro_referencia, int numero) {
    tp_casa *nova = (tp_casa *)malloc(sizeof(tp_casa));
    if (nova == NULL) {
        return 0;
    }
    nova->numero = numero;
    nova->unidade = unidade_por_posicao(numero);
    nova->tipo = tipo_casa_por_posicao(numero);
    nova->efeito = efeito_casa(nova->tipo);
    nova->proxima_casa = NULL;
    nova->casa_anterior = tabuleiro_referencia->fim;

    // Se ja existe pelo menos uma casa, encadeia; senao, define como inicio
    if (tabuleiro_referencia->fim != NULL) {
        tabuleiro_referencia->fim->proxima_casa = nova;
    } else {
        tabuleiro_referencia->inicio = nova;
    }
    tabuleiro_referencia->fim = nova;
    tabuleiro_referencia->total++;
    return 1;
}

// Cria as 30 casas do tabuleiro iterando de 1 a 30.
// Se alguma alocacao falhar, libera toda a memoria ja alocada.
int criar_tabuleiro(tp_tabuleiro *tabuleiro_referencia) {
    int i;
    inicializar_tabuleiro(tabuleiro_referencia);
    for (i = 1; i <= 30; i++) {
        if (!adicionar_casa(tabuleiro_referencia, i)) {
            destruir_tabuleiro(tabuleiro_referencia);
            return 0;
        }
    }
    return 1;
}

// Percorre a lista a partir do inicio, liberando cada no com free().
// Ao final, reinicializa a estrutura do tabuleiro.
void destruir_tabuleiro(tp_tabuleiro *tabuleiro_referencia) {
    tp_casa *atual = tabuleiro_referencia->inicio;
    tp_casa *proxima_temporaria;
    while (atual != NULL) {
        proxima_temporaria = atual->proxima_casa;
        free(atual);
        atual = proxima_temporaria;
    }
    inicializar_tabuleiro(tabuleiro_referencia);
}

// Navega pela lista a partir de 'inicio' ate encontrar a posicao desejada.
// A posicao e 1-indexada (1 = primeira casa).
tp_casa *obter_casa(tp_tabuleiro *tabuleiro_referencia, int posicao_alvo) {
    tp_casa *atual;
    int i;
    if (tabuleiro_referencia == NULL || posicao_alvo < 1 || posicao_alvo > tabuleiro_referencia->total) {
        return NULL;
    }
    atual = tabuleiro_referencia->inicio;
    for (i = 1; i < posicao_alvo && atual != NULL; i++) {
        atual = atual->proxima_casa;
    }
    return atual;
}

// Move o jogador 'passos' casas a partir de 'posicao_atual'.
// passos positivo = avanca (proxima_casa), passos negativo = volta (casa_anterior).
// Nao ultrapassa os limites do tabuleiro.
int mover_posicao(tp_tabuleiro *tabuleiro_referencia, int posicao_atual, int passos) {
    tp_casa *casa;
    int restante;
    if (tabuleiro_referencia == NULL || tabuleiro_referencia->inicio == NULL) {
        return posicao_atual;
    }
    casa = obter_casa(tabuleiro_referencia, posicao_atual);
    if (casa == NULL) {
        return posicao_atual;
    }
    restante = passos;
    // Avanca enquanto houver casas a percorrer e proxima casa existir
    while (restante > 0 && casa->proxima_casa != NULL) {
        casa = casa->proxima_casa;
        restante--;
    }
    // Volta enquanto houver casas a percorrer e casa anterior existir
    while (restante < 0 && casa->casa_anterior != NULL) {
        casa = casa->casa_anterior;
        restante++;
    }
    return casa->numero;
}
