// historico.c
// Implementacao da Arvore Binaria de Busca (BST) para historico de respostas do jogo.
// Cada resposta do jogador e registrada em um no da arvore e depois exportada
// para historico_respostas.csv. O banco de perguntas tambem e exportado.

#include "historico.h"

// Inicializa a arvore com raiz nula e contador zerado
void inicializar_arvore_estatisticas(tp_arvore_estatisticas *arvore) {
    arvore->raiz = NULL;
    arvore->total = 0;
}

// Converte dificuldade numerica para string legivel
char *texto_dificuldade(int dificuldade) {
    if (dificuldade == 2) return "Médio";
    if (dificuldade == 3) return "Difícil";
    return "Fácil";
}

// Converte numero da resposta (1-5) para letra maiuscula (A-E)
char letra_resposta(int resposta) {
    if (resposta >= 1 && resposta <= 5) return (char)('A' + resposta - 1);
    return '-';
}

// Cria e retorna um registro preenchendo todos os campos
tp_registro_estatistica criar_registro_estatistica(char turma[], char nome_jogador[], int id_carta, int unidade, char tema[], char subtema[], int dificuldade, int resposta_jogador, int resposta_correta, int acertou) {
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

// Aloca um novo no da arvore, copia o registro e inicializa filhos como NULL
tp_no_estatistica *criar_no_estatistica(tp_registro_estatistica registro) {
    tp_no_estatistica *novo = (tp_no_estatistica *)malloc(sizeof(tp_no_estatistica));
    if (novo == NULL) return NULL;
    novo->registro = registro;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Compara dois registros. Ordenacao:
//   1. id_carta (crescente)
//   2. nome_jogador (ordem alfabetica)
//   3. resposta_jogador (crescente)
// Se todos iguais, retorna 1 (para evitar duplicatas exatas)
int comparar_registros_estatistica(tp_registro_estatistica *primeiro, tp_registro_estatistica *segundo) {
    int comparacao_nome;
    if (primeiro->id_carta < segundo->id_carta) return -1;
    if (primeiro->id_carta > segundo->id_carta) return 1;
    comparacao_nome = strcmp(primeiro->nome_jogador, segundo->nome_jogador);
    if (comparacao_nome != 0) return comparacao_nome;
    if (primeiro->resposta_jogador < segundo->resposta_jogador) return -1;
    if (primeiro->resposta_jogador > segundo->resposta_jogador) return 1;
    return 1;
}

// Insere recursivamente um registro na BST.
// Se o registro ja existe (comparacao == 0), nao insere (inseriu = 0).
tp_no_estatistica *inserir_no_estatistica(tp_no_estatistica *raiz, tp_registro_estatistica registro, int *inseriu) {
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

// Insere um registro na arvore (interface publica).
// Atualiza o contador total se a insercao foi bem-sucedida.
int inserir_estatistica(tp_arvore_estatisticas *arvore, tp_registro_estatistica registro) {
    int inseriu = 0;
    arvore->raiz = inserir_no_estatistica(arvore->raiz, registro, &inseriu);
    if (inseriu) arvore->total++;
    return inseriu;
}

// Busca binaria na BST pelo id_carta. Retorna o no encontrado ou NULL.
tp_no_estatistica *buscar_estatistica_por_carta(tp_no_estatistica *raiz, int id_carta) {
    if (raiz == NULL) return NULL;
    if (id_carta == raiz->registro.id_carta) return raiz;
    if (id_carta < raiz->registro.id_carta) return buscar_estatistica_por_carta(raiz->esquerda, id_carta);
    return buscar_estatistica_por_carta(raiz->direita, id_carta);
}

// Escreve uma linha no formato CSV: turma;nome;id;unidade;tema;subtema;dificuldade;resp_jog;resp_corr;resultado
void escrever_registro_estatistica_csv(FILE *arquivo, tp_registro_estatistica *registro) {
    fprintf(arquivo, "%s;%s;P%03d;%d;%s;%s;%s;%c;%c;%s\n",
        registro->turma, registro->nome_jogador, registro->id_carta, registro->unidade,
        registro->tema, registro->subtema, texto_dificuldade(registro->dificuldade),
        letra_resposta(registro->resposta_jogador), letra_resposta(registro->resposta_correta),
        registro->acertou ? "Acertou" : "Errou");
}

// Percorre a arvore em ordem (esquerda-raiz-direita) escrevendo cada registro no CSV.
// O percurso em-ordem garante saida ordenada pelo criterio de comparacao.
void exportar_arvore_estatisticas_em_ordem(FILE *arquivo, tp_no_estatistica *raiz) {
    if (raiz == NULL) return;
    exportar_arvore_estatisticas_em_ordem(arquivo, raiz->esquerda);
    escrever_registro_estatistica_csv(arquivo, &raiz->registro);
    exportar_arvore_estatisticas_em_ordem(arquivo, raiz->direita);
}

// Libera memoria de todos os nos da arvore (percurso pos-ordem: esquerda-direita-raiz)
void destruir_arvore_estatisticas(tp_no_estatistica *raiz) {
    if (raiz == NULL) return;
    destruir_arvore_estatisticas(raiz->esquerda);
    destruir_arvore_estatisticas(raiz->direita);
    free(raiz);
}

// Escreve o cabecalho do CSV apenas se o arquivo estiver vazio (acabou de ser criado)
void escrever_cabecalho_historico(FILE *arquivo) {
    long tamanho;
    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    if (tamanho == 0) {
        fprintf(arquivo, "turma;id_jogador;id_pergunta;unidade;tema;subtema;dificuldade;resposta_jogador;resposta_correta;resultado\n");
    }
}

// Funcao principal de salvamento: cria o registro, insere na arvore e escreve no CSV.
// Usa a arvore para ordenacao antes de escrever no arquivo.
void salvar_historico_resposta(char turma[50], char nome_jogador[50], int id_carta, int unidade, char tema[50], char subtema[50], int dificuldade, int resposta_jogador, int resposta_correta, int acertou) {
    FILE *arquivo;
    tp_arvore_estatisticas arvore;
    tp_registro_estatistica registro;
    inicializar_arvore_estatisticas(&arvore);
    registro = criar_registro_estatistica(turma, nome_jogador, id_carta, unidade, tema, subtema, dificuldade, resposta_jogador, resposta_correta, acertou);
    if (!inserir_estatistica(&arvore, registro)) return;
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

// Exporta o banco de perguntas para perguntas.csv (apenas cabecalho + id, unidade, tema, dificuldade, enunciado)
void exportar_perguntas_csv(tp_carta banco[], int total) {
    FILE *arquivo = fopen("perguntas.csv", "w");
    int i;
    if (arquivo == NULL) return;
    fprintf(arquivo, "id_pergunta;unidade;tema;dificuldade;enunciado\n");
    for (i = 0; i < total; i++) {
        if (banco[i].id_carta == 0) continue;
        fprintf(arquivo, "P%03d;%d;%s;%s;%s\n", banco[i].id_carta, banco[i].unidade, banco[i].tema, texto_dificuldade(banco[i].dificuldade), banco[i].pergunta);
    }
    fclose(arquivo);
}
