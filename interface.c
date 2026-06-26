// interface.c
// Implementacao completa da interface grafica do QUACK Boardgame.
// Contem todas as telas (menu, jogo, pergunta, resultado, vencedor, pause,
// historico), o desenho do tabuleiro 3D, painel lateral, mascote pato,
// tokens 3D, dado 2D e o loop principal da GUI.

#include "interface.h"

// ========================================================================
// FUNCOES AUXILIARES DE INTERFACE
// ========================================================================

// Verifica se o cursor do mouse esta dentro do retangulo 'r'.
// Retorna 1 se sim, 0 caso contrario.
int mouse_em(Rectangle r) {
    return CheckCollisionPointRec(GetMousePosition(), r);
}

// Desenha um texto centralizado horizontalmente na posicao y.
void txt_draw(Estado *estado, char *texto, int x, int y, int tamanho, Color cor) {
    if (estado->font_carregada)
        DrawTextEx(estado->font_corpo, texto, (Vector2){ (float)x, (float)y }, tamanho, 1, cor);
    else
        DrawText(texto, x, y, tamanho, cor);
}

int txt_measure(Estado *estado, char *texto, int tamanho) {
    if (estado->font_carregada)
        return (int)MeasureTextEx(estado->font_corpo, texto, tamanho, 1).x;
    return MeasureText(texto, tamanho);
}

void txt_centro(Estado *estado, char *texto, int y, int tamanho, Color cor) {
    txt_draw(estado, texto, GetScreenWidth() / 2 - txt_measure(estado, texto, tamanho) / 2, y, tamanho, cor);
}

// Desenha texto com quebra de linha automatica (word wrap).
// Respeita 'largura_max' em pixels, quebra em palavras (espaços) e
// tambem em caracteres de nova linha ('\n').
void txt_wrap(Estado *estado, char *texto, int x, int y, int largura_max, int tamanho, Color cor) {
    int comprimento = strlen(texto);
    int inicio = 0;
    int linha_num = 0;
    while (inicio < comprimento) {
        int fim = inicio;
        int largura = 0;
        int ultimo_espaco = -1;
        while (fim < comprimento && texto[fim] != '\n') {
            char tmp[2] = { texto[fim], 0 };
            int cl = txt_measure(estado, tmp, tamanho);
            if (largura + cl > largura_max && largura > 0) break;
            largura += cl;
            if (texto[fim] == ' ') ultimo_espaco = fim;
            fim++;
        }
        if (fim == inicio) break;
        int copia = fim - inicio;
        if (ultimo_espaco >= 0 && texto[fim] != '\n' && fim < comprimento)
            copia = ultimo_espaco - inicio;
        if (copia <= 0)
            copia = fim - inicio;
        char linha[256];
        strncpy(linha, texto + inicio, copia);
        linha[copia] = '\0';
        txt_draw(estado, linha, x, y + linha_num * (tamanho + 4), tamanho, cor);
        linha_num++;
        if (texto[fim] == '\n') fim++;
        inicio += copia;
        if (texto[inicio] == ' ') inicio++;
    }
}

// Inicia uma transicao de fade entre telas (se reduced_motion estiver desligado)
void iniciar_transicao(Estado *estado) {
    if (estado->reduced_motion) return;
    tween_start(&estado->transicao, 0, 1, 0.25f, ease_sine_out);
    estado->transicao_ativa = 1;
}

// Atualiza e desenha a transicao de fade (retangulo preto com alpha crescente)
void draw_transicao(Estado *estado, float dt) {
    if (!estado->transicao_ativa) return;
    float alpha_transicao = tween_update(&estado->transicao, dt);
    if (tween_done(&estado->transicao)) {
        estado->transicao_ativa = 0;
        return;
    }
    DrawRectangle(0, 0, estado->larg, estado->alt, RGBA(0, 0, 0, (int)(alpha_transicao * 255)));
}

// ========================================================================
// PAINEL LATERAL
// ========================================================================

// Desenha o painel informativo no lado direito da tela de jogo.
// Contem: rodada atual, jogador da vez, barra de progresso, dado,
// botao para rolar, placar dos jogadores e botao de pause.
void draw_painel(Estado *estado) {
    int painel_largura = 280;
    if (painel_largura > estado->larg - 300) painel_largura = estado->larg - 300;
    int painel_x = estado->larg - painel_largura - 15;
    int painel_y = 15;
    int painel_h = estado->alt - 30;
    int conteudo_x = painel_x + 18;
    int conteudo_y;
    char buffer[128];
    tp_player *jogador = &estado->jogadores[estado->jogador_atual];
    DrawRectangle(painel_x + 4, painel_y + 5, painel_largura, painel_h, RGBA(0, 0, 0, 30));
    DrawRectangle(painel_x + 2, painel_y + 3, painel_largura, painel_h, RGBA(0, 0, 0, 50));
    DrawRectangle(painel_x, painel_y, painel_largura, painel_h, COR_SURFACE);
    DrawRectangleLinesEx((Rectangle){ painel_x, painel_y, painel_largura, painel_h }, 2, COR_PRIMARY);
    DrawLine(painel_x, painel_y + 55, painel_x + painel_largura, painel_y + 55, COR_PRIMARY_MUTED);

    conteudo_y = painel_y + 12;
    badge_draw((Vector2){ conteudo_x, conteudo_y }, "RODADA", BADGE_PRIMARY);
    sprintf(buffer, "%d", estado->rodada);
    txt_draw(estado, buffer, conteudo_x + 80, conteudo_y + 2, FONT_XL, WHITE);

    conteudo_y = painel_y + 65;
    badge_draw((Vector2){ conteudo_x, conteudo_y }, "VEZ DE", BADGE_INFO);
    conteudo_y += 30;
    draw_pato(estado, conteudo_x + 45, conteudo_y + 14, 0.55f,
              cor_jogador(estado->jogador_atual), 0);
    Color cor_nome = cor_jogador(estado->jogador_atual);
    sprintf(buffer, "%s", jogador->nome);
    txt_draw(estado, buffer, conteudo_x + 85, conteudo_y + 20, FONT_LG, cor_nome);

    conteudo_y += 50;
    sprintf(buffer, "Casa %d  |  Unidade %d", jogador->posicao, unidade_por_posicao(jogador->posicao));
    txt_draw(estado, buffer, conteudo_x, conteudo_y, FONT_SM, COR_TEXT_MUTED);

    conteudo_y += 30;
    DrawLine(painel_x + 10, conteudo_y, painel_x + painel_largura - 10, conteudo_y, COR_PRIMARY_MUTED);

    conteudo_y += 10;
    badge_draw((Vector2){ conteudo_x, conteudo_y }, "DADO", BADGE_WARNING);
    conteudo_y += 32;
    int dado_centro_x = conteudo_x + (painel_largura - 36) / 2;
    int dado_centro_y = conteudo_y + 22;
    draw_dado(dado_centro_x, dado_centro_y, 44, estado->ultimo_dado);

    conteudo_y += 55;
    int inner_w = painel_largura - 36;
    Rectangle botao_rolar = { (float)conteudo_x, (float)conteudo_y, inner_w, 48 };
    if (botao(botao_rolar, "ROLAR DADO", BTN_PRIMARY, &estado->som_clique)) {
        if (estado->aguardando_dado && estado->partida_ativa) {
            estado->ultimo_dado = rolar_dado();

            if (!estado->reduced_motion) {
                tween_start(&estado->dado_tween, 0, 1, 0.3f, ease_quad_out);
            }

            estado->jogadores[estado->jogador_atual].posicao = mover_posicao(
                &estado->tabuleiro,
                estado->jogadores[estado->jogador_atual].posicao,
                estado->ultimo_dado
            );
            registrar_historico_posicao(
                &estado->jogadores[estado->jogador_atual],
                estado->jogadores[estado->jogador_atual].posicao
            );

            if (estado->jogadores[estado->jogador_atual].posicao >= estado->tabuleiro.total) {
                estado->tela = TELA_VENCEDOR;
                return;
            }

            tp_casa *casa = obter_casa(
                &estado->tabuleiro,
                estado->jogadores[estado->jogador_atual].posicao
            );

            if (casa && casa->tipo == 1) {
                PlaySound(estado->som_alerta);
                int nivel = sortear_dificuldade();
                if (sortear_carta(estado->pilhas, estado->banco, 36,
                                  casa->unidade, nivel, &estado->carta_atual)) {
                    estado->tela = TELA_PERGUNTA;
                }
                estado->aguardando_dado = 0;
            } else if (casa && casa->tipo == 2) {
                estado->jogadores[estado->jogador_atual].posicao = mover_posicao(
                    &estado->tabuleiro, estado->jogadores[estado->jogador_atual].posicao, casa->efeito
                );
                registrar_historico_posicao(
                    &estado->jogadores[estado->jogador_atual],
                    estado->jogadores[estado->jogador_atual].posicao
                );
                if (estado->jogadores[estado->jogador_atual].posicao >= estado->tabuleiro.total) {
                    estado->tela = TELA_VENCEDOR;
                    return;
                }
                estado->aguardando_dado = 0;
            } else if (casa && casa->tipo == 3) {
                PlaySound(estado->som_fail);
                estado->jogadores[estado->jogador_atual].posicao = mover_posicao(
                    &estado->tabuleiro, estado->jogadores[estado->jogador_atual].posicao, -casa->efeito
                );
                registrar_historico_posicao(
                    &estado->jogadores[estado->jogador_atual],
                    estado->jogadores[estado->jogador_atual].posicao
                );
                estado->aguardando_dado = 0;
            } else {
                estado->aguardando_dado = 0;
            }
        }
    }

     conteudo_y += 70;
     DrawLine(painel_x + 10, conteudo_y, painel_x + painel_largura - 10, conteudo_y, COR_PRIMARY_MUTED);
     conteudo_y += 10;
     badge_draw((Vector2){ conteudo_x, conteudo_y }, "PLACAR", BADGE_INFO);
     {
         int k;
         int avail_h = painel_y + painel_h - 65 - (conteudo_y + 40);
         int row_spacing = 52;
         if (estado->num_jogadores * row_spacing > avail_h) {
             row_spacing = avail_h / estado->num_jogadores;
             if (row_spacing < 28) row_spacing = 28;
         }
         int bar_x = conteudo_x + 24;
         int bar_w = painel_largura - 36 - 24;
         for (k = 0; k < estado->num_jogadores; k++) {
             Color cor_jog = cor_jogador(k);
             int row_y = conteudo_y + 40 + k * row_spacing;
             if (row_y + row_spacing > painel_y + painel_h - 50) break;
             draw_pato(estado, conteudo_x + 5, row_y, 0.3f,
                       cor_jogador(k), 0);
             sprintf(buffer, "%s: Casa %d",
                     estado->jogadores[k].nome, estado->jogadores[k].posicao);
             Color nome_cor = (k == estado->jogador_atual) ? cor_jog : COR_TEXT_MUTED;
             txt_draw(estado, buffer, conteudo_x + 24, row_y + 2, FONT_SM, nome_cor);
             int bar_y = row_y + 20;
             if (bar_w > 6) {
                 DrawRectangle(bar_x, bar_y, bar_w, 6, COR_SURFACE_ALT);
                 int fill_w = (int)(bar_w * ((float)(estado->jogadores[k].posicao - 1) / 29.0f));
                 if (fill_w > 0)
                     DrawRectangle(bar_x, bar_y, fill_w, 6, cor_jog);
             }
         }
     }

    int pause_w = painel_largura / 2;
    int pause_x = painel_x + (painel_largura - pause_w) / 2;
    Rectangle botao_pause = { (float)pause_x, (float)(painel_y + painel_h - 55), pause_w, 38 };
    if (botao(botao_pause, "PAUSE", BTN_SECONDARY, &estado->som_clique)) {
        estado->tela_anterior = TELA_JOGO;
        estado->tela = TELA_PAUSE;
    }
}

// ========================================================================
// TELA: JOGO (tabuleiro 2D + painel lateral)
// ========================================================================

// Desenha a tela principal do jogo combinando o tabuleiro 2D com o painel
// lateral. Avanca o turno automaticamente quando o jogador ja jogou.
void draw_jogo(Estado *estado) {
    draw_board(estado);
    draw_painel(estado);
    if (estado->tela != TELA_JOGO) return;
    if (!estado->aguardando_dado && estado->partida_ativa) {
        avancar_turno(estado);
    }
}

// ========================================================================
// TELA: PERGUNTA (exibe carta sorteada com alternativas)
// ========================================================================

// Exibe a tela de pergunta com o enunciado e 5 alternativas clicaveis.
// Cada alternativa mostra badge de letra (A-E). Ao clicar, salva o
// resultado no historico e avanca para TELA_RESULTADO ou TELA_VENCEDOR.
void draw_pergunta(Estado *estado) {
    char buffer[128];
    int i;

    DrawRectangle(0, 0, estado->larg, estado->alt, COR_BG);

    int cx = estado->larg / 2;
    int cy = estado->alt / 2;

    int badge_y = 30;
    {
        sprintf(buffer, "UNIDADE %d", estado->carta_atual.unidade);
        int bw1 = txt_measure(estado, buffer, FONT_SM) + 60;
        int badge_total_w = bw1 + 10;
        sprintf(buffer, "DIFICULDADE %d", estado->carta_atual.dificuldade);
        int bw2 = txt_measure(estado, buffer, FONT_SM) + 60;
        badge_total_w += bw2;
        int bx = cx - badge_total_w / 2;
        sprintf(buffer, "UNIDADE %d", estado->carta_atual.unidade);
        badge_draw((Vector2){ bx, badge_y }, buffer, BADGE_PRIMARY);
        bx += bw1 + 10;
        sprintf(buffer, "DIFICULDADE %d", estado->carta_atual.dificuldade);
        BadgeVariant bv = (estado->carta_atual.dificuldade == 1) ? BADGE_SUCCESS :
                          (estado->carta_atual.dificuldade == 2) ? BADGE_WARNING : BADGE_DANGER;
        badge_draw((Vector2){ bx, badge_y }, buffer, bv);
    }

    int py_perg = badge_y + 50;
    int perg_max_w = estado->larg * 0.75f;
    if (perg_max_w < 400) perg_max_w = estado->larg - 100;
    txt_wrap(estado, estado->carta_atual.pergunta, cx - perg_max_w / 2, py_perg, perg_max_w, FONT_XL, COR_TEXT);

    int py_alt = py_perg + 160;
    if (py_alt > estado->alt - 250) py_alt = estado->alt - 250;
    int esp_alt = (estado->alt - py_alt - 60) / 5;
    if (esp_alt < 80) esp_alt = 80;
    if (esp_alt > 115) esp_alt = 115;

    int alt_box_w = estado->larg * 0.80f;
    if (alt_box_w > 1000) alt_box_w = 1000;
    if (alt_box_w > estado->larg - 80) alt_box_w = estado->larg - 80;
    int alt_box_x = cx - alt_box_w / 2;

    for (i = 0; i < 5; i++) {
        int ya = py_alt + i * esp_alt;
        Rectangle r = { (float)alt_box_x, (float)ya, (float)alt_box_w, (float)(esp_alt - 6) };

        Color cor = COR_SURFACE_ALT;
        Color borda = COR_BORDER;
        if (mouse_em(r)) {
            cor = COR_SURFACE_LIGHT;
            borda = COR_PRIMARY;
        }

        DrawRectangleRounded(r, RAD_MD, 24, cor);
        DrawRectangleRoundedLines(r, RAD_MD, 24, borda);

        int circle_x = (int)r.x + 24;
        int circle_y = (int)(r.y + r.height / 2);
        Color circle_cor = mouse_em(r) ? COR_PRIMARY : COR_PRIMARY_MUTED;
        DrawCircle(circle_x, circle_y, 20, circle_cor);
        DrawCircleLines(circle_x, circle_y, 20, cor_clarear(circle_cor, 30));
        char letter[2] = { 'A' + i, '\0' };
        int lw = txt_measure(estado, letter, FONT_MD);
        txt_draw(estado, letter, circle_x - lw / 2, circle_y - FONT_MD / 2, FONT_MD, WHITE);

        char alt_text[512];
        sprintf(alt_text, "%s", estado->carta_atual.alternativas[i]);
        txt_wrap(estado, alt_text, (int)r.x + 54, (int)(r.y + 18), alt_box_w - 70, FONT_MD, COR_TEXT);

        if (mouse_em(r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            estado->resposta_escolhida = i + 1;
            int acertou = (estado->resposta_escolhida == estado->carta_atual.resposta);

            char turma[50] = "AED-2026.1-T";
            salvar_historico_resposta(turma,
                estado->jogadores[estado->jogador_atual].nome,
                estado->carta_atual.id_carta, estado->carta_atual.unidade,
                estado->carta_atual.tema, estado->carta_atual.subtema,
                estado->carta_atual.dificuldade,
                estado->resposta_escolhida, estado->carta_atual.resposta, acertou);

            estado->resultado_perg = acertou ? estado->carta_atual.avanco : -1;
            if (!acertou) PlaySound(estado->som_fail);
            else PlaySound(estado->som_acerto);
            if (estado->resultado_perg != 0) {
                estado->jogadores[estado->jogador_atual].posicao = mover_posicao(
                    &estado->tabuleiro,
                    estado->jogadores[estado->jogador_atual].posicao,
                    estado->resultado_perg
                );
                registrar_historico_posicao(
                    &estado->jogadores[estado->jogador_atual],
                    estado->jogadores[estado->jogador_atual].posicao
                );
            }
            if (estado->jogadores[estado->jogador_atual].posicao >= estado->tabuleiro.total)
                estado->tela = TELA_VENCEDOR;
            else
                estado->tela = TELA_RESULTADO;
        }
    }

    draw_pato(estado, 80, estado->alt - 80, 0.8f, WHITE, 0);
    draw_pato(estado, 130, estado->alt - 40, 0.5f, WHITE, -10);
}

// ========================================================================
// TELA: RESULTADO (feedback apos responder pergunta)
// ========================================================================

// Mostra se o jogador acertou ou errou, com particulas de celebracao
// (se acertou), a resposta correta (se errou) e botao para continuar.
void draw_resultado(Estado *estado) {
    int acertou = (estado->resultado_perg > 0);
    Color cor = acertou ? COR_SUCCESS : COR_DANGER;
    char buffer[256];

    DrawRectangleGradientV(0, 0, estado->larg, estado->alt, COR_BG, COR_SURFACE);

    {
        double agora = GetTime();
        if (acertou && agora - estado->ultima_particula > 0.08) {
            estado->ultima_particula = agora;
            Vector2 pos = { (float)(rand() % estado->larg), -10 };
            Vector2 vel = { -20 + (float)(rand() % 40), 80 + (float)(rand() % 60) };
            Color cores[] = { COR_PRIMARY, COR_WARNING, COR_SUCCESS, COR_INFO };
            particle_emit(&estado->particles, pos, vel, cores[rand() % 4],
                          1.0f + (float)(rand() % 100) / 100.0f, 3 + rand() % 3);
        }
    }
    particle_update_and_draw(&estado->particles, GetFrameTime());

    int cx = estado->larg / 2;
    int cy = estado->alt / 2;

    txt_centro(estado, acertou ? "CORRETA!" : "ERRADA!", cy - 120, FONT_XXXL, cor);

    if (acertou) {
        sprintf(buffer, "Avançou %d casa(s)!", estado->resultado_perg);
        txt_centro(estado, buffer, cy - 50, FONT_MD, COR_TEXT_MUTED);
    } else {
        sprintf(buffer, "Resposta: %c) %s",
                'A' + estado->carta_atual.resposta - 1,
                estado->carta_atual.alternativas[estado->carta_atual.resposta - 1]);
        txt_centro(estado, buffer, cy - 50, FONT_MD, COR_TEXT_MUTED);
    }

    sprintf(buffer, "%s na Casa %d",
            estado->jogadores[estado->jogador_atual].nome,
            estado->jogadores[estado->jogador_atual].posicao);
    txt_centro(estado, buffer, cy + 50, FONT_LG, WHITE);

    Rectangle botao_continuar = { cx - 100, cy + 130, 200, 45 };
    if (botao(botao_continuar, "CONTINUAR", BTN_PRIMARY, &estado->som_clique)) {
        estado->tela = TELA_JOGO;
        avancar_turno(estado);
    }
}

// ========================================================================
// TELA: VENCEDOR (fim de jogo com ranking e confetes)
// ========================================================================

// Exibe o vencedor com titulo pulsante, confetes infinitos, ranking
// dos jogadores ordenado por posicao, e o historico de casas percorridas
// por cada jogador. Botao para voltar ao menu principal.
void draw_vencedor(Estado *estado) {
    char buffer[256];
    int i, j;

    DrawRectangleGradientV(0, 0, estado->larg, estado->alt, COR_BG, COR_SURFACE);

    if (!estado->confete_iniciado) {
        confete_init(&estado->confetes);
        estado->confete_iniciado = 1;
    }
    confete_update_and_draw(&estado->confetes, GetFrameTime());

    int cx = estado->larg / 2;

    Color cor_venc = cor_duck(estado->personagens_escolhidos[estado->jogador_atual]);
    draw_pato(estado, cx + cx * 0.4f, estado->alt * 0.35f, 2.2f, cor_venc, 0);
    draw_pato(estado, cx - cx * 0.4f, estado->alt * 0.38f, 1.8f, cor_venc, 0);
    draw_pato(estado, cx + cx * 0.25f, estado->alt * 0.22f, 1.0f, cor_venc, -15);
    draw_pato(estado, cx - cx * 0.25f, estado->alt * 0.24f, 1.0f, cor_venc, 15);

    {
        float brilho = sinf(GetTime() * 3.0f) * 0.4f + 0.6f;
        int alpha = (int)(brilho * 200);
        txt_draw(estado, "VENCEDOR!", cx - txt_measure(estado, "VENCEDOR!", FONT_XXXL)/2 + 2,
                 52, FONT_XXXL, COR_WARNING);
        txt_draw(estado, "VENCEDOR!", cx - txt_measure(estado, "VENCEDOR!", FONT_XXXL)/2,
                 50, FONT_XXXL, RGBA(255, 200 + (int)(brilho * 55), 50, alpha));
    }

    sprintf(buffer, "%s VENCEU O JOGO!", estado->jogadores[estado->jogador_atual].nome);
    txt_centro(estado, buffer, 110, FONT_XL, COR_SUCCESS);

    txt_centro(estado, "RANKING FINAL:", estado->alt * 0.26f, FONT_LG, COR_PRIMARY);

    tp_player copia[4];
    for (i = 0; i < estado->num_jogadores; i++)
        copia[i] = estado->jogadores[i];

    for (i = 0; i < estado->num_jogadores - 1; i++) {
        for (j = 0; j < estado->num_jogadores - i - 1; j++) {
            if (copia[j].posicao < copia[j + 1].posicao) {
                tp_player temp = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = temp;
            }
        }
    }

    int rank_y = estado->alt * 0.32f;
    int rank_spacing = 35;
    for (i = 0; i < estado->num_jogadores; i++) {
        Color cor = (i == 0) ? COR_WARNING : COR_TEXT;
        sprintf(buffer, "%do: %s (Casa %d)", i + 1, copia[i].nome, copia[i].posicao);
        int tw = txt_measure(estado, buffer, FONT_MD);
        int ry = rank_y + i * rank_spacing;

        if (i == 0) {
            DrawCircle(cx - tw / 2 - 35, ry - 8, 10, COR_WARNING);
            DrawCircle(cx - tw / 2 - 35, ry - 8, 7, RGB(255, 215, 0));
        }

        draw_pato(estado, cx - tw / 2 - 20, ry - 3, 0.5f, WHITE, 0);
        txt_draw(estado, buffer, cx - tw / 2, ry, FONT_MD, cor);
    }

    int caminhos_y = rank_y + estado->num_jogadores * rank_spacing + 60;
    if (caminhos_y > estado->alt - 120) caminhos_y = estado->alt - 120;
    txt_centro(estado, "Caminhos percorridos:", caminhos_y, FONT_SM, COR_PRIMARY_LIGHT);

    int path_max_w = (estado->larg < 700) ? estado->larg - 100 : 500;
    for (i = 0; i < estado->num_jogadores; i++) {
        char caminho[1500] = "";
        for (j = 0; j < copia[i].total_casas_visitadas; j++) {
            char tmp[8];
            sprintf(tmp, "%d", copia[i].historico_casas[j]);
            strcat(caminho, tmp);
            if (j < copia[i].total_casas_visitadas - 1)
                strcat(caminho, " -> ");
        }

        int py = caminhos_y + 26 + i * 40;
        if (py > estado->alt - 60) break;
        sprintf(buffer, "%s:", copia[i].nome);
        Color cor_j = cor_jogador(i);
        int name_w = txt_measure(estado, buffer, FONT_SM);
        int duck_w = 22;
        int gap = 10;
        int total_w = duck_w + gap + name_w + gap + path_max_w;
        int left_x = cx - total_w / 2;

        draw_pato(estado, left_x + duck_w / 2, py - 3, 0.4f, WHITE, 0);
        txt_draw(estado, buffer, left_x + duck_w + gap, py, FONT_SM, cor_j);
        txt_wrap(estado, caminho, left_x + duck_w + gap + name_w + gap, py - 2, path_max_w, FONT_XS, COR_TEXT_MUTED);
    }

    int botao_y = caminhos_y + 26 + estado->num_jogadores * 40 + 15;
    if (botao_y > estado->alt - 65) botao_y = estado->alt - 65;
    Rectangle botao_menu = { cx - 150, (float)botao_y, 300, 55 };
    if (botao(botao_menu, "MENU PRINCIPAL", BTN_PRIMARY, &estado->som_clique)) {
        destruir_tabuleiro(&estado->tabuleiro);
        estado->confete_iniciado = 0;
        particle_init(&estado->particles);
        iniciar_transicao(estado);
        estado->tela = TELA_MENU;
    }
}

// ========================================================================
// TELA: PAUSE (menu suspenso com overlay escuro)
// ========================================================================

// Exibe um card centralizado com opcoes de continuar, voltar ao menu
// principal ou sair do jogo. Fundo escurecido com alpha 180.
void draw_pause(Estado *estado) {
    DrawRectangle(0, 0, estado->larg, estado->alt, RGB(0, 0, 0));

    int cx = estado->larg / 2;
    int cy = estado->alt / 2;

    int card_w = 360;
    if (card_w > estado->larg - 60) card_w = estado->larg - 60;
    int card_h = 380;
    if (card_h > estado->alt - 60) card_h = estado->alt - 60;
    Rectangle card_r = { cx - card_w/2, cy - card_h/2, card_w, card_h };
    DrawRectangleRounded(card_r, RAD_XL, 24, COR_SURFACE);
    DrawRectangleRoundedLines(card_r, RAD_XL, 24, COR_PRIMARY);

    int card_top = cy - card_h / 2;

    txt_centro(estado, "PAUSE", card_top + 35, FONT_XXL, COR_PRIMARY);

    int btn_w = 220;
    if (btn_w > card_w - 40) btn_w = card_w - 40;
    int btn_x = cx - btn_w / 2;
    int btn_start_y = card_top + 90;
    int btn_step = (card_h - 130) / 3;
    if (btn_step > 65) btn_step = 65;
    if (btn_step < 45) btn_step = 45;

    Rectangle botao_continuar = { btn_x, btn_start_y, btn_w, 50 };
    Rectangle botao_menu = { btn_x, btn_start_y + btn_step, btn_w, 50 };
    Rectangle botao_sair = { btn_x, btn_start_y + btn_step * 2, btn_w, 50 };

    if (botao(botao_continuar, "CONTINUAR", BTN_PRIMARY, &estado->som_clique))
        estado->tela = estado->tela_anterior;

    if (botao(botao_menu, "MENU PRINCIPAL", BTN_SECONDARY, &estado->som_clique)) {
        destruir_tabuleiro(&estado->tabuleiro);
        iniciar_transicao(estado);
        estado->tela = TELA_MENU;
    }

    if (botao(botao_sair, "SAIR DO JOGO", BTN_DANGER, &estado->som_clique))
        CloseWindow();
}

// ========================================================================
// TELA: HISTORICO (exibe respostas salvas em CSV)
// ========================================================================

// Le o arquivo historico_respostas.csv (se existir) e exibe seu conteudo
// linha a linha em um card rolavel. Se o arquivo nao existir, mostra
// mensagem "Nenhum historico encontrado".
void draw_historico(Estado *estado) {
    DrawRectangle(0, 0, estado->larg, estado->alt, COR_BG);

    int cx = estado->larg / 2;

    txt_centro(estado, "HISTÓRICO DE RESPOSTAS", 40, FONT_XXL, COR_PRIMARY);

    if (!estado->hist_pronto) {
        estado->hist_buf[0] = '\0';
        FILE *arquivo = fopen("historico_respostas.csv", "r");
        if (arquivo) {
            size_t pos = 0;
            char linha[512];
            while (fgets(linha, sizeof(linha), arquivo) &&
                   pos < sizeof(estado->hist_buf) - 512) {
                size_t len = strlen(linha);
                memcpy(estado->hist_buf + pos, linha, len);
                pos += len;
            }
            estado->hist_buf[pos] = '\0';
            fclose(arquivo);
        }
        estado->hist_pronto = 1;
    }

    if (estado->hist_buf[0]) {
        int y = 110;
        int max_y = estado->alt - 70;
        char *line = estado->hist_buf;
        while (*line && y < max_y) {
            char *nl = strchr(line, '\n');
            int len = nl ? (int)(nl - line) : (int)strlen(line);
            if (len > 0) {
                int cl = (len < 511) ? len : 511;
                char tmp[512];
                memcpy(tmp, line, cl);
                tmp[cl] = '\0';
                txt_centro(estado, tmp, y, FONT_SM, COR_TEXT_MUTED);
                y += 18;
            }
            if (nl) line = nl + 1;
            else break;
        }
    } else {
        txt_centro(estado, "Nenhum histórico encontrado.", estado->alt / 2 - 20, FONT_LG, COR_TEXT_MUTED);
    }

    Rectangle botao_voltar = { cx - 70, estado->alt - 55, 140, 38 };
    if (botao(botao_voltar, "VOLTAR", BTN_PRIMARY, &estado->som_clique)) {
        estado->tela = TELA_MENU;
        estado->hist_pronto = 0;
    }
}

// ========================================================================
// FUNCAO PRINCIPAL DA INTERFACE GRAFICA (loop do jogo)
// ========================================================================

// Inicializa a janela, configura fontes e estado inicial.
// Executa o loop principal enquanto a janela estiver aberta:
//   1. Detecta atalho Ctrl+R para reduced motion
//   2. Desenha a tela correspondente ao estado->tela
//   3. Desenha transicao e toast sobre todas as telas
// Ao finalizar, libera recursos e fecha a janela.
void gui_main(void) {
    Estado estado;
    estado.larg = 1280;
    estado.alt = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(estado.larg, estado.alt, "Quack Jogo de Tabuleiro Educativo");
    SetTargetFPS(60);
    SetWindowMinSize(JANELA_MIN_LARG, JANELA_MIN_ALT);

    InitAudioDevice();
    estado.musica = LoadMusicStream("musica.ogg");
    estado.musica.looping = true;
    SetMusicVolume(estado.musica, 0.4f);
    PlayMusicStream(estado.musica);

    {
        int sample_rate = 44100;
        int duration_frames = (int)(sample_rate * 0.06f);
        Wave wave = { 0 };
        wave.frameCount = duration_frames;
        wave.sampleRate = sample_rate;
        wave.sampleSize = 16;
        wave.channels = 1;
        short *samples = (short *)malloc(duration_frames * sizeof(short));
        for (int s = 0; s < duration_frames; s++) {
            float t = (float)s / sample_rate;
            float env = 1.0f - (t / 0.06f);
            float freq = 800.0f + 400.0f * (1.0f - (float)s / duration_frames);
            float val = sinf(2.0f * PI * freq * t) * env * 0.5f;
            samples[s] = (short)(val * 32767.0f);
        }
        wave.data = samples;
        estado.som_clique = LoadSoundFromWave(wave);
        SetSoundVolume(estado.som_clique, 0.6f);
        free(samples);
    }

    {
        int sample_rate = 44100;
        float dur = 0.7f;
        int duration_frames = (int)(sample_rate * dur);
        Wave wave = { 0 };
        wave.frameCount = duration_frames;
        wave.sampleRate = sample_rate;
        wave.sampleSize = 16;
        wave.channels = 1;
        short *samples = (short *)malloc(duration_frames * sizeof(short));
        for (int s = 0; s < duration_frames; s++) {
            float t = (float)s / sample_rate;
            float env = 1.0f - (t / dur);
            if (env < 0) env = 0;
            float freq = 350.0f - 200.0f * (t / dur);
            float vibrato = sinf(2.0f * PI * 6.0f * t) * 8.0f;
            float saw = 2.0f * fmodf(freq * t + vibrato / sample_rate, 1.0f) - 1.0f;
            float val = saw * env * 0.4f;
            samples[s] = (short)(val * 32767.0f);
        }
        wave.data = samples;
        estado.som_fail = LoadSoundFromWave(wave);
        SetSoundVolume(estado.som_fail, 0.7f);
        free(samples);
    }

    {
        int sample_rate = 44100;
        float dur = 1.2f;
        int duration_frames = (int)(sample_rate * dur);
        Wave wave = { 0 };
        wave.frameCount = duration_frames;
        wave.sampleRate = sample_rate;
        wave.sampleSize = 16;
        wave.channels = 1;
        short *samples = (short *)malloc(duration_frames * sizeof(short));
        for (int s = 0; s < duration_frames; s++) {
            float t = (float)s / sample_rate;
            float env = 1.0f - (t / dur);
            if (env < 0) env = 0;
            float freq1 = 523.25f;
            float freq2 = 659.25f;
            float freq3 = 783.99f;
            float freq = freq1;
            if (t > 0.15f) freq = freq2;
            if (t > 0.3f) freq = freq3;
            if (t > 0.5f) freq = freq1;
            if (t > 0.65f) freq = freq2;
            if (t > 0.8f) freq = freq3;
            float clap = 0.0f;
            for (int c = 0; c < 4; c++) {
                float ct = 0.2f + c * 0.25f;
                float diff = t - ct;
                if (diff >= 0 && diff < 0.04f) {
                    clap += (1.0f - diff / 0.04f) * 0.5f * (float)((rand() % 1000) / 1000.0);
                }
            }
            float melodia = sinf(2.0f * PI * freq * t) * env * 0.3f;
            float val = melodia + clap * env;
            if (val > 1.0f) val = 1.0f;
            if (val < -1.0f) val = -1.0f;
            samples[s] = (short)(val * 32767.0f);
        }
        wave.data = samples;
        estado.som_acerto = LoadSoundFromWave(wave);
        SetSoundVolume(estado.som_acerto, 0.7f);
        free(samples);
    }

    {
        int sample_rate = 44100;
        float dur = 0.3f;
        int duration_frames = (int)(sample_rate * dur);
        Wave wave = { 0 };
        wave.frameCount = duration_frames;
        wave.sampleRate = sample_rate;
        wave.sampleSize = 16;
        wave.channels = 1;
        short *samples = (short *)malloc(duration_frames * sizeof(short));
        for (int s = 0; s < duration_frames; s++) {
            float t = (float)s / sample_rate;
            float env = 1.0f - (t / dur);
            if (env < 0) env = 0;
            float freq = (t < 0.15f) ? 880.0f : 1100.0f;
            float val = sinf(2.0f * PI * freq * t) * env * 0.5f;
            samples[s] = (short)(val * 32767.0f);
        }
        wave.data = samples;
        estado.som_alerta = LoadSoundFromWave(wave);
        SetSoundVolume(estado.som_alerta, 0.6f);
        free(samples);
    }

    estado.textura_pato = LoadTexture("pato.png");
    SetTextureFilter(estado.textura_pato, TEXTURE_FILTER_TRILINEAR);

    {
        int codepoints[256];
        int count = 0;
        for (int c = 32; c <= 126; c++) codepoints[count++] = c;
    int extras[] = {
        192, 193, 194, 195, 199,
        201, 202, 205, 211, 212,
        213, 218, 220,
        224, 225, 226, 227, 231,
        233, 234, 237, 243, 244,
        245, 250, 252
    };
    int extras_count = 25;
    for (int e = 0; e < extras_count; e++) {
            int found = 0;
            for (int j = 0; j < count; j++) if (codepoints[j] == extras[e]) { found = 1; break; }
            if (!found) codepoints[count++] = extras[e];
        }
        estado.font_corpo = LoadFontEx("ComicNeue-Regular.ttf", 256, codepoints, count);
        SetTextureFilter(estado.font_corpo.texture, TEXTURE_FILTER_TRILINEAR);
    }
    estado.font_carregada = 1;

    estado.tela = TELA_MENU;
    estado.tela_anterior = TELA_MENU;
    estado.hist_pronto = 0;
    estado.transicao_ativa = 0;
    estado.ultima_particula = 0;
    estado.confete_iniciado = 0;
    estado.reduced_motion = 0;
    estado.dado_valor_exibido = 0;
    estado.dado_tween.active = 0;

    particle_init(&estado.particles);
    estado.toast.ativo = 0;

    while (!WindowShouldClose()) {

        UpdateMusicStream(estado.musica);

        {
            float vol;
            switch (estado.tela) {
                case TELA_MENU:
                case TELA_NUM_JOG:
                case TELA_PERSONAGEM:
                    vol = 0.18f; break;
                default:
                    vol = 0.4f; break;
            }
            SetMusicVolume(estado.musica, vol);
        }

        estado.larg = GetScreenWidth();
        estado.alt = GetScreenHeight();

        if (IsKeyPressed(KEY_R) && IsKeyDown(KEY_LEFT_CONTROL)) {
            estado.reduced_motion = !estado.reduced_motion;
        }
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
            estado.larg = GetScreenWidth();
            estado.alt = GetScreenHeight();
        }

        BeginDrawing();

        if (estado.tela == TELA_JOGO) {
            DrawRectangle(0, 0, estado.larg, estado.alt, RGB(40, 18, 5));
        } else {
            ClearBackground(COR_BG);
        }

        float dt = GetFrameTime();

        switch (estado.tela) {
            case TELA_MENU:       draw_menu(&estado);       break;
            case TELA_NUM_JOG:    draw_num_jog(&estado);    break;
            case TELA_PERSONAGEM: draw_personagem(&estado); break;
            case TELA_JOGO:       draw_jogo(&estado);       break;
            case TELA_PERGUNTA:   draw_pergunta(&estado);   break;
            case TELA_RESULTADO:  draw_resultado(&estado);  break;
            case TELA_VENCEDOR:   draw_vencedor(&estado);   break;
            case TELA_PAUSE:      draw_pause(&estado);      break;
            case TELA_HISTORICO:  draw_historico(&estado);  break;
        }

        draw_transicao(&estado, dt);
        toast_update_and_draw(&estado.toast, dt);

        if (estado.tela == TELA_JOGO) {
            txt_draw(&estado, "Ctrl+R: reduzir animação",
                     10, estado.alt - 20, FONT_XS, RGBA(255, 255, 255, 35));
        }

        EndDrawing();
    }

    UnloadMusicStream(estado.musica);
    UnloadSound(estado.som_clique);
    UnloadSound(estado.som_fail);
    UnloadSound(estado.som_acerto);
    UnloadSound(estado.som_alerta);
    UnloadTexture(estado.textura_pato);
    CloseAudioDevice();

    if (estado.font_carregada) {
        UnloadFont(estado.font_titulo);
        UnloadFont(estado.font_corpo);
    }

    CloseWindow();
}



// ========================================================================
// DESENHO DO PATO MASCOTE (2D) — VERSÃO TEXTURA
// ========================================================================

// Desenha o pato a partir da textura pato.png com suporte a escala,
// rotacao e tint (cor). A sombra e desenhada separadamente abaixo.
// A animacao de balanco (sine wave no eixo Y) e mantida.
void draw_pato(Estado *estado, int x, int y_base, float escala, Color corpo, int angulo) {
    int sy = y_base + (int)(20 * escala);
    DrawEllipse(x, sy, 18 * escala, 5 * escala, RGBA(0, 0, 0, 25));
    DrawEllipse(x, sy, 12 * escala, 3 * escala, RGBA(0, 0, 0, 18));

    float balanco_y = sinf(GetTime() * 2.5f) * 1.5f * escala;
    int y = y_base + (int)roundf(balanco_y);

    // Fator de escala: procduck ~55px / textura 889px
    float fator = 55.0f / estado->textura_pato.width;
    float s = escala * fator;
    float sw = estado->textura_pato.width * s;
    float sh = estado->textura_pato.height * s;

    Vector2 origem = { sw / 2.0f, sh / 2.0f };
    DrawTexturePro(
        estado->textura_pato,
        (Rectangle){ 0, 0, (float)estado->textura_pato.width, (float)estado->textura_pato.height },
        (Rectangle){ (float)x, (float)y, sw, sh },
        origem, (float)angulo, corpo
    );
}

// ========================================================================
// FUNCOES DE LOGICA DO JOGO
// ========================================================================

// Reinicia o estado para uma nova partida: reseta flags, recria o
// tabuleiro, recarrega o banco de cartas e inicializa as pilhas de perguntas.
void reiniciar(Estado *estado) {
    int i;
    estado->partida_ativa     = 1;
    estado->turnos            = 0;
    estado->rodada            = 0;
    estado->aguardando_dado   = 1;
    estado->ultimo_dado       = 0;
    estado->resultado_perg    = 0;
    criar_tabuleiro(&estado->tabuleiro);
    montar_banco_cartas(estado->banco);
    exportar_perguntas_csv(estado->banco, 36);
    carregar_perguntas(estado->pilhas, estado->banco, 36);
    for (i = 0; i < 4; i++)
        estado->personagens_escolhidos[i] = 0;
    estado->personagem_idx = 0;
    particle_init(&estado->particles);
    estado->confete_iniciado = 0;
}

// Inicia a partida apos a selecao de personagens: inicializa jogadores,
// atribui nomes baseados nos personagens escolhidos, configura a fila
// de turnos e muda para a tela de jogo.
void comecar(Estado *estado) {
    int i;
    inicializar_jogadores(estado->jogadores, estado->num_jogadores);
    for (i = 0; i < estado->num_jogadores; i++) {
        int id_personagem = estado->personagens_escolhidos[i];
        char buffer[50];
        if (id_personagem >= 1 && id_personagem <= 4)
            copiar_texto(buffer, 50, nome_personagem(id_personagem));
        else
            copiar_texto(buffer, 50, "Pato");
        copiar_texto(estado->jogadores[i].nome, sizeof(estado->jogadores[i].nome), buffer);
    }
    colocar_jogadores_na_fila(estado->jogadores, estado->num_jogadores, &estado->fila_turnos);
    estado->jogador_atual = 0;
    estado->tela = TELA_JOGO;
}

// Avanca o turno: remove o ID do jogador atual da fila circular e o
// reinsere ao final, atualizando o indice do jogador da vez.
void avancar_turno(Estado *estado) {
    tp_item id;
    if (!remove_fila(&estado->fila_turnos, &id)) return;
    estado->jogador_atual = id - 1;
    insere_fila(&estado->fila_turnos, id);
    estado->turnos++;
    estado->rodada = ((estado->turnos - 1) / estado->num_jogadores) + 1;
    estado->aguardando_dado = 1;
}

// Verifica se um ID de personagem (1-4) ja foi escolhido por algum jogador.
int disponivel(Estado *estado, int id) {
    int i;
    for (i = 0; i < estado->personagem_idx; i++)
        if (estado->personagens_escolhidos[i] == id) return 0;
    return 1;
}

// ========================================================================
// TELA: MENU PRINCIPAL
// ========================================================================

// Tela inicial do jogo com titulo "QUACK BOARDGAME" com efeito glow,
// patos decorativos, botoes de Novo Jogo, Historico e Sair, e fundo
// com pontos animados (estrelas).
void draw_menu(Estado *estado) {
    DrawRectangleGradientV(0, 0, estado->larg, estado->alt, COR_BG, COR_SURFACE_ALT);

    int cx = estado->larg / 2;
    int cy = estado->alt / 2;

    draw_pato(estado, cx - cx * 0.65f, cy - cy * 0.15f, 2.8f, WHITE, -5);
    draw_pato(estado, cx + cx * 0.65f, cy + cy * 0.05f, 2.2f, WHITE, 5);
    draw_pato(estado, cx - cx * 0.5f, cy - cy * 0.45f, 1.0f, WHITE, 20);
    draw_pato(estado, cx + cx * 0.5f, cy - cy * 0.45f, 0.8f, WHITE, -25);

    {
        float brilho = sinf(GetTime() * 2.0f) * 0.3f + 0.7f;
        int alpha = (int)(brilho * 100) + 60;

        txt_draw(estado, "QUACK", cx - txt_measure(estado, "QUACK", FONT_HERO)/2 + 4, cy - cy * 0.42f + 4, FONT_HERO, COR_BG);
        txt_draw(estado, "QUACK", cx - txt_measure(estado, "QUACK", FONT_HERO)/2 + 2, cy - cy * 0.42f + 2, FONT_HERO, COR_PRIMARY_DARK);
        txt_draw(estado, "QUACK", cx - txt_measure(estado, "QUACK", FONT_HERO)/2, cy - cy * 0.42f, FONT_HERO, COR_PRIMARY);
        Color glow = RGBA(255, 180 + (int)(brilho * 60), 50, alpha);
        txt_draw(estado, "QUACK", cx - txt_measure(estado, "QUACK", FONT_HERO)/2 + 6, cy - cy * 0.42f + 6, FONT_HERO, glow);
        txt_draw(estado, "QUACK", cx - txt_measure(estado, "QUACK", FONT_HERO)/2 + 8, cy - cy * 0.42f + 8, FONT_HERO, RGBA(255, 102, 0, alpha/3));
    }

    txt_draw(estado, "BOARDGAME", cx - txt_measure(estado, "BOARDGAME", FONT_XXL)/2, cy - cy * 0.20f, FONT_XXL, COR_TEXT);

    {
        int btn_w = 240;
        if (btn_w > estado->larg - 60) btn_w = estado->larg - 60;
        int btn_x = cx - btn_w / 2;
        int btn_y_start = (int)(cy - cy * 0.03f) + 30;
        int btn_step = 65;

        Rectangle botao_jogar = { btn_x, btn_y_start, btn_w, 50 };
        Rectangle botao_historico = { btn_x, btn_y_start + btn_step, btn_w, 50 };
        Rectangle botao_sair = { btn_x, btn_y_start + btn_step * 2, btn_w, 50 };

        if (botao(botao_jogar, "  NOVO JOGO", BTN_PRIMARY, &estado->som_clique)) {
            reiniciar(estado);
            iniciar_transicao(estado);
            estado->tela = TELA_NUM_JOG;
        }
        if (botao(botao_historico, "  HISTÓRICO", BTN_SECONDARY, &estado->som_clique)) {
            estado->hist_pronto = 0;
            iniciar_transicao(estado);
            estado->tela = TELA_HISTORICO;
        }
        if (botao(botao_sair, "  SAIR", BTN_DANGER, &estado->som_clique)) {
            CloseWindow();
        }
    }

    {
        char *creditos = "@clubedeprogramacaocimatec";
        int cw = txt_measure(estado, creditos, FONT_SM);
        txt_draw(estado, creditos, estado->larg - cw - 15, estado->alt - 30, FONT_SM, COR_TEXT_DIM);
    }
}

// ========================================================================
// TELA: NÚMERO DE JOGADORES
// ========================================================================

// Exibe 3 botoes para escolher entre 2, 3 ou 4 jogadores. Ao selecionar,
// avanca para a tela de selecao de personagem.
void draw_num_jog(Estado *estado) {
    DrawRectangleGradientV(0, 0, estado->larg, estado->alt, COR_BG, COR_SURFACE);

    int cx = estado->larg / 2;
    int cy = estado->alt / 2;

    txt_centro(estado, "NÚMERO DE JOGADORES", cy - 140, FONT_XXL, COR_PRIMARY);
    txt_centro(estado, "Escolha quantos vão jogar", cy - 95, FONT_MD, COR_TEXT_MUTED);

    int card_w = 680;
    if (card_w > estado->larg - 60) card_w = estado->larg - 60;
    int card_h = 160;
    Rectangle card_r = { cx - card_w/2, cy - 30, card_w, card_h };
    DrawShadow(card_r, RAD_LG, 24, 8, 50, 5);
    DrawRectangleRounded(card_r, RAD_LG, 24, COR_SURFACE);
    DrawRectangleRoundedLines(card_r, RAD_LG, 24, COR_PRIMARY_MUTED);

    int btn_w = 190;
    if (3 * btn_w + 2 * 20 > card_w - 40) btn_w = (card_w - 80 - 2 * 20) / 3;
    int btn_gap = 20;
    int total_btn_w = 3 * btn_w + 2 * btn_gap;
    int btn_start_x = cx - total_btn_w / 2;
    int btn_y = cy - 30 + 48;

    Rectangle botao_2 = { btn_start_x, btn_y, btn_w, 65 };
    Rectangle botao_3 = { btn_start_x + btn_w + btn_gap, btn_y, btn_w, 65 };
    Rectangle botao_4 = { btn_start_x + 2 * (btn_w + btn_gap), btn_y, btn_w, 65 };

    if (botao(botao_2, "2 JOGADORES", BTN_PRIMARY, &estado->som_clique)) {
        estado->num_jogadores = 2;
        estado->personagem_idx = 0;
        iniciar_transicao(estado);
        estado->tela = TELA_PERSONAGEM;
    }
    if (botao(botao_3, "3 JOGADORES", BTN_PRIMARY, &estado->som_clique)) {
        estado->num_jogadores = 3;
        estado->personagem_idx = 0;
        iniciar_transicao(estado);
        estado->tela = TELA_PERSONAGEM;
    }
    if (botao(botao_4, "4 JOGADORES", BTN_PRIMARY, &estado->som_clique)) {
        estado->num_jogadores = 4;
        estado->personagem_idx = 0;
        iniciar_transicao(estado);
        estado->tela = TELA_PERSONAGEM;
    }

}

// ========================================================================
// TELA: SELEÇÃO DE PERSONAGEM
// ========================================================================

// Exibe 4 cards de personagem (2x2 grid). Cada jogador escolhe um
// personagem por vez. Personagens ja escolhidos aparecem com check.
// O progresso da selecao e mostrado por indicadores no topo.
void draw_personagem(Estado *estado) {
    char buffer[64];
    int ids[4] = { 1, 2, 3, 4 };
    int i;

    DrawRectangleGradientV(0, 0, estado->larg, estado->alt, COR_BG, COR_SURFACE);

    int cx = estado->larg / 2;

    txt_centro(estado, "SELEÇÃO DE PERSONAGEM", 30, FONT_XXL, COR_PRIMARY);
    sprintf(buffer, "Jogador %d, escolha seu personagem", estado->personagem_idx + 1);
    txt_centro(estado, buffer, 80, FONT_LG, COR_TEXT_MUTED);

    {
        int i;
        float step_w = 280.0f / estado->num_jogadores;
        for (i = 0; i < estado->num_jogadores; i++) {
            float cx_p = cx - 140 + step_w * (i + 0.5f);
            int preenchido = i < estado->personagem_idx;
            int ativo = i == estado->personagem_idx;
            Color c = ativo ? COR_PRIMARY : (preenchido ? COR_SUCCESS : COR_BORDER);
            DrawCircle((int)cx_p, 118, ativo ? 9 : 7, c);
            if (ativo) {
                float t = sinf(GetTime() * 3) * 0.3f + 0.7f;
                DrawCircleLines((int)cx_p, 118, 13, RGBA(c.r, c.g, c.b, (int)(t * 150)));
            }
        }
    }

    int card_w = 260;
    int card_h = 250;
    int gap_x = 20;
    int gap_y = 20;
    int grid_w = 2 * card_w + gap_x;
    int grid_h = 2 * card_h + gap_y;

    if (grid_w > estado->larg - 60) {
        card_w = (estado->larg - 60 - gap_x) / 2;
        grid_w = 2 * card_w + gap_x;
    }
    if (grid_h > estado->alt - 185) {
        card_h = (estado->alt - 185 - gap_y) / 2;
        if (card_h < 140) card_h = 140;
        grid_h = 2 * card_h + gap_y;
    }

    int grid_x_start = cx - grid_w / 2;
    int grid_y_start = 160;
    int bottom_margin = estado->alt - grid_y_start - grid_h;
    if (bottom_margin < 20) grid_y_start = 160 - ((-bottom_margin + 20) / 2);
    if (grid_y_start < 145) grid_y_start = 145;

    for (i = 0; i < 4; i++) {
        int id = ids[i];
        int disponivel_local = disponivel(estado, id);
        int col = i % 2;
        int row = i / 2;
        int card_x = grid_x_start + col * (card_w + gap_x);
        int card_y = grid_y_start + row * (card_h + gap_y);
        Rectangle r = { (float)card_x, (float)card_y, card_w, card_h };

        Color fundo_card = disponivel_local ?
            RGBA(35, 35, 40, 220) : RGBA(25, 25, 28, 180);

        Color borda = COR_PRIMARY_MUTED;
        if (disponivel_local) {
            if (mouse_em(r)) {
                int pulso = (int)(sinf(GetTime() * 4.0f) * 30 + 60);
                borda = RGBA(255, 150 + pulso, 0, 255);
            } else {
                borda = COR_PRIMARY;
            }
        } else {
            borda = COR_BORDER;
        }

        DrawShadow(r, RAD_LG, 24, 4, 40, 4);
        DrawRectangleRounded(r, RAD_LG, 24, fundo_card);
        DrawRectangleRoundedLines(r, RAD_LG, 24, borda);

        float pato_escala = card_h < 200 ? 1.0f : 1.4f;
        draw_pato(estado, card_x + card_w/2, card_y + card_h * 0.32f, pato_escala, cor_duck(id), 0);

        {
            int ny = card_y + card_h - 60;
            char *n = nome_personagem(id);
            int nl = txt_measure(estado, n, FONT_LG);
            int nx = card_x + card_w/2 - nl / 2;
            Rectangle nr = { (float)nx - 10, (float)ny - 4, nl + 20, 30 };
            DrawRectangleRounded(nr, RAD_SM, 16,
                disponivel_local ? RGBA(0, 0, 0, 140) : RGBA(0, 0, 0, 60));
            txt_draw(estado, n, nx, ny, FONT_LG,
                disponivel_local ? COR_TEXT : COR_TEXT_MUTED);
        }

        if (disponivel_local && mouse_em(r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            estado->personagens_escolhidos[estado->personagem_idx] = id;
            estado->personagem_idx++;
            if (estado->personagem_idx >= estado->num_jogadores) {
                iniciar_transicao(estado);
                comecar(estado);
            }
        }

        if (!disponivel_local) {
            DrawCircle(card_x + card_w - 30, card_y + 20, 16, COR_SUCCESS);
            txt_draw(estado, "OK", card_x + card_w - 38, card_y + 16, FONT_SM, WHITE);
        }
    }
}

// ========================================================================
// TABULEIRO 2D
// ========================================================================

// Renderiza o tabuleiro 2D com 30 casas em formato serpentina (3 fileiras
// de 10). Cada casa e um quadrado arredondado colorido conforme o tipo:
// normal=marrom, pergunta=laranja, bonus=verde, penalidade=vermelho.
// Tokens dos jogadores sao circulos coloridos sobre a casa atual.
// Um destaque pulsante indica a casa do jogador da vez.
void calc_pos_casa(int indice, int *x, int *y, int board_x, int board_y, int board_w, int board_h) {
    int fileira = indice / TAB_COLS;
    int col = indice % TAB_COLS;
    if (fileira == 1) col = TAB_COLS - 1 - col;

    float cell_w = (float)(board_w - (TAB_COLS - 1) * TAB_GAP) / TAB_COLS;
    if (cell_w > TAB_CELL) cell_w = TAB_CELL;
    float step = cell_w + TAB_GAP;
    float espaco_y = (board_h - TAB_ROWS * cell_w) / (TAB_ROWS - 1);
    if (espaco_y < TAB_ESPACO_Y * 0.5f) espaco_y = TAB_ESPACO_Y * 0.5f;
    if (espaco_y > TAB_ESPACO_Y) espaco_y = TAB_ESPACO_Y;

    *x = board_x + (int)(col * step);
    *y = board_y + (int)(fileira * (cell_w + espaco_y));
}

void draw_board(Estado *estado) {
    int i, j;
    int cx[QTD_CASAS], cy[QTD_CASAS];

    int painel_w = 235;
    int margin = 30;
    int board_x = margin;
    int board_y = 30;
    int board_w = estado->larg - painel_w - margin * 2;
    int board_h = estado->alt - 60;

    float cell_w = (float)(board_w - (TAB_COLS - 1) * TAB_GAP) / TAB_COLS;
    if (cell_w > TAB_CELL) cell_w = TAB_CELL;
    float step = cell_w + TAB_GAP;

    int total_grid_w = (int)(TAB_COLS * step - TAB_GAP);
    int grid_x_center = board_x + board_w / 2;
    int actual_board_x = grid_x_center - total_grid_w / 2;

    float espaco_y = (board_h - TAB_ROWS * cell_w) / (TAB_ROWS - 1);
    if (espaco_y < 80) espaco_y = 80;
    if (espaco_y > TAB_ESPACO_Y) espaco_y = TAB_ESPACO_Y;

    int total_grid_h = (int)(TAB_ROWS * cell_w + (TAB_ROWS - 1) * espaco_y);
    int grid_y_center = board_y + board_h / 2;
    int actual_board_y = grid_y_center - total_grid_h / 2;

    for (i = 0; i < QTD_CASAS; i++)
        calc_pos_casa(i, &cx[i], &cy[i], actual_board_x, actual_board_y, board_w, board_h);

    // Fundo do tabuleiro (removido - apenas casas flutuam no fundo laranja)
    // Caminho entre casas
    for (i = 0; i < QTD_CASAS - 1; i++) {
        int x1 = cx[i] + TAB_CELL / 2;
        int y1 = cy[i] + TAB_CELL / 2;
        int x2 = cx[i + 1] + TAB_CELL / 2;
        int y2 = cy[i + 1] + TAB_CELL / 2;
        Color path_cor = RGBA(100, 100, 110, 100);
        float path_width = 4.0f;
        if (i == 9 || i == 19) {
            DrawLineEx((Vector2){ (float)x1, (float)y1 },
                       (Vector2){ (float)(x1), (float)(y1 + (y2 - y1) / 2) },
                       path_width, path_cor);
            DrawLineEx((Vector2){ (float)(x1), (float)(y1 + (y2 - y1) / 2) },
                       (Vector2){ (float)(x2), (float)(y2) },
                       path_width, path_cor);
        } else {
            DrawLineEx((Vector2){ (float)x1, (float)y1 },
                       (Vector2){ (float)x2, (float)y2 },
                       path_width, path_cor);
        }
    }

    // Casas
    for (i = 0; i < QTD_CASAS; i++) {
        tp_casa *c = obter_casa(&estado->tabuleiro, i + 1);
        Color cor = COR_CASA_NORMAL;
        if (c) {
            if (c->tipo == 1)      cor = COR_CASA_PERG;
            else if (c->tipo == 2) cor = COR_CASA_BONUS;
            else if (c->tipo == 3) cor = COR_CASA_PENAL;
        }

        Rectangle casa_r = { (float)cx[i], (float)cy[i], TAB_CELL, TAB_CELL };
        Color cor_escura = cor_escurecer(cor, 0.6f);

        DrawRectangleRounded(casa_r, RAD_SM, 16, cor);
        DrawRectangleRoundedLines(casa_r, RAD_SM, 16, cor_escura);

        // Indicador de tipo no canto superior esquerdo
        {
            int icone_x = cx[i] + 6;
            int icone_y = cy[i] + 5;
            if (c && c->tipo == 1) {
                txt_draw(estado, "?", icone_x + 1, icone_y + 1, FONT_XS, BLACK);
            } else if (c && c->tipo == 2) {
                txt_draw(estado, "+", icone_x + 1, icone_y + 1, FONT_XS, BLACK);
            } else if (c && c->tipo == 3) {
                txt_draw(estado, "-", icone_x + 1, icone_y + 1, FONT_XS, BLACK);
            }
        }

        // Numero da casa
        char numero[4];
        sprintf(numero, "%d", i + 1);
        int num_x = cx[i] + TAB_CELL / 2 - txt_measure(estado, numero, FONT_SM) / 2;
        int num_y = cy[i] + TAB_CELL / 2 - FONT_SM / 2;
        txt_draw(estado, numero, num_x, num_y, FONT_SM, WHITE);

        // Badge INICIO / FIM
        if (i == 0) {
            int bx = cx[i] + TAB_CELL / 2 - txt_measure(estado, "INICIO", FONT_XS) / 2;
            txt_draw(estado, "INICIO", bx, cy[i] + TAB_CELL + 2, FONT_XS, COR_SUCCESS);
        } else if (i == QTD_CASAS - 1) {
            int bx = cx[i] + TAB_CELL / 2 - txt_measure(estado, "FIM", FONT_XS) / 2;
            txt_draw(estado, "FIM", bx, cy[i] + TAB_CELL + 2, FONT_XS, COR_WARNING);
        }
    }

    // Tokens dos jogadores
    for (j = 0; j < estado->num_jogadores; j++) {
        int casa_idx = estado->jogadores[j].posicao - 1;
        if (casa_idx < 0 || casa_idx >= QTD_CASAS) continue;
        float offset = 0;
        int k;
        for (k = 0; k < j; k++) {
            if (estado->jogadores[k].posicao == estado->jogadores[j].posicao)
                offset += 18;
        }
        int tok_x = cx[casa_idx] + TAB_CELL - 12 + (int)offset;
        int tok_y = cy[casa_idx] + TAB_CELL - 12;
        Color cor_jog = cor_jogador(j);
        DrawCircle(tok_x, tok_y, 11, RGBA(0, 0, 0, 30));
        DrawCircle(tok_x, tok_y, 10, cor_jog);
        DrawCircleLines(tok_x, tok_y, 10, cor_clarear(cor_jog, 50));
        char buf[2];
        sprintf(buf, "%d", j + 1);
        txt_draw(estado, buf, tok_x - txt_measure(estado, buf, FONT_XS) / 2,
                 tok_y - FONT_XS / 2, FONT_XS, WHITE);
    }

    // Destaque pulsante na casa do jogador atual
    int casa_atual = estado->jogadores[estado->jogador_atual].posicao - 1;
    if (casa_atual >= 0 && casa_atual < QTD_CASAS) {
        float pulse = sinf(GetTime() * 3.0f) * 0.3f + 0.7f;
        int alpha = (int)(pulse * 120);
        Rectangle hr = {
            (float)cx[casa_atual] - 3, (float)cy[casa_atual] - 3,
            TAB_CELL + 6, TAB_CELL + 6
        };
        DrawRectangleRoundedLines(hr, RAD_SM, 16,
            RGBA(255, 255, 100, alpha));
    }

    // Unidade labels entre as fileiras no centro do tabuleiro
    {
        char *unidades[] = { "Unidade 1", "Unidade 2", "Unidade 3" };
        int ui;
        for (ui = 0; ui < TAB_ROWS; ui++) {
            int row_first = ui * TAB_COLS;
            int row_last = (ui == 1) ? (ui * TAB_COLS) : (ui * TAB_COLS + TAB_COLS - 1);
            if (ui == 1) row_last = ui * TAB_COLS;
            int ref_idx = ui * TAB_COLS;
            int uy = cy[ref_idx] - 25;
            int ux = actual_board_x + total_grid_w / 2 - txt_measure(estado, unidades[ui], FONT_MD) / 2;
            txt_draw(estado, unidades[ui], ux, uy, FONT_MD, COR_TEXT_DIM);
        }
    }
}

// ========================================================================
// DADO (desenho 2D de um dado de 6 faces)
// ========================================================================

// Desenha um dado quadrado 2D com cantos arredondados, cor de fundo
// creme e pontos pretos organizados conforme o valor (1 a 6).
void draw_dado(int centro_x, int centro_y, int tamanho, int valor) {
    Rectangle r = { centro_x - tamanho/2, centro_y - tamanho/2, tamanho, tamanho };
    DrawShadow(r, 0.15f, 24, 5, 45, 4);
    DrawRectangleRounded(r, 0.15f, 24, RGBA(245, 240, 235, 255));
    DrawRectangleRoundedLines(r, 0.15f, 24, RGBA(120, 115, 110, 255));

    int ponto_diam = tamanho / 7;
    if (ponto_diam < 2) ponto_diam = 2;

    Color pto = RGBA(40, 40, 45, 255);
    int quarto = tamanho / 4;

    switch (valor) {
        case 1: DrawCircle(centro_x, centro_y, ponto_diam, pto); break;
        case 2:
            DrawCircle(centro_x - quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y + quarto, ponto_diam, pto);
            break;
        case 3:
            DrawCircle(centro_x - quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x, centro_y, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y + quarto, ponto_diam, pto);
            break;
        case 4:
            DrawCircle(centro_x - quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x - quarto, centro_y + quarto, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y + quarto, ponto_diam, pto);
            break;
        case 5:
            DrawCircle(centro_x - quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x, centro_y, ponto_diam, pto);
            DrawCircle(centro_x - quarto, centro_y + quarto, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y + quarto, ponto_diam, pto);
            break;
        case 6:
            DrawCircle(centro_x - quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y - quarto, ponto_diam, pto);
            DrawCircle(centro_x - quarto, centro_y, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y, ponto_diam, pto);
            DrawCircle(centro_x - quarto, centro_y + quarto, ponto_diam, pto);
            DrawCircle(centro_x + quarto, centro_y + quarto, ponto_diam, pto);
            break;
    }
}


