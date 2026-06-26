// fila.h
// Declaracoes da estrutura Fila Circular (FIFO - First In, First Out).
// Usada no gerenciamento da ordem dos turnos dos jogadores.

#ifndef FILA_H
#define FILA_H

#ifndef MAX
#define MAX 100
#endif

#ifndef TP_ITEM_DEFINED
#define TP_ITEM_DEFINED
typedef int tp_item;  // Tipo generico dos itens da fila
#endif

// Estrutura de uma fila circular estatica
// Utiliza um vetor com indices ini (anterior ao primeiro) e fim (ultimo).
typedef struct {
    tp_item item[MAX]; // Vetor circular que armazena os elementos
    int ini;           // Indice anterior ao primeiro elemento
    int fim;           // Indice do ultimo elemento
} tp_fila;

// Inicializa a fila (ini = fim = MAX-1, indicando vazia)
void inicializa_fila(tp_fila *f);

// Retorna 1 se a fila esta vazia, 0 caso contrario
int fila_vazia(tp_fila *f);

// Retorna 1 se a fila esta cheia, 0 caso contrario
int fila_cheia(tp_fila *f);

// Insere elemento ao final da fila. Retorna 1 se sucesso, 0 se cheia.
int insere_fila(tp_fila *f, tp_item e);

// Remove o elemento do inicio da fila e o armazena em *e. Retorna 1 se sucesso, 0 se vazia.
int remove_fila(tp_fila *f, tp_item *e);

#endif
