#ifndef CONFIG_H
#define CONFIG_H

// config.h
// ---------------------------------------------------------------------------
// Constantes centrais de configuracao do jogo.

// Numero de alternativas por pergunta.
#define NUM_ALTERNATIVAS 5

// Quantidade de unidades (trechos) no tabuleiro.
#define NUM_UNIDADES 3

// Quantidade de niveis de dificuldade.
#define NUM_DIFICULDADES 3

// Casas por unidade e total de casas do tabuleiro.
#define CASAS_POR_UNIDADE 10
#define TOTAL_CASAS (NUM_UNIDADES * CASAS_POR_UNIDADE)

// Perguntas por unidade (unidades 1 e 2) e total do banco.
#define PERGUNTAS_POR_UNIDADE 12
#define TOTAL_PERGUNTAS (2 * PERGUNTAS_POR_UNIDADE)

// Penalidade ao errar e efeitos das casas especiais.
#define PENALIDADE_ERRO 1
#define AVANCO_CASA_ESPECIAL 2
#define RECUO_CASA_ESPECIAL 2

#endif
