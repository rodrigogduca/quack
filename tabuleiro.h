// tabuleiro.h
// Declaracoes do Tabuleiro do jogo, implementado como Lista Duplamente Encadeada.
// Cada casa possui tipo, efeito e referencias para a casa anterior e seguinte.
// O tabuleiro tem 30 casas divididas em 3 unidades de 10 casas cada.

#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdlib.h>

// Tipos de casa no tabuleiro:
//   0 = normal (sem efeito especial)
//   1 = pergunta (jogador responde uma questao)
//   2 = bonus (avanca casas)
//   3 = penalidade (volta casas)
typedef int tp_tipo_casa;

// Estrutura de uma casa do tabuleiro (no da lista duplamente encadeada)
typedef struct tp_casa {
    int numero;                        // Numero da casa (1 a 30)
    int unidade;                       // Unidade a que pertence (1 a 3)
    tp_tipo_casa tipo;                 // Tipo da casa (0=normal, 1=pergunta, 2=bonus, 3=penalidade)
    int efeito;                        // Quantidade de casas a avancar/voltar
    struct tp_casa *proxima_casa;      // Ponteiro para a proxima casa (ou NULL se ultima)
    struct tp_casa *casa_anterior;     // Ponteiro para a casa anterior (ou NULL se primeira)
} tp_casa;

// Estrutura do tabuleiro (cabeca da lista)
typedef struct {
    tp_casa *inicio;   // Ponteiro para a primeira casa (numero 1)
    tp_casa *fim;      // Ponteiro para a ultima casa (numero 30)
    int total;         // Total de casas (sempre 30)
} tp_tabuleiro;

// Inicializa o tabuleiro com ponteiros nulos e total = 0
void inicializar_tabuleiro(tp_tabuleiro *tabuleiro_referencia);

// Retorna a unidade (1, 2 ou 3) dado o numero da casa
int unidade_por_posicao(int posicao_alvo);

// Retorna o tipo da casa (0, 1, 2 ou 3) baseado no padrao das casas
tp_tipo_casa tipo_casa_por_posicao(int posicao_alvo);

// Retorna o efeito (quantidade de casas) para um dado tipo de casa
int efeito_casa(tp_tipo_casa tipo);

// Adiciona uma nova casa ao final do tabuleiro. Retorna 1 se sucesso, 0 se erro de alocacao.
int adicionar_casa(tp_tabuleiro *tabuleiro_referencia, int numero);

// Cria as 30 casas do tabuleiro. Retorna 1 se sucesso, 0 se erro.
int criar_tabuleiro(tp_tabuleiro *tabuleiro_referencia);

// Libera a memoria de todas as casas e reinicializa o tabuleiro
void destruir_tabuleiro(tp_tabuleiro *tabuleiro_referencia);

// Retorna o ponteiro para a casa em determinada posicao (1-indexada), ou NULL se invalida
tp_casa *obter_casa(tp_tabuleiro *tabuleiro_referencia, int posicao_alvo);

// Move uma quantidade de passos (positivo = avanca, negativo = volta) a partir da posicao atual.
// Retorna o novo numero da casa.
int mover_posicao(tp_tabuleiro *tabuleiro_referencia, int posicao_atual, int passos);

#endif
