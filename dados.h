#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>
#include <time.h>

#include "pilha.h"
#include "fila.h"

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

int criar_semente_aleatoria() {
    int marcador_local = 0;
    int semente;
    long endereco_local;

    semente = (int)time(NULL);
    endereco_local = (long)&marcador_local;
    semente = semente ^ (int)endereco_local;

    if (semente == 0) {
        semente = 1u;
    }

    return semente;
}

int numero_aleatorio(int limite, int *estado) {
    long long proximo;

    if (limite <= 0 || estado == NULL) {
        return 0;
    }

    proximo = ((long long)(*estado) * 1103515245LL) + 12345LL;
    if (proximo < 0) {
        proximo = -proximo;
    }

    *estado = (int)(proximo % 2147483647LL);
    return *estado % limite;
}

void copiar_texto(char destino[], int tamanho, char origem[]) {
    int i = 0;

    if (tamanho <= 0) {
        return;
    }

    while (i < tamanho - 1 && origem[i] != '\0') {
        destino[i] = origem[i];
        i++;
    }

    destino[i] = '\0';
}

void inteiro_para_texto(int valor, char texto[]) {
    char invertido[12];
    int i = 0;
    int j;

    if (valor == 0) {
        texto[0] = '0';
        texto[1] = '\0';
        return;
    }

    while (valor > 0 && i < 11) {
        invertido[i] = (char)('0' + (valor % 10));
        valor /= 10;
        i++;
    }

    for (j = 0; j < i; j++) {
        texto[j] = invertido[i - 1 - j];
    }

    texto[i] = '\0';
}

void montar_nome_jogador(char nome_saida[], int tamanho, int indice) {
    char prefixo[] = "Jogador ";
    char numero[12];
    int i = 0;
    int j = 0;

    if (tamanho <= 0) {
        return;
    }

    inteiro_para_texto(indice, numero);

    while (i < tamanho - 1 && prefixo[i] != '\0') {
        nome_saida[i] = prefixo[i];
        i++;
    }

    while (i < tamanho - 1 && numero[j] != '\0') {
        nome_saida[i] = numero[j];
        i++;
        j++;
    }

    nome_saida[i] = '\0';
}

tp_carta criar_carta(
    char perg[],
    char alt1[],
    char alt2[],
    char alt3[],
    char alt4[],
    char alt5[],
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
        montar_nome_jogador(jogadores[i].nome, sizeof(jogadores[i].nome), i + 1);
        jogadores[i].id_player = i + 1;
    }
}

void colocar_jogadores_na_fila(tp_player jogadores[], int quantidade, tp_fila *fila_turnos) {
    int i;

    inicializa_fila(fila_turnos);

    for (i = 0; i < quantidade; i++) {
        insere_fila(fila_turnos, jogadores[i].id_player);
    }
}

void carregar_perguntas(tp_pilha *pilha_cartas) {
    int i, j;
    int estado;
    tp_item ids[NUM_CARTAS];
    tp_item aux;

    estado = criar_semente_aleatoria();

    inicializa_pilha(pilha_cartas);

    for (i = 0; i < NUM_CARTAS; i++) {
        ids[i] = i;
    }

    for (i = NUM_CARTAS - 1; i > 0; i--) {
        j = numero_aleatorio(i + 1, &estado);
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

int sortear_carta(tp_pilha *pilha_cartas, tp_carta *carta_sorteada) {
    tp_carta banco_cartas[NUM_CARTAS];
    tp_item id_carta;

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

    *carta_sorteada = banco_cartas[id_carta];
    return 1;
}

#endif
