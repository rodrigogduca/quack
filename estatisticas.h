#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

// estatisticas.h
// ---------------------------------------------------------------------------
// Arvore de estatisticas para visitas, acertos e erros por casa.

#include <stdio.h>
#include <stdlib.h>

typedef struct tp_no_est {
    int casa; // Numero da casa usada como chave.
    int visitas; // Quantas vezes a casa foi visitada.
    int acertos; // Respostas corretas nessa casa.
    int erros; // Respostas incorretas nessa casa.
    struct tp_no_est *esq; // Filho a esquerda.
    struct tp_no_est *dir; // Filho a direita.
} tp_no_est;

// Cria um no de estatisticas com contadores zerados.
tp_no_est *criar_no_est(int casa) {
    // Aloca memoria para o novo no.
    tp_no_est *novo = (tp_no_est *)malloc(sizeof(tp_no_est));
    if (novo == NULL) {
        return NULL;
    }
    // Inicializa campos basicos e ponteiros.
    novo->casa = casa;
    novo->visitas = 0;
    novo->acertos = 0;
    novo->erros = 0;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// Busca a casa na arvore; insere se nao existir.
tp_no_est *obter_ou_inserir(tp_no_est **raiz, int casa) {
    tp_no_est *atual;
    tp_no_est *pai = NULL;

    if (raiz == NULL) {
        return NULL;
    }

    // Caminha na arvore ate encontrar a chave ou o ponto de insercao.
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

    // Cria e insere o novo no no lado correto do pai.
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

// Inicializa a arvore com todas as casas do tabuleiro.
void inicializar_estatisticas(tp_no_est **raiz, int total_casas) {
    int i;

    // Pre-carrega a arvore com todas as casas para facilitar o registro.
    for (i = 1; i <= total_casas; i++) {
        obter_ou_inserir(raiz, i);
    }
}

// Soma uma visita para a casa informada.
void registrar_visita(tp_no_est **raiz, int casa) {
    // Garante que o no exista antes de incrementar.
    tp_no_est *no = obter_ou_inserir(raiz, casa);
    if (no != NULL) {
        no->visitas++;
    }
}

// Registra acerto ou erro para a casa informada.
void registrar_resultado(tp_no_est **raiz, int casa, int acertou) {
    // Garante a existencia do no e atualiza o contador correto.
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

// Percorre em ordem para gerar saida ordenada por casa.
void salvar_relatorio_interno(tp_no_est *raiz, FILE *arquivo) {
    if (raiz == NULL || arquivo == NULL) {
        return;
    }
    // Traversal em ordem: esquerda, no atual, direita.
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

// Salva o relatorio completo em arquivo texto.
int salvar_relatorio(tp_no_est *raiz, char caminho[]) {
    // Abre o arquivo e grava o cabecalho do relatorio.
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

// Libera recursivamente a arvore de estatisticas.
void liberar_estatisticas(tp_no_est *raiz) {
    if (raiz == NULL) {
        return;
    }
    // Libera em pos-ordem para nao perder os ponteiros.
    liberar_estatisticas(raiz->esq);
    liberar_estatisticas(raiz->dir);
    free(raiz);
}

#endif
