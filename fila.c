// fila.c
// Implementacao da Fila Circular (FIFO).
// Operacoes: insere_fila (enfileirar ao final), remove_fila (desenfileirar do inicio).
// Usa a funcao estatica proximo() para navegacao circular no vetor.

#include "fila.h"

// Avanca um indice no vetor circular (volta a 0 ao ultrapassar MAX-1)
int proximo(int pos) {
    if (pos == MAX - 1) return 0;
    return pos + 1;
}

// Inicializa fila: ini e fim apontam para MAX-1 (estado vazio)
void inicializa_fila(tp_fila *f) {
    f->ini = MAX - 1;
    f->fim = MAX - 1;
}

// Fila vazia quando ini == fim (nenhum elemento entre eles)
int fila_vazia(tp_fila *f) {
    return f->ini == f->fim;
}

// Fila cheia quando o proximo de fim alcanca ini (um espaco sempre vago)
int fila_cheia(tp_fila *f) {
    return proximo(f->fim) == f->ini;
}

// Insere 'e' na posicao seguinte a fim. Nao insere se fila cheia.
int insere_fila(tp_fila *f, tp_item e) {
    if (fila_cheia(f)) return 0;
    f->fim = proximo(f->fim);
    f->item[f->fim] = e;
    return 1;
}

// Remove o elemento na posicao seguinte a ini e o copia para *e.
int remove_fila(tp_fila *f, tp_item *e) {
    if (fila_vazia(f)) return 0;
    f->ini = proximo(f->ini);
    *e = f->item[f->ini];
    return 1;
}
