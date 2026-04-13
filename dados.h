#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "pilha.h"

#define NUM_ALTERNATIVAS 5
#define NUM_CARTAS 12

typedef struct {
    char pergunta[300];
    char alternativas[NUM_ALTERNATIVAS][150];
    int resposta;
    int avanco;
    int dificuldade;
    int id_carta;
} tp_carta;

typedef struct {
    int posicao;
    char nome[50];
    int id_player;
} tp_player;

void inicializar_aleatorio() {
    unsigned int seed = (unsigned int)(uintptr_t)&seed;
    srand(seed);
}

static void copiar_texto(char *destino, size_t tamanho, const char *origem) {
    strncpy(destino, origem, tamanho - 1);
    destino[tamanho - 1] = '\0';
}

tp_carta criar_carta(
    const char *perg,
    const char *alt1,
    const char *alt2,
    const char *alt3,
    const char *alt4,
    const char *alt5,
    int resp,
    int avanc,
    int dif,
    int id
) {
    tp_carta c;

    copiar_texto(c.pergunta, sizeof(c.pergunta), perg);
    copiar_texto(c.alternativas[0], sizeof(c.alternativas[0]), alt1);
    copiar_texto(c.alternativas[1], sizeof(c.alternativas[1]), alt2);
    copiar_texto(c.alternativas[2], sizeof(c.alternativas[2]), alt3);
    copiar_texto(c.alternativas[3], sizeof(c.alternativas[3]), alt4);
    copiar_texto(c.alternativas[4], sizeof(c.alternativas[4]), alt5);

    c.resposta = resp;
    c.avanco = avanc;
    c.dificuldade = dif;
    c.id_carta = id;

    return c;
}

void montar_banco_cartas(tp_carta cartas[]) {
    cartas[0] = criar_carta(
        "Sobre a estrutura struct em C, e correto afirmar que:",
        "Permite armazenar apenas dados do mesmo tipo.",
        "E uma estrutura de dados homogenea.",
        "Permite agrupar variaveis de tipos diferentes sob um mesmo nome.",
        "Nao pode conter vetores em seu interior.",
        "E declarada obrigatoriamente dentro da funcao.",
        3, 1, 1, 1
    );

    cartas[1] = criar_carta(
        "Qual a principal caracteristica de um vetor (array) na linguagem C?",
        "Seus elementos sao espalhados aleatoriamente na memoria.",
        "O tamanho pode ser alterado dinamicamente sem comandos especiais.",
        "E uma estrutura heterogenea.",
        "Ocupa um bloco de memoria contiguo e possui tamanho fixo.",
        "O primeiro elemento de um vetor v[10] e acessado pelo indice v[1].",
        4, 1, 1, 2
    );

    cartas[2] = criar_carta(
        "Qual estrutura de repeticao garante execucao pelo menos uma vez?",
        "while",
        "for",
        "switch case",
        "if",
        "do while",
        5, 1, 1, 3
    );

    cartas[3] = criar_carta(
        "O que acontece ao acessar indice 10 de um vetor int vet[10]?",
        "O programa acessa o ultimo elemento do vetor.",
        "Acessa posicao de memoria fora do limite (lixo ou erro).",
        "O compilador corrige automaticamente para o indice 9.",
        "O valor retornado sera sempre 0.",
        "O vetor aumenta de tamanho automaticamente.",
        2, 1, 1, 4
    );

    cartas[4] = criar_carta(
        "Na FILA (Queue), qual a regra de insercao e remocao?",
        "LIFO - O ultimo a entrar e o primeiro a sair.",
        "FIFO - O primeiro a entrar e o primeiro a sair.",
        "O usuario escolhe qualquer elemento para remover.",
        "A remocao acontece no fim e a insercao no inicio.",
        "Nao existe ordem especifica para saida.",
        2, 2, 2, 5
    );

    cartas[5] = criar_carta(
        "Na PILHA (Stack), o termo POP refere-se a:",
        "Inserir um elemento no topo.",
        "Limpar todos os elementos da memoria.",
        "Remover o elemento que esta no topo.",
        "Consultar o elemento da base sem remove-lo.",
        "Inverter a ordem dos elementos.",
        3, 2, 2, 6
    );

    cartas[6] = criar_carta(
        "Qual o resultado da expressao (15 %% 4)?",
        "3.75",
        "3",
        "1",
        "0",
        "4",
        2, 2, 2, 7
    );

    cartas[7] = criar_carta(
        "Pilha [10,20,30] (30 no topo): apos PUSH(40) e POP(), qual o topo?",
        "40",
        "10",
        "20",
        "30",
        "A pilha ficara vazia.",
        4, 2, 2, 8
    );

    cartas[8] = criar_carta(
        "Fila em vetor de 5 posicoes, apos inserir 5: onde aponta 'Fim'?",
        "Aponta para a posicao 0.",
        "Aponta para a posicao 4.",
        "Aponta para a posicao 5 (fora do vetor).",
        "Retorna ao valor nulo automaticamente.",
        "E resetado para o valor do ponteiro de Inicio.",
        2, 3, 3, 9
    );

    cartas[9] = criar_carta(
        "Qual a principal diferenca logica entre Pilha e Fila?",
        "A Pilha usa struct e a Fila nao.",
        "A Fila e estatica e a Pilha e sempre dinamica.",
        "Na Pilha sai o mais recente; na Fila sai o mais antigo.",
        "Na Pilha os dados sao ordenados, na Fila nao.",
        "Nao ha diferenca, sao apenas nomes diferentes.",
        3, 3, 3, 10
    );

    cartas[10] = criar_carta(
        "Busca em vetor desordenado com for: qual o pior cenario?",
        "O elemento estar na primeira posicao.",
        "O elemento estar no meio do vetor.",
        "O elemento nao existir ou estar na ultima posicao.",
        "O vetor estar vazio.",
        "O compilador encontrar um erro de sintaxe.",
        3, 3, 3, 11
    );

    cartas[11] = criar_carta(
        "Ao passar struct para funcao em C, o que acontece por padrao?",
        "A funcao altera diretamente os dados da struct original.",
        "Uma copia de toda a struct e enviada (passagem por valor).",
        "Apenas o primeiro campo da struct e enviado.",
        "O programa trava, pois structs nao podem ser passadas.",
        "A struct e convertida automaticamente em vetor de inteiros.",
        2, 3, 3, 12
    );
}

void inicializar_jogadores(tp_player jogadores[], int quantidade) {
    int i;

    for (i = 0; i < quantidade; i++) {
        jogadores[i].posicao = 1;
        snprintf(jogadores[i].nome, sizeof(jogadores[i].nome), "Player %d", i + 1);
        jogadores[i].id_player = i + 1;
    }
}

void carregar_perguntas(tp_pilha *pilha_cartas) {
    int i, j;
    tp_item ids[NUM_CARTAS];
    tp_item aux;

    inicializa_pilha(pilha_cartas);

    for (i = 0; i < NUM_CARTAS; i++) {
        ids[i] = i;
    }

    for (i = NUM_CARTAS - 1; i > 0; i--) {
        j = rand() % (i + 1);
        aux = ids[i];
        ids[i] = ids[j];
        ids[j] = aux;
    }

    for (i = 0; i < NUM_CARTAS; i++) {
        push(pilha_cartas, ids[i]);
    }
}

int tamanho_pilha_cartas(tp_pilha *pilha_cartas) {
    return altura_pilha(pilha_cartas);
}

int fazer_pergunta(tp_pilha *pilha_cartas) {
    tp_carta banco_cartas[NUM_CARTAS];
    tp_item id_carta;
    tp_carta carta;
    int i, resposta;

    montar_banco_cartas(banco_cartas);

    if (pilha_vazia(pilha_cartas)) {
        printf("\nTodas as perguntas foram respondidas! Recarregando...\n");
        carregar_perguntas(pilha_cartas);
    }

    if (!pop(pilha_cartas, &id_carta)) {
        return 0;
    }

    if (id_carta < 0 || id_carta >= NUM_CARTAS) {
        return 0;
    }

    carta = banco_cartas[id_carta];

    printf("\n========================================\n");
    printf("   CARTA SORTEADA! (Dificuldade %d)     \n", carta.dificuldade);
    printf("========================================\n");
    printf("\n%s\n\n", carta.pergunta);

    for (i = 0; i < NUM_ALTERNATIVAS; i++) {
        printf("%d) %s\n", i + 1, carta.alternativas[i]);
    }

    printf("\nDigite o numero da sua resposta: ");
    if (scanf("%d", &resposta) != 1) {
        resposta = -1;
    }

    while ((i = getchar()) != '\n' && i != EOF) {
        /* limpa buffer de entrada */
    }

    printf("\nRESPOSTA...\n");

    if (resposta == carta.resposta) {
        printf("\n*** CORRETA! ***\n");
        printf("Voce avanca %d casa(s)!\n", carta.avanco);
        return carta.avanco;
    }

    printf("\n*** ERRADA! ***\n");
    printf(
        "A resposta correta era: %d) %s\n",
        carta.resposta,
        carta.alternativas[carta.resposta - 1]
    );
    return 0;
}

#endif
