#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct tp_no_est {
    int casa;
    int visitas;
    int acertos;
    int erros;
    struct tp_no_est *esq;
    struct tp_no_est *dir;
} tp_no_est;

tp_no_est *criar_no_est(int casa) {
    tp_no_est *novo = (tp_no_est *)malloc(sizeof(tp_no_est));
    if (novo == NULL) {
        return NULL;
    }

    novo->casa = casa;
    novo->visitas = 0;
    novo->acertos = 0;
    novo->erros = 0;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

tp_no_est *obter_ou_inserir(tp_no_est **raiz, int casa) {
    tp_no_est *atual;
    tp_no_est *pai = NULL;

    if (raiz == NULL) {
        return NULL;
    }

    atual = *raiz;
    while (atual != NULL) {
        if (casa == atual->casa) {
            return atual;
        }
        pai = atual;
        if (casa < atual->casa) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    atual = criar_no_est(casa);
    if (atual == NULL) {
        return NULL;
    }

    if (pai == NULL) {
        *raiz = atual;
    } else if (casa < pai->casa) {
        pai->esq = atual;
    } else {
        pai->dir = atual;
    }

    return atual;
}

void inicializar_estatisticas(tp_no_est **raiz, int total_casas) {
    int i;

    for (i = 1; i <= total_casas; i++) {
        obter_ou_inserir(raiz, i);
    }
}

void registrar_visita(tp_no_est **raiz, int casa) {
    tp_no_est *no = obter_ou_inserir(raiz, casa);
    if (no != NULL) {
        no->visitas++;
    }
}

void registrar_resultado(tp_no_est **raiz, int casa, int acertou) {
    tp_no_est *no = obter_ou_inserir(raiz, casa);
    if (no == NULL) {
        return;
    }

    if (acertou) {
        no->acertos++;
    } else {
        no->erros++;
    }
}

void salvar_relatorio_interno(tp_no_est *raiz, FILE *arquivo) {
    if (raiz == NULL || arquivo == NULL) {
        return;
    }

    salvar_relatorio_interno(raiz->esq, arquivo);
    fprintf(
        arquivo,
        "%d | %d | %d | %d\n",
        raiz->casa,
        raiz->visitas,
        raiz->acertos,
        raiz->erros
    );
    salvar_relatorio_interno(raiz->dir, arquivo);
}

int salvar_relatorio(tp_no_est *raiz, char caminho[]) {
    FILE *arquivo = fopen(caminho, "w");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "RELATORIO DE ESTATISTICAS\n");
    fprintf(arquivo, "Casa | Visitas | Acertos | Erros\n");
    salvar_relatorio_interno(raiz, arquivo);
    fclose(arquivo);
    return 1;
}

void liberar_estatisticas(tp_no_est *raiz) {
    if (raiz == NULL) {
        return;
    }

    liberar_estatisticas(raiz->esq);
    liberar_estatisticas(raiz->dir);
    free(raiz);
}

#endif
