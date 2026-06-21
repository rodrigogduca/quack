#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

// estatisticas.h
// Arvore binaria para organizar registros de respostas e exportacao CSV.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dados.h"

// Registro armazenado em cada no da arvore de estatisticas.
typedef struct {
    char turma[50];
    char nome_jogador[50];
    int id_carta;
    int unidade;
    char tema[50];
    char subtema[50];
    int dificuldade;
    int resposta_jogador;
    int resposta_correta;
    int acertou;
} tp_registro_estatistica;

// No da arvore binaria de estatisticas.
typedef struct tp_no_estatistica {
    tp_registro_estatistica registro;
    struct tp_no_estatistica *esquerda;
    struct tp_no_estatistica *direita;
} tp_no_estatistica;

// Estrutura principal que controla a arvore de estatisticas.
typedef struct {
    tp_no_estatistica *raiz;
    int total;
} tp_arvore_estatisticas;

// Inicializa arvore vazia.
void inicializar_arvore_estatisticas(tp_arvore_estatisticas *arvore) {
    arvore->raiz = NULL;
    arvore->total = 0;
}

// Converte dificuldade numerica para texto legivel.
char *texto_dificuldade(int dificuldade) {
    if (dificuldade == 2) {
        return "Medio";
    }

    if (dificuldade == 3) {
        return "Dificil";
    }

    return "Facil";
}

// Converte resposta numerica para alternativa A-E.
char letra_resposta(int resposta) {
    if (resposta >= 1 && resposta <= 5) {
        return (char)('A' + resposta - 1);
    }

    return '-';
}

// Monta um registro de estatistica a partir dos dados da rodada.
tp_registro_estatistica criar_registro_estatistica(
    char turma[],
    char nome_jogador[],
    int id_carta,
    int unidade,
    char tema[],
    char subtema[],
    int dificuldade,
    int resposta_jogador,
    int resposta_correta,
    int acertou
) {
    tp_registro_estatistica registro;

    copiar_texto(registro.turma, sizeof(registro.turma), turma);
    copiar_texto(registro.nome_jogador, sizeof(registro.nome_jogador), nome_jogador);
    registro.id_carta = id_carta;
    registro.unidade = unidade;
    copiar_texto(registro.tema, sizeof(registro.tema), tema);
    copiar_texto(registro.subtema, sizeof(registro.subtema), subtema);
    registro.dificuldade = dificuldade;
    registro.resposta_jogador = resposta_jogador;
    registro.resposta_correta = resposta_correta;
    registro.acertou = acertou;

    return registro;
}

// Cria um novo no para a arvore.
tp_no_estatistica *criar_no_estatistica(tp_registro_estatistica registro) {
    tp_no_estatistica *novo = (tp_no_estatistica *)malloc(sizeof(tp_no_estatistica));

    if (novo == NULL) {
        return NULL;
    }

    novo->registro = registro;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

// Compara registros para ordenar a arvore por pergunta e jogador.
int comparar_registros_estatistica(
    tp_registro_estatistica *primeiro,
    tp_registro_estatistica *segundo
) {
    int comparacao_nome;

    if (primeiro->id_carta < segundo->id_carta) {
        return -1;
    }

    if (primeiro->id_carta > segundo->id_carta) {
        return 1;
    }

    comparacao_nome = strcmp(primeiro->nome_jogador, segundo->nome_jogador);
    if (comparacao_nome != 0) {
        return comparacao_nome;
    }

    if (primeiro->resposta_jogador < segundo->resposta_jogador) {
        return -1;
    }

    if (primeiro->resposta_jogador > segundo->resposta_jogador) {
        return 1;
    }

    return 1;
}

// Insere recursivamente um registro na arvore.
tp_no_estatistica *inserir_no_estatistica(
    tp_no_estatistica *raiz,
    tp_registro_estatistica registro,
    int *inseriu
) {
    int comparacao;

    if (raiz == NULL) {
        *inseriu = 1;
        return criar_no_estatistica(registro);
    }

    comparacao = comparar_registros_estatistica(&registro, &raiz->registro);

    if (comparacao < 0) {
        raiz->esquerda = inserir_no_estatistica(raiz->esquerda, registro, inseriu);
    } else {
        raiz->direita = inserir_no_estatistica(raiz->direita, registro, inseriu);
    }

    return raiz;
}

// Insere um registro na arvore e atualiza o total.
int inserir_estatistica(
    tp_arvore_estatisticas *arvore,
    tp_registro_estatistica registro
) {
    int inseriu = 0;

    arvore->raiz = inserir_no_estatistica(arvore->raiz, registro, &inseriu);

    if (inseriu) {
        arvore->total++;
    }

    return inseriu;
}

// Busca a primeira estatistica encontrada para uma carta.
tp_no_estatistica *buscar_estatistica_por_carta(tp_no_estatistica *raiz, int id_carta) {
    if (raiz == NULL) {
        return NULL;
    }

    if (id_carta == raiz->registro.id_carta) {
        return raiz;
    }

    if (id_carta < raiz->registro.id_carta) {
        return buscar_estatistica_por_carta(raiz->esquerda, id_carta);
    }

    return buscar_estatistica_por_carta(raiz->direita, id_carta);
}

// Escreve uma linha CSV de estatistica.
void escrever_registro_estatistica_csv(FILE *arquivo, tp_registro_estatistica *registro) {
    fprintf(
        arquivo,
        "%s;%s;P%03d;%d;%s;%s;%s;%c;%c;%s\n",
        registro->turma,
        registro->nome_jogador,
        registro->id_carta,
        registro->unidade,
        registro->tema,
        registro->subtema,
        texto_dificuldade(registro->dificuldade),
        letra_resposta(registro->resposta_jogador),
        letra_resposta(registro->resposta_correta),
        registro->acertou ? "Acertou" : "Errou"
    );
}

// Percorre a arvore em ordem e exporta os registros.
void exportar_arvore_estatisticas_em_ordem(FILE *arquivo, tp_no_estatistica *raiz) {
    if (raiz == NULL) {
        return;
    }

    exportar_arvore_estatisticas_em_ordem(arquivo, raiz->esquerda);
    escrever_registro_estatistica_csv(arquivo, &raiz->registro);
    exportar_arvore_estatisticas_em_ordem(arquivo, raiz->direita);
}

// Libera todos os nos da arvore.
void destruir_arvore_estatisticas(tp_no_estatistica *raiz) {
    if (raiz == NULL) {
        return;
    }

    destruir_arvore_estatisticas(raiz->esquerda);
    destruir_arvore_estatisticas(raiz->direita);
    free(raiz);
}

// Salva cabecalho quando o arquivo ainda esta vazio.
void escrever_cabecalho_historico(FILE *arquivo) {
    long tamanho;

    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);

    if (tamanho == 0) {
        fprintf(
            arquivo,
            "turma;id_jogador;id_pergunta;unidade;tema;subtema;dificuldade;resposta_jogador;resposta_correta;resultado\n"
        );
    }
}

// Salva uma resposta individual no historico CSV usando a estrutura de arvore.
void salvar_historico_resposta(
    char turma[50],
    char nome_jogador[50],
    int id_carta,
    int unidade,
    char tema[50],
    char subtema[50],
    int dificuldade,
    int resposta_jogador,
    int resposta_correta,
    int acertou
) {
    FILE *arquivo;
    tp_arvore_estatisticas arvore;
    tp_registro_estatistica registro;

    inicializar_arvore_estatisticas(&arvore);
    registro = criar_registro_estatistica(
        turma,
        nome_jogador,
        id_carta,
        unidade,
        tema,
        subtema,
        dificuldade,
        resposta_jogador,
        resposta_correta,
        acertou
    );

    if (!inserir_estatistica(&arvore, registro)) {
        return;
    }

    arquivo = fopen("historico_respostas.csv", "a");
    if (arquivo == NULL) {
        destruir_arvore_estatisticas(arvore.raiz);
        return;
    }

    escrever_cabecalho_historico(arquivo);
    exportar_arvore_estatisticas_em_ordem(arquivo, arvore.raiz);

    fclose(arquivo);
    destruir_arvore_estatisticas(arvore.raiz);
}

// Exporta o banco de perguntas para um arquivo CSV separado para fins pedagogicos.
void exportar_perguntas_csv(tp_carta banco[], int total) {
    FILE *arquivo = fopen("perguntas.csv", "w");
    int i;

    if (arquivo == NULL) {
        return;
    }

    fprintf(arquivo, "id_pergunta;unidade;tema;dificuldade;enunciado\n");

    for (i = 0; i < total; i++) {
        if (banco[i].id_carta == 0) {
            continue;
        }

        fprintf(
            arquivo,
            "P%03d;%d;%s;%s;%s\n",
            banco[i].id_carta,
            banco[i].unidade,
            banco[i].tema,
            texto_dificuldade(banco[i].dificuldade),
            banco[i].pergunta
        );
    }

    fclose(arquivo);
}

#endif
