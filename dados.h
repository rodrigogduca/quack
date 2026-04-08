#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_CARTAS 20
#define NUM_ALTERNATIVAS 5
#define MAX_JOGADORES 4

// Estrutura da carta com alternativas em matriz
typedef struct {
    char pergunta[300];
    char alternativas[5][150];
    int resposta;
    int avanco;
    int dificuldade;
    int id_carta;
} tp_carta;

// Pilha de cartas
typedef struct {
    tp_carta cartas[MAX_CARTAS];
    int topo;
} tp_pilha_cartas;

// Informacoes do jogador
typedef struct {
    int posicao;
    char nome[50];
    int id_player;
} tp_player;

// Inicializa o gerador de numeros aleatorios
void inicializar_aleatorio() {
    srand(time(NULL));
}

// Inicializa pilha de cartas vazia
void inicializa_pilha_cartas(tp_pilha_cartas *p) {
    p->topo = -1;
}

// Verifica se pilha de cartas esta vazia
int pilha_cartas_vazia(tp_pilha_cartas *p) {
    return p->topo == -1;
}

// Verifica se pilha de cartas esta cheia
int pilha_cartas_cheia(tp_pilha_cartas *p) {
    return p->topo == MAX_CARTAS - 1;
}

// Empilha uma carta
int push_carta(tp_pilha_cartas *p, tp_carta carta) {
    if (pilha_cartas_cheia(p)) return 0;
    p->topo++;
    p->cartas[p->topo] = carta;
    return 1;
}

// Desempilha uma carta
int pop_carta(tp_pilha_cartas *p, tp_carta *carta) {
    if (pilha_cartas_vazia(p)) return 0;
    *carta = p->cartas[p->topo];
    p->topo--;
    return 1;
}

// Retorna quantidade de cartas na pilha
int tamanho_pilha_cartas(tp_pilha_cartas *p) {
    return p->topo + 1;
}

// Cria uma carta com os dados fornecidos
tp_carta criar_carta(char *perg, char *alt1, char *alt2, char *alt3, char *alt4, char *alt5,
                     int resp, int avanc, int dif, int id) {
    tp_carta c;
    strcpy(c.pergunta, perg);
    strcpy(c.alternativas[0], alt1);
    strcpy(c.alternativas[1], alt2);
    strcpy(c.alternativas[2], alt3);
    strcpy(c.alternativas[3], alt4);
    strcpy(c.alternativas[4], alt5);
    c.resposta = resp;
    c.avanco = avanc;
    c.dificuldade = dif;
    c.id_carta = id;
    return c;
}

// Carrega todas as perguntas nas pilhas por dificuldade
void carregar_perguntas(tp_pilha_cartas *facil, tp_pilha_cartas *medio, tp_pilha_cartas *dificil) {
    inicializa_pilha_cartas(facil);
    inicializa_pilha_cartas(medio);
    inicializa_pilha_cartas(dificil);
    
    // DIFICULDADE 1 - FACIL (avanca 1 casa)
    push_carta(facil, criar_carta(
        "Sobre a estrutura struct em C, e correto afirmar que:",
        "Permite armazenar apenas dados do mesmo tipo.",
        "E uma estrutura de dados homogenea.",
        "Permite agrupar variaveis de tipos diferentes sob um mesmo nome.",
        "Nao pode conter vetores em seu interior.",
        "E declarada obrigatoriamente dentro da funcao.",
        3, 1, 1, 1
    ));
    
    push_carta(facil, criar_carta(
        "Qual a principal caracteristica de um vetor (array) na linguagem C?",
        "Seus elementos sao espalhados aleatoriamente na memoria.",
        "O tamanho pode ser alterado dinamicamente sem comandos especiais.",
        "E uma estrutura heterogenea.",
        "Ocupa um bloco de memoria contiguo e possui tamanho fixo.",
        "O primeiro elemento de um vetor v[10] e acessado pelo indice v[1].",
        4, 1, 1, 2
    ));
    
    push_carta(facil, criar_carta(
        "Qual estrutura de repeticao garante execucao pelo menos uma vez?",
        "while",
        "for",
        "switch case",
        "if",
        "do while",
        5, 1, 1, 3
    ));
    
    push_carta(facil, criar_carta(
        "O que acontece ao acessar indice 10 de um vetor int vet[10]?",
        "O programa acessa o ultimo elemento do vetor.",
        "Acessa posicao de memoria fora do limite (lixo ou erro).",
        "O compilador corrige automaticamente para o indice 9.",
        "O valor retornado sera sempre 0.",
        "O vetor aumenta de tamanho automaticamente.",
        2, 1, 1, 4
    ));
    
    // DIFICULDADE 2 - MEDIO (avanca 2 casas)
    push_carta(medio, criar_carta(
        "Na FILA (Queue), qual a regra de insercao e remocao?",
        "LIFO - O ultimo a entrar e o primeiro a sair.",
        "FIFO - O primeiro a entrar e o primeiro a sair.",
        "O usuario escolhe qualquer elemento para remover.",
        "A remocao acontece no fim e a insercao no inicio.",
        "Nao existe ordem especifica para saida.",
        2, 2, 2, 5
    ));
    
    push_carta(medio, criar_carta(
        "Na PILHA (Stack), o termo POP refere-se a:",
        "Inserir um elemento no topo.",
        "Limpar todos os elementos da memoria.",
        "Remover o elemento que esta no topo.",
        "Consultar o elemento da base sem remove-lo.",
        "Inverter a ordem dos elementos.",
        3, 2, 2, 6
    ));
    
    push_carta(medio, criar_carta(
        "Qual o resultado da expressao (15 %% 4)?",
        "3.75",
        "3",
        "1",
        "0",
        "4",
        2, 2, 2, 7
    ));
    
    push_carta(medio, criar_carta(
        "Pilha [10,20,30] (30 no topo): apos PUSH(40) e POP(), qual o topo?",
        "40",
        "10",
        "20",
        "30",
        "A pilha ficara vazia.",
        4, 2, 2, 8
    ));
    
    // DIFICULDADE 3 - DIFICIL (avanca 3 casas)
    push_carta(dificil, criar_carta(
        "Fila em vetor de 5 posicoes, apos inserir 5: onde aponta 'Fim'?",
        "Aponta para a posicao 0.",
        "Aponta para a posicao 4.",
        "Aponta para a posicao 5 (fora do vetor).",
        "Retorna ao valor nulo automaticamente.",
        "E resetado para o valor do ponteiro de Inicio.",
        2, 3, 3, 9
    ));
    
    push_carta(dificil, criar_carta(
        "Qual a principal diferenca logica entre Pilha e Fila?",
        "A Pilha usa struct e a Fila nao.",
        "A Fila e estatica e a Pilha e sempre dinamica.",
        "Na Pilha sai o mais recente; na Fila sai o mais antigo.",
        "Na Pilha os dados sao ordenados, na Fila nao.",
        "Nao ha diferenca, sao apenas nomes diferentes.",
        3, 3, 3, 10
    ));
    
    push_carta(dificil, criar_carta(
        "Busca em vetor desordenado com for: qual o pior cenario?",
        "O elemento estar na primeira posicao.",
        "O elemento estar no meio do vetor.",
        "O elemento nao existir ou estar na ultima posicao.",
        "O vetor estar vazio.",
        "O compilador encontrar um erro de sintaxe.",
        3, 3, 3, 11
    ));
    
    push_carta(dificil, criar_carta(
        "Ao passar struct para funcao em C, o que acontece por padrao?",
        "A funcao altera diretamente os dados da struct original.",
        "Uma copia de toda a struct e enviada (passagem por valor).",
        "Apenas o primeiro campo da struct e enviado.",
        "O programa trava, pois structs nao podem ser passadas.",
        "A struct e convertida automaticamente em vetor de inteiros.",
        2, 3, 3, 12
    ));
}

// Inicializa array de jogadores
void inicializar_jogadores(tp_player jogadores[], int quantidade) {
    int i;
    for (i = 0; i < quantidade; i++) {
        jogadores[i].posicao = 1;
        sprintf(jogadores[i].nome, "Player %d", i + 1);
        jogadores[i].id_player = i + 1;
    }
}

// Embaralha uma pilha de cartas
void embaralhar_pilha(tp_pilha_cartas *p) {
    int n, i, j;
    tp_carta temp[MAX_CARTAS];
    tp_carta aux;
    
    if (pilha_cartas_vazia(p)) return;
    
    n = tamanho_pilha_cartas(p);
    
    for (i = 0; i < n; i++) {
        pop_carta(p, &temp[i]);
    }
    
    // Embaralha (Fisher-Yates)
    for (i = n - 1; i > 0; i--) {
        j = rand() % (i + 1);
        aux = temp[i];
        temp[i] = temp[j];
        temp[j] = aux;
    }
    
    for (i = 0; i < n; i++) {
        push_carta(p, temp[i]);
    }
}

// Faz uma pergunta e retorna avanco (0 se errar)
int fazer_pergunta(tp_pilha_cartas *facil, tp_pilha_cartas *medio, tp_pilha_cartas *dificil) {
    int dif, i, resposta;
    tp_pilha_cartas *pilha_escolhida;
    tp_carta carta;
    
    dif = (rand() % 3) + 1;
    
    if (dif == 1 && !pilha_cartas_vazia(facil)) {
        pilha_escolhida = facil;
    } else if (dif == 2 && !pilha_cartas_vazia(medio)) {
        pilha_escolhida = medio;
    } else if (dif == 3 && !pilha_cartas_vazia(dificil)) {
        pilha_escolhida = dificil;
    } else {
        if (!pilha_cartas_vazia(facil)) pilha_escolhida = facil;
        else if (!pilha_cartas_vazia(medio)) pilha_escolhida = medio;
        else if (!pilha_cartas_vazia(dificil)) pilha_escolhida = dificil;
        else {
            printf("\nTodas as perguntas foram respondidas! Recarregando...\n");
            carregar_perguntas(facil, medio, dificil);
            embaralhar_pilha(facil);
            embaralhar_pilha(medio);
            embaralhar_pilha(dificil);
            pilha_escolhida = facil;
        }
    }
    
    pop_carta(pilha_escolhida, &carta);
    
    printf("\n========================================\n");
    printf("   CARTA SORTEADA! (Dificuldade %d)     \n", carta.dificuldade);
    printf("========================================\n");
    printf("\n%s\n\n", carta.pergunta);
    
    for (i = 0; i < NUM_ALTERNATIVAS; i++) {
        printf("%d) %s\n", i + 1, carta.alternativas[i]);
    }
    
    printf("\nDigite o numero da sua resposta: ");
    scanf("%d", &resposta);
    
    printf("\nRESPOSTA...\n");
    Sleep(1500);
    
    if (resposta == carta.resposta) {
        printf("\n*** CORRETA! ***\n");
        printf("Voce avanca %d casa(s)!\n", carta.avanco);
        return carta.avanco;
    } else {
        printf("\n*** ERRADA! ***\n");
        printf("A resposta correta era: %d) %s\n", carta.resposta, carta.alternativas[carta.resposta - 1]);
        return 0;
    }
}

#endif
