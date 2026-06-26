// historico.h
// Declaracoes do sistema de historico de respostas usando Arvore Binaria de Busca (BST).
// Permite registrar respostas dos jogadores, buscar por ID da carta e
// exportar relatorios em formato CSV (historico_respostas.csv, perguntas.csv).

#ifndef HISTORICO_H
#define HISTORICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jogador.h"

// Estrutura que armazena um registro de resposta de um jogador
typedef struct {
    char turma[50];          // Turma do jogador (ex: "AED-2026.1-T")
    char nome_jogador[50];   // Nome do jogador
    int id_carta;            // ID da carta respondida
    int unidade;             // Unidade da pergunta (1, 2 ou 3)
    char tema[50];           // Tema da pergunta
    char subtema[50];        // Subtema da pergunta
    int dificuldade;         // Dificuldade (1=Facil, 2=Medio, 3=Dificil)
    int resposta_jogador;    // Alternativa escolhida pelo jogador (1 a 5)
    int resposta_correta;    // Alternativa correta (1 a 5)
    int acertou;             // 1 se acertou, 0 se errou
} tp_registro_estatistica;

// No da Arvore Binaria de Busca, ordenado por (id_carta, nome, resposta_jogador)
typedef struct tp_no_estatistica {
    tp_registro_estatistica registro;              // Dados armazenados no no
    struct tp_no_estatistica *esquerda;            // Subarvore esquerda (menores)
    struct tp_no_estatistica *direita;             // Subarvore direita (maiores)
} tp_no_estatistica;

// Estrutura da arvore (raiz + contador total)
typedef struct {
    tp_no_estatistica *raiz;  // Ponteiro para a raiz da arvore
    int total;               // Quantidade total de nos inseridos
} tp_arvore_estatisticas;

// Inicializa a arvore com raiz = NULL e total = 0
void inicializar_arvore_estatisticas(tp_arvore_estatisticas *arvore);

// Converte o valor numérico de dificuldade para string: 1->"Fácil", 2->"Médio", 3->"Difícil"
char *texto_dificuldade(int dificuldade);

// Converte o numero da resposta (1-5) para letra (A-E)
char letra_resposta(int resposta);

// Cria um registro de estatistica com todos os campos preenchidos
tp_registro_estatistica criar_registro_estatistica(char turma[], char nome_jogador[], int id_carta, int unidade, char tema[], char subtema[], int dificuldade, int resposta_jogador, int resposta_correta, int acertou);

// Aloca e inicializa um no da arvore com o registro fornecido
tp_no_estatistica *criar_no_estatistica(tp_registro_estatistica registro);

// Compara dois registros: primeiro por id_carta, depois por nome, depois por resposta_jogador.
// Retorna -1, 0 ou 1 (padrao de funcao de comparacao).
int comparar_registros_estatistica(tp_registro_estatistica *primeiro, tp_registro_estatistica *segundo);

// Insere recursivamente um registro na arvore (sem balanceamento).
// Define *inseriu = 1 se conseguiu inserir, 0 se ja existia.
tp_no_estatistica *inserir_no_estatistica(tp_no_estatistica *raiz, tp_registro_estatistica registro, int *inseriu);

// Insere um registro na arvore (interface publica).
// Incrementa contador total. Retorna 1 se inseriu, 0 se ja existia.
int inserir_estatistica(tp_arvore_estatisticas *arvore, tp_registro_estatistica registro);

// Busca um registro na arvore pelo id_carta. Retorna o no ou NULL se nao encontrado.
tp_no_estatistica *buscar_estatistica_por_carta(tp_no_estatistica *raiz, int id_carta);

// Escreve uma linha CSV com os dados do registro no arquivo
void escrever_registro_estatistica_csv(FILE *arquivo, tp_registro_estatistica *registro);

// Percorre a arvore em ordem (in-order) e escreve cada registro no CSV
void exportar_arvore_estatisticas_em_ordem(FILE *arquivo, tp_no_estatistica *raiz);

// Libera a memoria de todos os nos da arvore (pos-ordem)
void destruir_arvore_estatisticas(tp_no_estatistica *raiz);

// Escreve o cabecalho do CSV se o arquivo estiver vazio
void escrever_cabecalho_historico(FILE *arquivo);

// Funcao de alto nivel: cria registro, insere na arvore e escreve no arquivo historico_respostas.csv
void salvar_historico_resposta(char turma[50], char nome_jogador[50], int id_carta, int unidade, char tema[50], char subtema[50], int dificuldade, int resposta_jogador, int resposta_correta, int acertou);

// Exporta o banco de perguntas para perguntas.csv (apenas metadados, sem alternativas)
void exportar_perguntas_csv(tp_carta banco[], int total);

#endif /* HISTORICO_H */
