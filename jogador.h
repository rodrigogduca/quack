// modelos_jogadores.h
// Declaracoes dos modelos de dados do jogo: cartas (perguntas) e jogadores.
// Define as estruturas e funcoes para gerenciamento do banco de 36 perguntas,
// sorteio de cartas por unidade e dificuldade, e inicializacao dos jogadores.

#ifndef MODELOS_JOGADORES_H
#define MODELOS_JOGADORES_H

#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"
#include "fila.h"

// Estrutura de uma carta-pergunta do jogo
typedef struct {
    char pergunta[400];             // Enunciado da pergunta
    char alternativas[5][200];      // 5 alternativas de resposta
    int resposta;                   // Indice da alternativa correta (1 a 5)
    int avanco;                     // Quantas casas avancar ao acertar (1 ou 2)
    int dificuldade;                // 1=Facil, 2=Medio, 3=Dificil
    int unidade;                    // Unidade do conteudo (1, 2 ou 3)
    int id_carta;                   // Identificador unico da carta
    char tema[50];                  // Tema da pergunta (ex: "Ponteiros")
    char subtema[50];               // Subtema (ex: "NULL")
} tp_carta;

// Estrutura de um jogador
typedef struct {
    int posicao;                    // Casa atual (1 a 30)
    char nome[50];                  // Nome do jogador
    int id_player;                  // Identificador unico
    int historico_casas[100];       // Registro das casas visitadas (ate 100)
    int total_casas_visitadas;      // Quantidade de casas visitadas ate agora
} tp_player;

// Retorna um valor aleatorio entre 1 e 6 (dado de 6 faces)
int rolar_dado(void);

// Retorna 1, 2 ou 3 aleatoriamente (nivel de dificuldade)
int sortear_dificuldade(void);

// Copia uma string de origem para destino com limite de tamanho
void copiar_texto(char destino[], int tamanho, char origem[]);

// Converte um valor inteiro para string (ex: 42 -> "42")
void inteiro_para_texto(int valor, char texto[]);

// Monta o nome padrao do jogador: "Jogador 1", "Jogador 2", etc.
void montar_nome_jogador(char nome_saida[], int tamanho, int indice);

// Preenche o vetor de 36 cartas com o banco de perguntas pre-definido
void montar_banco_cartas(tp_carta cartas[36]);

// Inicializa os jogadores com posicao inicial 1 e nomes padrao
void inicializar_jogadores(tp_player jogadores[], int quantidade);

// Registra a passagem do jogador por uma nova casa (historico)
void registrar_historico_posicao(tp_player *jogador, int posicao);

// Insere os IDs dos jogadores na fila de turnos (ordem de jogo)
void colocar_jogadores_na_fila(tp_player jogadores[4], int quantidade, tp_fila *fila_turnos);

// Embaralha um vetor de IDs usando o algoritmo Fisher-Yates
void embaralhar_ids(tp_item indice_cartas[3], int total_elementos);

// Coleta os indices das cartas que correspondem a uma unidade e dificuldade especificas
int coletar_ids_unidade_dificuldade(tp_carta banco[36], int total, int unidade, int dificuldade, tp_item indice_cartas[3]);

// Recarrega uma pilha com as cartas de determinada unidade e dificuldade (embaralhadas)
void recarregar_pilha_unidade(tp_pilha *pilha, tp_carta banco[36], int total, int unidade, int dificuldade);

// Carrega todas as 9 pilhas (3 unidades x 3 dificuldades) com as cartas do banco
void carregar_perguntas(tp_pilha pilhas[3][3], tp_carta banco[36], int total);

// Sorteia uma carta de uma pilha especifica (unidade x dificuldade).
// Se a pilha estiver vazia, recarrega automaticamente. Retorna 1 se sucesso, 0 se erro.
int sortear_carta(tp_pilha pilhas[3][3], tp_carta banco[36], int total, int unidade, int dificuldade, tp_carta *carta_sorteada);

#endif
