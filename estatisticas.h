#ifndef ESTATISTICAS_H
#define ESTATISTICAS_H

// estatisticas.h
// Funcoes de exportacao de historico de respostas e banco de perguntas em CSV.

#include <stdio.h>
#include <stdlib.h>
#include "dados.h"

// Salva uma resposta individual no historico CSV baseado no PDF de orientacao.
void salvar_historico_resposta(
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
    FILE *arquivo = fopen("historico_respostas.csv", "a");
    long tamanho;

    if (arquivo == NULL) return;

    // Verifica tamanho para inserir cabecalho se estiver vazio.
    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);
    if (tamanho == 0) {
        fprintf(arquivo, "turma;id_jogador;id_pergunta;unidade;tema;subtema;dificuldade;resposta_jogador;resposta_correta;resultado\n");
    }

    // Mapeia os numeros para as descricoes legiveis
    char *dif_str = "Facil";
    if (dificuldade == 2) dif_str = "Medio";
    else if (dificuldade == 3) dif_str = "Dificil";

    char *res_str = acertou ? "Acertou" : "Errou";
    char resp_jog = (resposta_jogador >= 1 && resposta_jogador <= 5) ? (char)('A' + resposta_jogador - 1) : '-';
    char resp_cor = (char)('A' + resposta_correta - 1);

    fprintf(arquivo, "%s;%s;P%03d;%d;%s;%s;%s;%c;%c;%s\n",
            turma, nome_jogador, id_carta, unidade, tema, subtema, dif_str, resp_jog, resp_cor, res_str);

    fclose(arquivo);
}

// Exporta o banco de perguntas para um arquivo CSV separado para fins pedagogicos.
void exportar_perguntas_csv(tp_carta banco[], int total) {
    FILE *arquivo = fopen("perguntas.csv", "w");
    int i;
    
    if (arquivo == NULL) return;

    fprintf(arquivo, "id_pergunta;unidade;tema;dificuldade;enunciado\n");
    
    for (i = 0; i < total; i++) {
        // Ignora cartas nao inicializadas
        if (banco[i].id_carta == 0) continue;
        
        char *dif_str = "Facil";
        if (banco[i].dificuldade == 2) dif_str = "Medio";
        else if (banco[i].dificuldade == 3) dif_str = "Dificil";

        fprintf(arquivo, "P%03d;%d;%s;%s;%s\n",
                banco[i].id_carta,
                banco[i].unidade,
                banco[i].tema,
                dif_str,
                banco[i].pergunta);
    }
    
    fclose(arquivo);
}

#endif
