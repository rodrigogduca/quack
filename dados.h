#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>
#include <time.h>

#include "config.h"
#include "pilha.h"
#include "fila.h"

typedef struct {
    char pergunta[300];
    char alternativas[NUM_ALTERNATIVAS][150];
    int resposta;
    int avanco;
    int dificuldade;
    int unidade;
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

int rolar_dado(int *estado) {
    int valor = numero_aleatorio(6, estado);
    return valor + 1;
}

int sortear_dificuldade(int *estado) {
    int valor = numero_aleatorio(NUM_DIFICULDADES, estado);
    return valor + 1;
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
    int unidade,
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
    c.unidade = unidade;
    c.id_carta = id;

    return c;
}

void montar_banco_cartas(tp_carta cartas[]) {
    cartas[0] = criar_carta(
        "Qual operador realiza atribuicao em C?",
        "=",
        "==",
        "!=",
        "<=",
        "++",
        1, 1, 1, 1, 1
    );

    cartas[1] = criar_carta(
        "Qual simbolo inicia um comentario de uma linha em C?",
        "/*",
        "//",
        "#",
        "<!--",
        "--",
        2, 1, 1, 1, 2
    );

    cartas[2] = criar_carta(
        "Qual e o primeiro indice valido de um vetor int v[10]?",
        "1",
        "0",
        "-1",
        "10",
        "Depende do compilador",
        2, 2, 2, 1, 3
    );

    cartas[3] = criar_carta(
        "Qual o resultado da expressao (7 % 3)?",
        "0",
        "1",
        "2",
        "3",
        "7",
        2, 2, 2, 1, 4
    );

    cartas[4] = criar_carta(
        "O que acontece ao acessar v[10] em int v[10]?",
        "Acessa o ultimo elemento",
        "Acesso fora do limite (erro)",
        "O compilador corrige para v[9]",
        "Retorna sempre 0",
        "O vetor aumenta de tamanho",
        2, 3, 3, 1, 5
    );

    cartas[5] = criar_carta(
        "O que pode ocorrer ao usar printf com variavel nao inicializada?",
        "Erro de compilacao",
        "Valor lixo ou comportamento indefinido",
        "Sempre imprime zero",
        "Sempre imprime -1",
        "O programa termina automaticamente",
        2, 3, 3, 1, 6
    );

    cartas[6] = criar_carta(
        "Qual operador acessa campo de struct via ponteiro?",
        ".",
        "->",
        "&",
        "*",
        "::",
        2, 1, 1, 2, 7
    );

    cartas[7] = criar_carta(
        "Qual funcao aloca memoria dinamica em C?",
        "malloc",
        "printf",
        "scanf",
        "sizeof",
        "free",
        1, 1, 1, 2, 8
    );

    cartas[8] = criar_carta(
        "Um ponteiro armazena:",
        "Um valor inteiro fixo",
        "Endereco de memoria",
        "Uma string",
        "Um arquivo aberto",
        "Um vetor de chars",
        2, 2, 2, 2, 9
    );

    cartas[9] = criar_carta(
        "Ao passar um ponteiro para funcao, e possivel:",
        "Alterar o conteudo apontado",
        "Somente ler o valor",
        "Alterar o codigo fonte",
        "Mover o ponteiro para NULL automaticamente",
        "Converter para float",
        1, 2, 2, 2, 10
    );

    cartas[10] = criar_carta(
        "sizeof(int*) retorna:",
        "Tamanho do int",
        "Tamanho do ponteiro",
        "Sempre 4",
        "Sempre 8",
        "Erro de compilacao",
        2, 3, 3, 2, 11
    );

    cartas[11] = criar_carta(
        "Apos usar malloc, deve-se:",
        "Ignorar o retorno",
        "Verificar NULL e usar free quando nao precisar",
        "Sempre usar scanf",
        "Reiniciar o programa",
        "Declarar outra variavel global",
        2, 3, 3, 2, 12
    );

    cartas[12] = criar_carta(
        "Em uma fila, quem sai primeiro?",
        "Quem entrou primeiro (FIFO)",
        "Quem entrou por ultimo (LIFO)",
        "Quem tem maior valor",
        "Quem pediu primeiro",
        "Nao existe ordem",
        1, 1, 1, 3, 13
    );

    cartas[13] = criar_carta(
        "Em uma pilha, qual operacao remove o topo?",
        "push",
        "enqueue",
        "pop",
        "peek",
        "insert",
        3, 1, 1, 3, 14
    );

    cartas[14] = criar_carta(
        "Em uma lista simplesmente encadeada, cada no possui:",
        "Apenas um indice",
        "Valor e ponteiro para o proximo",
        "Somente ponteiro para o anterior",
        "Dois ponteiros e nenhum valor",
        "Um vetor fixo de 10 itens",
        2, 2, 2, 3, 15
    );

    cartas[15] = criar_carta(
        "Qual estrutura e mais adequada para controlar turnos em ordem de chegada?",
        "Fila",
        "Pilha",
        "Arvore",
        "Lista circular sem controle",
        "Tabela hash",
        1, 2, 2, 3, 16
    );

    cartas[16] = criar_carta(
        "Qual o custo medio de busca em uma arvore de busca balanceada?",
        "O(1)",
        "O(log n)",
        "O(n)",
        "O(n^2)",
        "O(n log n)",
        2, 3, 3, 3, 17
    );

    cartas[17] = criar_carta(
        "Em uma pilha, inserir no topo e chamado de:",
        "pop",
        "push",
        "dequeue",
        "peek",
        "shift",
        2, 3, 3, 3, 18
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

void embaralhar_ids(tp_item ids[], int total, int *estado) {
    int i, j;
    tp_item aux;

    for (i = total - 1; i > 0; i--) {
        j = numero_aleatorio(i + 1, estado);
        aux = ids[i];
        ids[i] = ids[j];
        ids[j] = aux;
    }
}

int coletar_ids_unidade_dificuldade(
    tp_carta banco[],
    int total,
    int unidade,
    int dificuldade,
    tp_item ids[]
) {
    int i;
    int quantidade = 0;

    for (i = 0; i < total; i++) {
        if (banco[i].unidade == unidade && banco[i].dificuldade == dificuldade) {
            ids[quantidade] = i;
            quantidade++;
        }
    }

    return quantidade;
}

void recarregar_pilha_unidade(
    tp_pilha *pilha,
    tp_carta banco[],
    int total,
    int unidade,
    int dificuldade,
    int *estado
) {
    tp_item ids[TOTAL_PERGUNTAS];
    int quantidade;
    int i;

    inicializa_pilha(pilha);

    quantidade = coletar_ids_unidade_dificuldade(
        banco,
        total,
        unidade,
        dificuldade,
        ids
    );

    if (quantidade == 0) {
        return;
    }

    embaralhar_ids(ids, quantidade, estado);

    for (i = 0; i < quantidade; i++) {
        push(pilha, ids[i]);
    }
}

void carregar_perguntas(
    tp_pilha pilhas[NUM_UNIDADES][NUM_DIFICULDADES],
    tp_carta banco[],
    int total,
    int *estado
) {
    int unidade;
    int dificuldade;

    for (unidade = 1; unidade <= NUM_UNIDADES; unidade++) {
        for (dificuldade = 1; dificuldade <= NUM_DIFICULDADES; dificuldade++) {
            recarregar_pilha_unidade(
                &pilhas[unidade - 1][dificuldade - 1],
                banco,
                total,
                unidade,
                dificuldade,
                estado
            );
        }
    }
}

int sortear_carta(
    tp_pilha pilhas[NUM_UNIDADES][NUM_DIFICULDADES],
    tp_carta banco[],
    int total,
    int unidade,
    int dificuldade,
    int *estado,
    tp_carta *carta_sorteada
) {
    tp_item id_carta;
    tp_pilha *pilha;

    if (unidade < 1 || unidade > NUM_UNIDADES) {
        return 0;
    }

    if (dificuldade < 1 || dificuldade > NUM_DIFICULDADES) {
        return 0;
    }

    pilha = &pilhas[unidade - 1][dificuldade - 1];

    if (pilha_vazia(pilha)) {
        printf(
            "\nPerguntas da unidade %d dificuldade %d esgotadas. Reembaralhando...\n",
            unidade,
            dificuldade
        );
        recarregar_pilha_unidade(pilha, banco, total, unidade, dificuldade, estado);
    }

    if (!pop(pilha, &id_carta)) {
        return 0;
    }

    if (id_carta < 0 || id_carta >= total) {
        return 0;
    }

    *carta_sorteada = banco[id_carta];
    return 1;
}

#endif
