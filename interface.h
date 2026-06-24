#ifndef INTERFACE_H
#define INTERFACE_H

/*
 * interface.h - Interface grafica do Quack Boardgame
 *
 * Implementa toda a parte visual do jogo usando Raylib.
 * O estado do jogo e mantido em uma unica struct (Estado) alocada
 * na funcao main e passada como ponteiro para todas as funcoes que
 * precisam acessa-lo.
 *
 * Nao ha variaveis globais, nem uso das palavras-chave static ou const.
 * Todas as constantes de cor sao definidas como macros do pre-processador.
 */

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dados.h"        /* tp_carta, tp_player, rolar_dado, etc */
#include "tabuleiro.h"    /* tp_tabuleiro, tp_casa, criar_tabuleiro */
#include "estatisticas.h" /* salvar_historico_resposta, exportar_perguntas_csv */

/* ======================================================================== */
/* MACROS AUXILIARES                                                        */
/* ======================================================================== */

#define LARG      1280   /* Largura da janela */
#define ALT       800    /* Altura da janela  */
#define QTD_CASAS 30     /* Total de casas do tabuleiro */

/* Macro para criar uma Color com valores RGBA (0-255) */
#define RGBA(r,g,b,a) ((Color){(r),(g),(b),(a)})
#define RGB(r,g,b)    RGBA(r,g,b,255)

/* ======================================================================== */
/* PALETA DE CORES (definida como macros para evitar variaveis globais)     */
/* ======================================================================== */

#define COR_LARANJA    RGB(255, 102, 0)
#define COR_LARANJA_C  RGB(255, 128, 0)
#define COR_LARANJA_E  RGB(255, 153, 51)
#define COR_LARANJA_D  RGB(200, 80, 0)
#define COR_LARANJA_M  RGB(204, 82, 0)
#define COR_LARANJA_G  RGB(150, 60, 0)

#define COR_PRETO      RGB(18, 18, 20)
#define COR_PRETO_2    RGB(25, 25, 28)
#define COR_PRETO_3    RGB(35, 35, 40)
#define COR_PRETO_C    RGB(40, 40, 45)
#define COR_CINZA_ESC  RGB(60, 60, 65)
#define COR_BRANCO_S   RGB(220, 220, 225)
#define COR_DOURADO    RGB(255, 200, 50)

#define COR_VERDE      RGB(50, 205, 50)
#define COR_VERMELHO   RGB(220, 50, 50)

/* Cores das casas do tabuleiro conforme o tipo */
#define COR_CASA_NORMAL RGB(70, 63, 50)
#define COR_CASA_PERG   RGB(255, 140, 0)
#define COR_CASA_BONUS  RGB(0, 180, 60)
#define COR_CASA_PENAL  RGB(200, 40, 40)

/* Cores de identificacao dos jogadores no placar */
#define COR_JOG_1 RGB(255, 140, 50)
#define COR_JOG_2 RGB(255, 200, 50)
#define COR_JOG_3 RGB(80, 200, 255)
#define COR_JOG_4 RGB(255, 100, 100)

/* ======================================================================== */
/* COR DOS PERSONAGENS                                                      */
/* ======================================================================== */

/*
 * Retorna a cor principal do corpo de cada personagem.
 * 1 - Patolino    (preto)
 * 2 - Pato Donald (branco)
 * 3 - Tio Patinhas (dourado)
 * 4 - Pato        (marrom)
 */
Color cor_personagem(int id) {
    switch (id) {
        case 1: return RGB(35, 35, 40);
        case 2: return RGB(230, 225, 230);
        case 3: return RGB(210, 175, 30);
        case 4: return RGB(150, 100, 50);
        default: return RGB(100, 100, 100);
    }
}

/*
 * Retorna o nome do personagem com base no ID numerico.
 */
char* nome_pers(int id) {
    if (id == 1) return "Patolino";
    if (id == 2) return "Pato Donald";
    if (id == 3) return "Tio Patinhas";
    if (id == 4) return "Pato";
    return "?";
}

/* ======================================================================== */
/* ENUMERACAO DE TELAS                                                      */
/* ======================================================================== */

typedef enum {
    TELA_MENU,        /* Menu principal                                      */
    TELA_NUM_JOG,     /* Selecao do numero de jogadores                      */
    TELA_PERSONAGEM,  /* Selecao de personagem por cada jogador              */
    TELA_JOGO,        /* Tela principal do jogo com tabuleiro 3D             */
    TELA_PERGUNTA,    /* Exibicao da carta de pergunta                       */
    TELA_RESULTADO,   /* Mostra se acertou ou errou a pergunta               */
    TELA_VENCEDOR,    /* Tela de vitoria com ranking e caminhos              */
    TELA_PAUSE,       /* Menu de pausa                                       */
    TELA_HISTORICO    /* Exibe o arquivo CSV com historico de respostas      */
} Tela;

/* ======================================================================== */
/* ESTRUTURA PRINCIPAL DE ESTADO                                            */
/* ======================================================================== */

/*
 * Estado - unifica todos os dados necessarios para o funcionamento
 * do jogo. Uma unica instancia e criada em gui_main() e passada
 * como ponteiro para todas as funcoes que precisam modifica-lo.
 */
typedef struct {
    /* Jogadores da partida atual (max 4) */
    tp_player jogadores[4];

    /* Fila circular que controla a ordem dos turnos (FIFO) */
    tp_fila fila_turnos;

    /* Pilhas de perguntas organizadas por unidade e dificuldade [3][3] */
    tp_pilha pilhas[3][3];

    /* Banco fixo com 36 cartas de perguntas */
    tp_carta banco[36];

    /* Tabuleiro como lista duplamente encadeada de 30 casas */
    tp_tabuleiro tabuleiro;

    /* Numero de jogadores na partida (2 a 4) */
    int num_jogadores;

    /* Indice do jogador que esta jogando agora (0..num_jogadores-1) */
    int jogador_atual;

    /* Rodada atual (incrementada a cada ciclo completo de jogadores) */
    int rodada;

    /* Contador total de turnos ja executados */
    int turnos;

    /* Flag booleana: 1 enquanto a partida estiver em andamento */
    int partida_ativa;

    /* Ultimo valor sorteado no dado (1 a 6) */
    int ultimo_dado;

    /* Flag: 1 = jogador pode rolar o dado, 0 = aguardando processamento */
    int aguardando_dado;

    /* ID da alternativa escolhida pelo jogador na pergunta (1 a 5) */
    int resposta_escolhida;

    /* Resultado da pergunta: >0 quantas casas avancou, <0 quantas recuou */
    int resultado_perg;

    /* Carta (pergunta) que esta sendo exibida no momento */
    tp_carta carta_atual;

    /* Indice do jogador que esta escolhendo personagem (0..num_jogadores) */
    int personagem_idx;

    /* IDs dos personagens escolhidos por cada jogador */
    int personagens_escolhidos[4];

    /* Tela atual sendo exibida (determina o que e renderizado) */
    Tela tela;

    /* Camera 3D para visualizacao do tabuleiro em perspectiva */
    Camera3D cam;

    /* Posicoes 3D pre-calculadas de cada casa do tabuleiro */
    Vector3 pos_3d[QTD_CASAS];

    /* Buffer do arquivo de historico lido do disco */
    char hist_buf[15000];

    /* Flag que indica se o buffer do historico ja foi carregado */
    int hist_pronto;

} Estado;

/* ======================================================================== */
/* FUNCOES DE CALCULO DAS POSICOES 3D                                      */
/* ======================================================================== */

/*
 * calc_pos_3d - Preenche o vetor pos_3d com as coordenadas (x, y, z)
 * de cada casa no espaco 3D.
 *
 * O tabuleiro tem 30 casas dispostas em formato "cobra" (snake):
 *   - Fileira inferior (z=2.8): casas 1 a 10, da esquerda para a direita
 *   - Fileira do meio  (z=1.4): casas 11 a 20, da direita para a esquerda
 *   - Fileira superior (z=0.0): casas 21 a 30, da esquerda para a direita
 */
void calc_pos_3d(Vector3 *pos_3d) {
    int i;

    /* Fileira inferior: casas 1-10, x crescente */
    for (i = 0; i < 10; i++) {
        pos_3d[i].x = i * 1.4f;
        pos_3d[i].y = 0;
        pos_3d[i].z = 2.8f;
    }

    /* Fileira do meio: casas 11-20, x decrescente */
    for (i = 0; i < 10; i++) {
        pos_3d[19 - i].x = i * 1.4f;
        pos_3d[19 - i].y = 0;
        pos_3d[19 - i].z = 1.4f;
    }

    /* Fileira superior: casas 21-30, x crescente */
    for (i = 0; i < 10; i++) {
        pos_3d[20 + i].x = i * 1.4f;
        pos_3d[20 + i].y = 0;
        pos_3d[20 + i].z = 0;
    }
}

/* ======================================================================== */
/* FUNCOES AUXILIARES DE INTERFACE                                          */
/* ======================================================================== */

/*
 * mouse_em - Verifica se o cursor do mouse esta dentro de um retangulo.
 * Parametros: r = retangulo a ser testado
 * Retorno: 1 se o mouse esta dentro, 0 caso contrario
 */
int mouse_em(Rectangle r) {
    return CheckCollisionPointRec(GetMousePosition(), r);
}

/*
 * botao - Desenha um botao com texto centralizado e detecta clique.
 * Parametros:
 *   r   = retangulo do botao (posicao e tamanho)
 *   txt = texto exibido dentro do botao
 *   cor = cor de fundo do botao
 * Retorno: 1 se o botao foi clicado, 0 caso contrario
 *
 * Quando o mouse passa sobre o botao, a cor e clareada em 40 pontos
 * por canal para dar feedback visual de hover.
 */
int botao(Rectangle r, char *txt, Color cor) {
    int hover = mouse_em(r);
    Color cor_atual;

    /* Clareia a cor se o mouse estiver em cima */
    if (hover) {
        cor_atual = RGBA(
            (cor.r + 40 > 255) ? 255 : cor.r + 40,
            (cor.g + 40 > 255) ? 255 : cor.g + 40,
            (cor.b + 40 > 255) ? 255 : cor.b + 40,
            255
        );
    } else {
        cor_atual = cor;
    }

    /* Desenha o fundo arredondado e a borda */
    DrawRectangleRounded(r, 0.12f, 6, cor_atual);
    DrawRectangleRoundedLines(r, 0.12f, 6, RGBA(255, 255, 255, 100));

    /* Centraliza e desenha o texto */
    int tam = 22;
    int tx = r.x + r.width / 2 - MeasureText(txt, tam) / 2;
    int ty = r.y + r.height / 2 - tam / 2;
    DrawText(txt, tx, ty, tam, WHITE);

    /* Retorna verdadeiro apenas se o mouse esta em cima E houve clique */
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

/*
 * txt_centro - Desenha um texto centralizado horizontalmente na tela.
 * Parametros:
 *   t   = texto a ser exibido
 *   y   = posicao Y (vertical) do texto
 *   s   = tamanho da fonte em pixels
 *   c   = cor do texto
 */
void txt_centro(char *t, int y, int s, Color c) {
    DrawText(t, LARG / 2 - MeasureText(t, s) / 2, y, s, c);
}

/*
 * txt_wrap - Desenha texto com quebra automatica de linhas (word wrap).
 * Parametros:
 *   t       = texto de entrada
 *   x, y    = posicao inicial do texto
 *   ml      = largura maxima em pixels antes de quebrar
 *   s       = tamanho da fonte
 *   c       = cor do texto
 *
 * O algoritmo percorre o texto caractere por caractere e quebra a linha
 * quando ultrapassa ml, dando preferencia a espacos para nao cortar palavras.
 */
void txt_wrap(char *t, int x, int y, int ml, int s, Color c) {
    int len = strlen(t);
    int inicio = 0;
    int linha_num = 0;

    while (inicio < len) {
        int fim = inicio;
        int larg = 0;
        int ult_esp = -1;

        /* Avanca ate estourar a largura maxima ou encontrar fim de linha */
        while (fim < len && t[fim] != '\n') {
            char tmp[2] = { t[fim], 0 };
            int cl = MeasureText(tmp, s);
            if (larg + cl > ml && larg > 0) break;
            larg += cl;
            if (t[fim] == ' ') ult_esp = fim;
            fim++;
        }

        if (fim == inicio) break;

        /* Determina quantos caracteres copiar, quebrando no espaco se possivel */
        int copia = fim - inicio;
        if (ult_esp >= 0 && t[fim] != '\n' && fim < len)
            copia = ult_esp - inicio;
        if (copia <= 0)
            copia = fim - inicio;

        char buf[256];
        strncpy(buf, t + inicio, copia);
        buf[copia] = '\0';

        DrawText(buf, x, y + linha_num * (s + 4), s, c);
        linha_num++;

        if (t[fim] == '\n') fim++;
        inicio += copia;
        if (t[inicio] == ' ') inicio++;
    }
}

/*
 * escurecer - Retorna uma cor com cada canal RGB multiplicado por f.
 * Parametros:
 *   c = cor original
 *   f = fator de escurecimento (0..1, onde 1 = igual, 0 = preto)
 * Retorno: cor modificada
 */
Color escurecer(Color c, float f) {
    return RGBA(c.r * f, c.g * f, c.b * f, c.a);
}

/* ======================================================================== */
/* DESENHO DO PATO MASCOTE (2D)                                            */
/* ======================================================================== */

/*
 * draw_pato - Desenha um pato estilizado com detalhes elaborados
 * usando circulos, triangulos e linhas.
 *
 * Parametros:
 *   x, y   = posicao central do pato na tela
 *   sc     = escala (1.0 = tamanho padrao)
 *   corpo  = cor do corpo do pato
 *   angulo = rotacao em graus (0 = virado para a direita)
 *
 * O pato e composto por:
 *   - Sombra projetada no chao
 *   - Pernas com pes (triangulos de membrana)
 *   - Rabo em leque (3 penas)
 *   - Corpo (3 circulos sobrepostos formando um oval)
 *   - Asa com detalhes de penas (circulos e tracos)
 *   - Cabeca com topete/crista
 *   - Bico laranja com linha divisoria
 *   - Olho que pisca periodicamente
 *   - Sobrancelha e bochecha
 *   - Animacao suave de balanco (idle)
 */
void draw_pato(int x, int y0, float sc, Color corpo, int angulo) {
    float a  = angulo * DEG2RAD;
    float ca = cosf(a);
    float sa = sinf(a);
    float bob_y = sinf(GetTime() * 2.5f) * 1.5f * sc;
    int blink_aberto = (fmod(GetTime(), 4.0) < 3.5);

    Color sombra   = escurecer(corpo, 0.70f);
    Color mais_esc = escurecer(corpo, 0.50f);
    Color clara;
    int r2 = (int)corpo.r + 30; if (r2 > 255) r2 = 255;
    int g2 = (int)corpo.g + 30; if (g2 > 255) g2 = 255;
    int b2 = (int)corpo.b + 30; if (b2 > 255) b2 = 255;
    clara = RGB(r2, g2, b2);

    /* Macros para transladar e rotacionar pontos 2D */
    #define PX(dx,dy)  (int)(x + ((dx)*ca - (dy)*sa)*sc)
    #define PY(dx,dy)  (int)(y + ((dx)*sa + (dy)*ca)*sc)

    /* Sombra projetada no chao (sem balanco) */
    {
        int sy = y0 + (int)(20 * sc);
        DrawEllipse(PX(0, 20), sy, 18 * sc, 4 * sc, RGBA(0, 0, 0, 35));
    }

    /* Aplica balanco no eixo Y para o resto do desenho */
    int y = y0 + (int)roundf(bob_y);

    /* --- PERNAS --- */
    DrawLine(PX(-3, 17), PY(-3, 17), PX(-4, 26), PY(-4, 26), COR_LARANJA_D);
    DrawLine(PX( 3, 17), PY( 3, 17), PX( 2, 26), PY( 2, 26), COR_LARANJA_D);

    /* Pes (membrana) */
    DrawCircle(PX(-4, 26), PY(-4, 26), 3 * sc, COR_LARANJA);
    DrawCircle(PX( 2, 26), PY( 2, 26), 3 * sc, COR_LARANJA);
    DrawTriangle(
        (Vector2){ PX(-4, 26), PY(-4, 26) },
        (Vector2){ PX(-10, 25), PY(-10, 25) },
        (Vector2){ PX(-4, 24), PY(-4, 24) },
        COR_LARANJA_E
    );
    DrawTriangle(
        (Vector2){ PX(2, 26), PY(2, 26) },
        (Vector2){ PX(-4, 25), PY(-4, 25) },
        (Vector2){ PX(2, 24), PY(2, 24) },
        COR_LARANJA_E
    );

    /* --- RABO (leque de 3 penas) --- */
    DrawCircle(PX(-20, 2), PY(-20, 2), 6 * sc, sombra);
    DrawCircle(PX(-22, 6), PY(-22, 6), 5 * sc, sombra);
    DrawCircle(PX(-18,-1), PY(-18,-1), 5 * sc, sombra);

    /* --- CORPO (oval) --- */
    DrawCircle(PX(0, 8),   PY(0, 8),   14 * sc, corpo);
    DrawCircle(PX(8, 4),   PY(8, 4),   12 * sc, corpo);
    DrawCircle(PX(-8, 10), PY(-8, 10), 10 * sc, corpo);

    /* Detalhe claro no peito */
    DrawCircle(PX(4, 11), PY(4, 11), 6 * sc, clara);

    /* --- ASA COM PENAS --- */
    DrawCircle(PX(0, 6), PY(0, 6), 9 * sc, sombra);
    DrawCircle(PX(3, 8), PY(3, 8), 6 * sc, mais_esc);
    DrawLine(PX(-2, 10), PY(-2, 10), PX(-6, 13), PY(-6, 13), mais_esc);
    DrawLine(PX( 1, 11), PY( 1, 11), PX(-3, 14), PY(-3, 14), mais_esc);
    DrawLine(PX( 4, 11), PY( 4, 11), PX( 0, 14), PY( 0, 14), mais_esc);

    /* --- CABECA --- */
    DrawCircle(PX(14, -5), PY(14, -5), 10 * sc, corpo);

    /* Topete / crista */
    DrawCircle(PX(12, -12), PY(12, -12), 4 * sc, sombra);
    DrawCircle(PX(15, -14), PY(15, -14), 3 * sc, sombra);

    /* --- BICO --- */
    DrawTriangle(
        (Vector2){ PX(21, -5), PY(21, -5) },
        (Vector2){ PX(33, -2), PY(33, -2) },
        (Vector2){ PX(21,  1), PY(21,  1) },
        COR_LARANJA
    );
    DrawLine(PX(21, -2), PY(21, -2), PX(31, -2), PY(31, -2), COR_LARANJA_D);

    /* --- OLHO (com piscada) --- */
    DrawCircle(PX(17, -6), PY(17, -6), 3 * sc, WHITE);
    DrawCircle(PX(18, -6), PY(18, -6), 1.5f * sc, COR_PRETO);
    if (!blink_aberto) {
        DrawLine(PX(15, -6), PY(15, -6), PX(20, -6), PY(20, -6), mais_esc);
    }

    /* Sobrancelha */
    DrawLine(PX(14, -10), PY(14, -10), PX(19, -10), PY(19, -10), mais_esc);

    /* Bochecha */
    DrawCircle(PX(10, -1), PY(10, -1), 3 * sc, RGBA(255, 150, 150, 80));

    #undef PX
    #undef PY
}

/* ======================================================================== */
/* DESENHO DO TOKEN 3D DO PERSONAGEM                                       */
/* ======================================================================== */

/*
 * draw_token_3d - Renderiza o personagem como um modelo 3D simples
 * usando esferas, cilindros e circulos 3D.
 *
 * Cada personagem tem uma aparencia unica:
 *   Patolino:    corpo/cabeca pretos + bico laranja
 *   Pato Donald: corpo branco + chapeu de marinheiro azul
 *   Tio Patinhas: dourado + cartola marrom + oculos
 *   Pato:        marrom + detalhe verde
 */
void draw_token_3d(Estado *est, int jogador_idx, Vector3 pos) {
    int cid = est->personagens_escolhidos[jogador_idx];
    if (cid < 1 || cid > 4) cid = 4;

    Color cor = cor_personagem(cid);
    float raio_corpo = 0.30f;
    float raio_cabeca = 0.18f;

    switch (cid) {

        /* ---- Patolino: pato preto com bico laranja ---- */
        case 1: {
            /* Corpo */
            DrawSphere(pos, raio_corpo, RGB(30, 30, 35));
            /* Cabeca (deslocada para frente e para cima) */
            Vector3 cab = { pos.x + 0.12f, pos.y + 0.22f, pos.z };
            DrawSphere(cab, raio_cabeca, RGB(30, 30, 35));
            /* Bico (cone) */
            DrawCylinder(
                (Vector3){ cab.x + 0.08f, cab.y - 0.02f, cab.z },
                0, 0.06f, 0.07f, 4, COR_LARANJA
            );
            /* Olho */
            DrawCircle3D(
                (Vector3){ cab.x + 0.05f, cab.y + 0.02f, cab.z + 0.04f },
                0.025f, (Vector3){0,0,1}, 0, WHITE
            );
            DrawCircle3D(
                (Vector3){ cab.x + 0.05f, cab.y + 0.02f, cab.z + 0.04f },
                0.012f, (Vector3){0,0,1}, 0, COR_PRETO
            );
            break;
        }

        /* ---- Pato Donald: branco com chapeu de marinheiro ---- */
        case 2: {
            DrawSphere(pos, raio_corpo, RGB(225, 222, 228));
            Vector3 cab = { pos.x, pos.y + 0.22f, pos.z };
            DrawSphere(cab, raio_cabeca, RGB(225, 222, 228));
            /* Chapeu de marinheiro (cilindro azul achatado) */
            DrawCylinder(
                (Vector3){ cab.x, cab.y + 0.12f, cab.z },
                0.12f, 0.08f, 0.06f, 8, RGB(40, 80, 180)
            );
            /* Bico */
            DrawCylinder(
                (Vector3){ cab.x, cab.y + 0.06f, cab.z },
                0.08f, 0.03f, 0.07f, 4, COR_LARANJA
            );
            /* Olho */
            DrawCircle3D(
                (Vector3){ cab.x + 0.05f, cab.y + 0.02f, cab.z + 0.04f },
                0.025f, (Vector3){0,0,1}, 0, WHITE
            );
            DrawCircle3D(
                (Vector3){ cab.x + 0.05f, cab.y + 0.02f, cab.z + 0.04f },
                0.012f, (Vector3){0,0,1}, 0, COR_PRETO
            );
            break;
        }

        /* ---- Tio Patinhas: dourado com cartola e oculos ---- */
        case 3: {
            DrawSphere(pos, raio_corpo, RGB(210, 175, 30));
            Vector3 cab = { pos.x + 0.08f, pos.y + 0.22f, pos.z };
            DrawSphere(cab, raio_cabeca, RGB(210, 175, 30));
            /* Cartola (cilindro marrom) */
            DrawCylinder(
                (Vector3){ cab.x, cab.y + 0.14f, cab.z },
                0.09f, 0.11f, 0.08f, 8, RGB(60, 40, 20)
            );
            /* Bico */
            DrawCylinder(
                (Vector3){ cab.x + 0.06f, cab.y - 0.02f, cab.z },
                0, 0.05f, 0.06f, 4, COR_LARANJA
            );
            /* Oculos (dois aneis + ponte) */
            DrawCircle3D(
                (Vector3){ cab.x, cab.y + 0.04f, cab.z + 0.06f },
                0.035f, (Vector3){0,0,1}, 0, RGBA(255,255,200,120)
            );
            DrawCircle3D(
                (Vector3){ cab.x, cab.y + 0.04f, cab.z + 0.06f },
                0.015f, (Vector3){0,0,1}, 0, COR_PRETO
            );
            DrawCircle3D(
                (Vector3){ cab.x, cab.y + 0.04f, cab.z - 0.06f },
                0.035f, (Vector3){0,0,1}, 0, RGBA(255,255,200,120)
            );
            DrawCircle3D(
                (Vector3){ cab.x, cab.y + 0.04f, cab.z - 0.06f },
                0.015f, (Vector3){0,0,1}, 0, COR_PRETO
            );
            /* Ponte dos oculos */
            DrawLine3D(
                (Vector3){ cab.x, cab.y + 0.04f, cab.z + 0.06f },
                (Vector3){ cab.x, cab.y + 0.04f, cab.z - 0.06f },
                RGBA(255,255,200,120)
            );
            break;
        }

        /* ---- Pato: marrom com detalhe verde ---- */
        case 4: {
            DrawSphere(pos, raio_corpo, RGB(150, 100, 50));
            Vector3 cab = { pos.x + 0.1f, pos.y + 0.22f, pos.z };
            DrawSphere(cab, raio_cabeca, RGB(150, 100, 50));
            /* Bico verde */
            DrawCylinder(
                (Vector3){ cab.x + 0.07f, cab.y - 0.02f, cab.z },
                0, 0.05f, 0.06f, 4, COR_VERDE
            );
            /* Olho */
            DrawCircle3D(
                (Vector3){ cab.x + 0.04f, cab.y + 0.02f, cab.z + 0.04f },
                0.025f, (Vector3){0,0,1}, 0, WHITE
            );
            DrawCircle3D(
                (Vector3){ cab.x + 0.04f, cab.y + 0.02f, cab.z + 0.04f },
                0.012f, (Vector3){0,0,1}, 0, COR_PRETO
            );
            break;
        }

    }
}

/* ======================================================================== */
/* FUNCOES DE LOGICA DO JOGO                                               */
/* ======================================================================== */

/*
 * reiniciar - Prepara o estado para uma nova partida:
 *   - Reseta todas as flags e contadores
 *   - Cria o tabuleiro (lista encadeada de 30 casas)
 *   - Monta o banco de cartas com as 36 perguntas
 *   - Exporta as perguntas para CSV
 *   - Carrega as pilhas de perguntas embaralhadas
 *   - Reseta a selecao de personagens
 */
void reiniciar(Estado *est) {
    int i;

    est->partida_ativa     = 1;
    est->turnos            = 0;
    est->rodada            = 0;
    est->aguardando_dado   = 1;
    est->ultimo_dado       = 0;
    est->resultado_perg    = 0;

    /* Gera o tabuleiro */
    criar_tabuleiro(&est->tabuleiro);

    /* Carrega e prepara as perguntas */
    montar_banco_cartas(est->banco);
    exportar_perguntas_csv(est->banco, 36);
    carregar_perguntas(est->pilhas, est->banco, 36);

    /* Libera a selecao de personagens */
    for (i = 0; i < 4; i++)
        est->personagens_escolhidos[i] = 0;
    est->personagem_idx = 0;
}

/*
 * comecar - Inicializa os jogadores e a fila de turnos apos
 * todos terem escolhido seus personagens.
 *
 * Para cada jogador, copia o nome do personagem escolhido
 * para o campo nome do struct tp_player.
 */
void comecar(Estado *est) {
    int i;

    inicializar_jogadores(est->jogadores, est->num_jogadores);

    /* Atribui o nome do personagem a cada jogador */
    for (i = 0; i < est->num_jogadores; i++) {
        int cid = est->personagens_escolhidos[i];
        char buf[50];
        if (cid >= 1 && cid <= 4)
            copiar_texto(buf, 50, nome_pers(cid));
        else
            copiar_texto(buf, 50, "Pato");
        copiar_texto(est->jogadores[i].nome, sizeof(est->jogadores[i].nome), buf);
    }

    /* Poe os jogadores na fila de turnos */
    colocar_jogadores_na_fila(est->jogadores, est->num_jogadores, &est->fila_turnos);
    est->jogador_atual = 0;

    /* Camera com angulo isometrico sobre o tabuleiro */
    est->cam = (Camera3D){
        { 5.5f, 12, 5.5f },  /* posicao da camera */
        { 5.5f, 0,  1.4f },  /* ponto para onde a camera olha */
        { 0,    1,  0    },  /* vetor "up" da camera */
        50,                   /* campo de visao (fov) */
        CAMERA_PERSPECTIVE    /* tipo de projecao */
    };

    est->tela = TELA_JOGO;
}

/*
 * avancar_turno - Passa a vez para o proximo jogador na fila.
 *
 * Remove o jogador atual da fila e o reinsere no final,
 * depois atualiza o indice do jogador_atual para o proximo.
 * Incrementa turnos e calcula a rodada.
 */
void avancar_turno(Estado *est) {
    tp_item id;

    if (!remove_fila(&est->fila_turnos, &id)) return;

    est->jogador_atual = id - 1;
    insere_fila(&est->fila_turnos, id);

    est->turnos++;
    est->rodada = ((est->turnos - 1) / est->num_jogadores) + 1;
    est->aguardando_dado = 1;
}

/* ======================================================================== */
/* VERIFICA SE PERSONAGEM ESTA DISPONIVEL                                  */
/* ======================================================================== */

/*
 * disp - Retorna 1 se o personagem com o ID informado ainda
 * nao foi escolhido por nenhum jogador, ou 0 se ja esta ocupado.
 */
int disp(Estado *est, int id) {
    int i;
    for (i = 0; i < est->personagem_idx; i++)
        if (est->personagens_escolhidos[i] == id) return 0;
    return 1;
}

/* ======================================================================== */
/* TELA: MENU PRINCIPAL                                                     */
/* ======================================================================== */

void draw_menu(Estado *est) {
    /* Patos decorativos maiores e mais espacados */
    draw_pato(80, 260, 3.2f, COR_LARANJA, 0);
    draw_pato(LARG - 100, 300, 2.4f, escurecer(COR_LARANJA, 0.7f), 0);
    draw_pato(60, ALT - 180, 1.8f, COR_LARANJA_E, -15);
    draw_pato(LARG - 80, ALT - 160, 1.5f, COR_LARANJA_M, 15);

    /* Linhas decorativas no topo e base */
    int i;
    for (i = 0; i < LARG; i += 20) {
        DrawLine(i, 50, i + 10, 50, COR_LARANJA_G);
        DrawLine(i, ALT - 30, i + 10, ALT - 30, COR_LARANJA_G);
    }

    /* Titulo principal com sombra e brilho */
    DrawText("QUACK", LARG/2 - MeasureText("QUACK", 72)/2 + 3, 83, 72, COR_LARANJA_G);
    DrawText("QUACK", LARG/2 - MeasureText("QUACK", 72)/2, 80, 72, COR_LARANJA);
    /* Brilho no titulo */
    {
        int gt = (int)(sinf(GetTime() * 2.0f) * 30 + 60);
        Color brilho = RGBA(255, 180 + gt, 50, 160);
        DrawText("QUACK", LARG/2 - MeasureText("QUACK", 72)/2 + 6, 86, 72, brilho);
    }

    DrawText("BOARDGAME",
        LARG/2 - MeasureText("BOARDGAME", 40)/2 + 2, 142, 40, COR_PRETO_3);
    DrawText("BOARDGAME",
        LARG/2 - MeasureText("BOARDGAME", 40)/2, 140, 40, WHITE);

    DrawText("Perguntas e Respostas",
        LARG/2 - MeasureText("Perguntas e Respostas", 16)/2,
        185, 16, COR_LARANJA_E);

    /* Caixa que agrupa os botoes com moldura decorativa */
    DrawRectangleRounded((Rectangle){ LARG/2-140, 340, 280, 250 }, 0.08f, 6, COR_PRETO_2);
    DrawRectangleRoundedLines((Rectangle){ LARG/2-140, 340, 280, 250 }, 0.08f, 6, COR_LARANJA);
    /* Cantos decorativos */
    DrawCircle(LARG/2-140+10, 340+10, 6, COR_LARANJA);
    DrawCircle(LARG/2+140-10, 340+10, 6, COR_LARANJA);
    DrawCircle(LARG/2-140+10, 590-10, 6, COR_LARANJA);
    DrawCircle(LARG/2+140-10, 590-10, 6, COR_LARANJA);

    /* Botoes do menu */
    Rectangle bj = { LARG/2-110, 375, 220, 50 };
    Rectangle bh = { LARG/2-110, 445, 220, 50 };
    Rectangle bs = { LARG/2-110, 515, 220, 50 };

    if (botao(bj, "NOVO JOGO", COR_LARANJA)) {
        reiniciar(est);
        est->tela = TELA_NUM_JOG;
    }
    if (botao(bh, "HISTORICO", COR_PRETO_C)) {
        est->hist_pronto = 0;
        est->tela = TELA_HISTORICO;
    }
    if (botao(bs, "SAIR", COR_VERMELHO)) {
        CloseWindow();
    }
}

/* ======================================================================== */
/* TELA: NUMERO DE JOGADORES                                                */
/* ======================================================================== */

void draw_num_jog(Estado *est) {
    draw_pato(LARG - 150, ALT - 180, 1.5f, COR_LARANJA, 0);

    txt_centro("NUMERO DE JOGADORES", 160, 34, COR_LARANJA);
    txt_centro("Escolha entre 2 e 4 jogadores", 210, 18, COR_BRANCO_S);

    /* Painel dos botoes */
    DrawRectangleRounded((Rectangle){ LARG/2-215, 300, 430, 90 }, 0.08f, 6, COR_PRETO_2);
    DrawRectangleRoundedLines((Rectangle){ LARG/2-215, 300, 430, 90 }, 0.08f, 6, COR_LARANJA_M);

    Rectangle b2 = { LARG/2 - 195, 315, 170, 55 };
    Rectangle b3 = { LARG/2 - 85,  315, 170, 55 };
    Rectangle b4 = { LARG/2 + 25,  315, 170, 55 };

    if (botao(b2, "2 JOGADORES", COR_LARANJA)) {
        est->num_jogadores = 2;
        est->personagem_idx = 0;
        est->tela = TELA_PERSONAGEM;
    }
    if (botao(b3, "3 JOGADORES", COR_LARANJA)) {
        est->num_jogadores = 3;
        est->personagem_idx = 0;
        est->tela = TELA_PERSONAGEM;
    }
    if (botao(b4, "4 JOGADORES", COR_LARANJA)) {
        est->num_jogadores = 4;
        est->personagem_idx = 0;
        est->tela = TELA_PERSONAGEM;
    }
}

/* ======================================================================== */
/* TELA: SELECAO DE PERSONAGEM                                              */
/* ======================================================================== */

void draw_personagem(Estado *est) {
    char buf[64];
    int ids[4] = { 1, 2, 3, 4 };
    int i;

    txt_centro("SELECAO DE PERSONAGEM", 50, 30, COR_LARANJA);
    sprintf(buf, "Jogador %d, escolha seu personagem", est->personagem_idx + 1);
    txt_centro(buf, 90, 18, COR_BRANCO_S);

    /* Barra de progresso indicando quantos ja escolheram */
    {
        float frac = (float)est->personagem_idx / est->num_jogadores;
        Rectangle bar = { LARG/2 - 150, 115, 300, 10 };
        DrawRectangleRounded(bar, 0.5f, 6, COR_PRETO_3);
        Rectangle fill = { LARG/2 - 150, 115, 300 * frac, 10 };
        DrawRectangleRounded(fill, 0.5f, 6, COR_LARANJA);
    }

    /* Exibe os 4 personagens em grade 2x2 */
    for (i = 0; i < 4; i++) {
        int id = ids[i];
        int disponivel = disp(est, id);

        int cx = 140 + (i % 2) * 530;
        int cy = 150 + (i / 2) * 250;

        Rectangle r = { (float)cx, (float)cy, 240, 230 };

        /* Borda pulsante se o mouse estiver em cima e disponivel */
        Color borda = COR_LARANJA_M;
        if (disponivel) {
            if (mouse_em(r)) {
                int pulso = (int)(sinf(GetTime() * 4.0f) * 30 + 60);
                borda = RGBA(255, 150 + pulso, 0, 255);
            } else {
                borda = COR_LARANJA;
            }
        } else {
            borda = COR_CINZA_ESC;
        }

        DrawRectangleRounded(r, 0.15f, 6,
            disponivel ? COR_PRETO_2 : COR_PRETO_3);
        DrawRectangleRoundedLines(r, 0.15f, 6, borda);

        /* Preview visual do personagem */
        Color cp = cor_personagem(id);
        draw_pato(cx + 120, cy + 70, 1.3f,
            disponivel ? cp : escurecer(cp, 0.4f), 0);

        /* Nome do personagem */
        txt_centro(nome_pers(id), cy + 150, 20,
            disponivel ? WHITE : COR_CINZA_ESC);

        /* Ao clicar, seleciona o personagem (se disponivel) */
        if (disponivel && mouse_em(r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            est->personagens_escolhidos[est->personagem_idx] = id;
            est->personagem_idx++;
            if (est->personagem_idx >= est->num_jogadores)
                comecar(est);
        }

        /* Indicador visual de ja escolhido */
        if (!disponivel) {
            char check[4];
            strcpy(check, "✓");
            DrawText(check, cx + 210, cy + 5, 24, COR_VERDE);
        }
    }
}

/* ======================================================================== */
/* TABULEIRO 3D                                                             */
/* ======================================================================== */

/*
 * draw_board_3d - Renderiza o tabuleiro em perspectiva 3D.
 *
 * Desenha:
 *   - Um grid de referencia no chao
 *   - Um plano escuro simulando o "tabuleiro"
 *   - Um cubo para cada casa, colorido conforme o tipo
 *   - Marcadores 3D indicando o tipo da casa (cone, cilindro)
 *   - Os tokens 3D dos jogadores nas posicoes atuais
 *   - Um destaque pulsante na casa do jogador da vez
 */
void draw_board_3d(Estado *est) {
    int i, j;

    BeginMode3D(est->cam);

    /* Chao e grid */
    DrawGrid(20, 1.0f);
    DrawPlane((Vector3){ 5.5f, -0.01f, 1.4f }, (Vector2){ 17, 5 },
              RGBA(30, 28, 25, 255));

    /* Desenha cada casa do tabuleiro */
    for (i = 0; i < QTD_CASAS; i++) {
        tp_casa *c = obter_casa(&est->tabuleiro, i + 1);

        /* Seleciona a cor com base no tipo da casa */
        Color cor = COR_CASA_NORMAL;
        if (c) {
            if (c->tipo == 1)      cor = COR_CASA_PERG;
            else if (c->tipo == 2) cor = COR_CASA_BONUS;
            else if (c->tipo == 3) cor = COR_CASA_PENAL;
        }

        Vector3 p = est->pos_3d[i];

        /* Cubo principal da casa */
        DrawCube(p, 1.0f, 0.35f, 1.0f, cor);
        DrawCubeWires(p, 1.0f, 0.35f, 1.0f, RGBA(0, 0, 0, 100));

        /* Marcador 3D do tipo de casa sobre o cubo */
        Vector3 marc = { p.x, p.y + 0.3f, p.z };
        if (c && c->tipo == 1) {
            /* Casa de pergunta: cone apontando para cima */
            DrawCylinder(marc, 0.08f, 0, 0.15f, 4, RGBA(255,255,200,200));
        } else if (c && c->tipo == 2) {
            /* Casa bonus: cilindro curto */
            DrawCylinder(marc, 0.06f, 0.06f, 0.1f, 6, RGBA(200,255,200,200));
        } else if (c && c->tipo == 3) {
            /* Casa penalidade: cone invertido */
            DrawCylinder(marc, 0.08f, 0, 0.12f, 4, RGBA(255,200,200,200));
        }
    }

    /* Tokens dos jogadores sobre as casas */
    for (j = 0; j < est->num_jogadores; j++) {
        int casa_idx = est->jogadores[j].posicao - 1;
        if (casa_idx < 0 || casa_idx >= QTD_CASAS) continue;

        Vector3 bp = est->pos_3d[casa_idx];

        /* Se mais de um jogador esta na mesma casa, desloca lateralmente */
        float offset = 0;
        int k;
        for (k = 0; k < j; k++) {
            if (est->jogadores[k].posicao == est->jogadores[j].posicao)
                offset += 0.25f;
        }

        Vector3 tp = { bp.x - 0.2f + offset, bp.y + 0.3f, bp.z + 0.2f };
        draw_token_3d(est, j, tp);
    }

    /* Destaque na casa do jogador atual (caixa amarela pulsante) */
    int casa_atual = est->jogadores[est->jogador_atual].posicao - 1;
    if (casa_atual >= 0 && casa_atual < QTD_CASAS) {
        Vector3 hp = est->pos_3d[casa_atual];
        DrawCubeWires(
            (Vector3){ hp.x, hp.y + 0.25f, hp.z },
            1.2f, 0.6f, 1.2f,
            RGBA(255, 255, 100, 80)
        );
    }

    EndMode3D();

    /* Numeros das casas projetados em 2D (sobre a projecao 3D) */
    for (i = 0; i < QTD_CASAS; i++) {
        Vector3 p = est->pos_3d[i];
        Vector2 sc = GetWorldToScreen(
            (Vector3){ p.x, p.y + 0.2f, p.z }, est->cam
        );
        char num[4];
        sprintf(num, "%d", i + 1);
        DrawText(num, sc.x - MeasureText(num, 12) / 2, sc.y - 12, 12,
                 RGBA(255, 255, 255, 150));
    }
}

/* ======================================================================== */
/* PAINEL LATERAL DO JOGO                                                  */
/* ======================================================================== */
/*
 * draw_dado - Desenha um dado com pontos no estilo classico.
 * cx, cy = centro do dado, tam = largura/altura.
 * valor = 1 a 6.
 */
void draw_dado(int cx, int cy, int tam, int valor) {
    Rectangle r = { cx - tam/2, cy - tam/2, tam, tam };
    DrawRectangleRounded(r, 0.15f, 6, RGBA(240, 235, 230, 255));
    DrawRectangleRoundedLines(r, 0.15f, 6, COR_PRETO);

    int pd = tam / 7;
    if (pd < 2) pd = 2;

    Color pto = COR_PRETO;
    int m3 = tam / 2;
    int q1 = tam / 4;
    int q3 = 3 * tam / 4;

    switch (valor) {
        case 1:
            DrawCircle(cx, cy, pd, pto);
            break;
        case 2:
            DrawCircle(cx - q1, cy - q1, pd, pto);
            DrawCircle(cx + q1, cy + q1, pd, pto);
            break;
        case 3:
            DrawCircle(cx - q1, cy - q1, pd, pto);
            DrawCircle(cx, cy, pd, pto);
            DrawCircle(cx + q1, cy + q1, pd, pto);
            break;
        case 4:
            DrawCircle(cx - q1, cy - q1, pd, pto);
            DrawCircle(cx + q1, cy - q1, pd, pto);
            DrawCircle(cx - q1, cy + q1, pd, pto);
            DrawCircle(cx + q1, cy + q1, pd, pto);
            break;
        case 5:
            DrawCircle(cx - q1, cy - q1, pd, pto);
            DrawCircle(cx + q1, cy - q1, pd, pto);
            DrawCircle(cx, cy, pd, pto);
            DrawCircle(cx - q1, cy + q1, pd, pto);
            DrawCircle(cx + q1, cy + q1, pd, pto);
            break;
        case 6:
            DrawCircle(cx - q1, cy - q1, pd, pto);
            DrawCircle(cx + q1, cy - q1, pd, pto);
            DrawCircle(cx - q1, cy, pd, pto);
            DrawCircle(cx + q1, cy, pd, pto);
            DrawCircle(cx - q1, cy + q1, pd, pto);
            DrawCircle(cx + q1, cy + q1, pd, pto);
            break;
    }
}

/*
 * draw_painel - Desenha o painel informativo na lateral direita da tela.
 * Contem informacoes do jogador atual, dado, botoes de acao, placar e pause.
 */
void draw_painel(Estado *est) {
    int px = LARG - 290;
    int py = 15;
    int pw = 275;
    int ix = px + 18;
    int iy;
    char buf[128];
    tp_player *j = &est->jogadores[est->jogador_atual];

    /* Fundo do painel */
    DrawRectangleRounded((Rectangle){ (float)px, (float)py, pw, ALT - 30 },
                         0.08f, 6, COR_PRETO_2);
    DrawRectangleRoundedLines((Rectangle){ (float)px, (float)py, pw, ALT - 30 },
                              0.08f, 6, COR_LARANJA_M);
    DrawLine(px, py + 55, px + pw, py + 55, COR_LARANJA_M);

    /* ---- RODADA ---- */
    iy = py + 12;
    DrawText("RODADA", ix, iy, 14, COR_LARANJA_E);
    sprintf(buf, "%d", est->rodada);
    DrawText(buf, ix + 85, iy, 22, WHITE);

    /* ---- VEZ DE ---- */
    iy = py + 65;
    DrawText("VEZ DE:", ix, iy, 14, COR_BRANCO_S);
    draw_pato(ix + 50, iy + 15, 0.6f,
              cor_personagem(est->personagens_escolhidos[est->jogador_atual]), 0);
    Color cor_nome = (est->jogador_atual == 0) ? COR_JOG_1 :
                     (est->jogador_atual == 1) ? COR_JOG_2 :
                     (est->jogador_atual == 2) ? COR_JOG_3 : COR_JOG_4;
    sprintf(buf, "%s", j->nome);
    DrawText(buf, ix + 95, iy + 20, 20, cor_nome);

    /* Casa e unidade atuais */
    iy += 55;
    sprintf(buf, "Casa %d / Unidade %d", j->posicao, unidade_por_posicao(j->posicao));
    DrawText(buf, ix, iy, 15, COR_BRANCO_S);

    /* ---- DADO ---- */
    iy += 45;
    DrawText("DADO:", ix, iy, 14, COR_LARANJA_E);

    int dcex = ix + 80;
    int dcey = iy + 22;
    draw_dado(dcex, dcey, 52, est->ultimo_dado);

    /* ---- BOTAO ROLAR DADO ---- */
    iy += 75;
    Rectangle rr = { (float)ix, (float)iy, pw - 36, 45 };
    if (botao(rr, "ROLAR DADO", COR_LARANJA)) {
        if (est->aguardando_dado && est->partida_ativa) {

            est->ultimo_dado = rolar_dado();
            est->jogadores[est->jogador_atual].posicao = mover_posicao(
                &est->tabuleiro,
                est->jogadores[est->jogador_atual].posicao,
                est->ultimo_dado
            );
            registrar_historico_posicao(
                &est->jogadores[est->jogador_atual],
                est->jogadores[est->jogador_atual].posicao
            );

            if (est->jogadores[est->jogador_atual].posicao >= est->tabuleiro.total) {
                est->tela = TELA_VENCEDOR;
                return;
            }

            tp_casa *casa = obter_casa(
                &est->tabuleiro,
                est->jogadores[est->jogador_atual].posicao
            );

            if (casa && casa->tipo == 1) {
                int nivel = sortear_dificuldade();
                if (sortear_carta(
                    est->pilhas, est->banco, 36,
                    casa->unidade, nivel,
                    &est->carta_atual
                )) {
                    est->tela = TELA_PERGUNTA;
                }
                est->aguardando_dado = 0;

            } else if (casa && casa->tipo == 2) {
                est->jogadores[est->jogador_atual].posicao = mover_posicao(
                    &est->tabuleiro,
                    est->jogadores[est->jogador_atual].posicao,
                    casa->efeito
                );
                registrar_historico_posicao(
                    &est->jogadores[est->jogador_atual],
                    est->jogadores[est->jogador_atual].posicao
                );
                if (est->jogadores[est->jogador_atual].posicao >= est->tabuleiro.total) {
                    est->tela = TELA_VENCEDOR;
                    return;
                }
                est->aguardando_dado = 0;

            } else if (casa && casa->tipo == 3) {
                est->jogadores[est->jogador_atual].posicao = mover_posicao(
                    &est->tabuleiro,
                    est->jogadores[est->jogador_atual].posicao,
                    -casa->efeito
                );
                registrar_historico_posicao(
                    &est->jogadores[est->jogador_atual],
                    est->jogadores[est->jogador_atual].posicao
                );
                est->aguardando_dado = 0;

            } else {
                est->aguardando_dado = 0;
            }
        }
    }

    /* ---- PLACAR ---- */
    iy += 70;
    DrawText("-- PLACAR --", ix, iy, 14, COR_LARANJA_E);
    {
        int k;
        for (k = 0; k < est->num_jogadores; k++) {
            Color cor_jog = (k == 0) ? COR_JOG_1 :
                            (k == 1) ? COR_JOG_2 :
                            (k == 2) ? COR_JOG_3 : COR_JOG_4;

            /* Icone do personagem ao lado do nome */
            draw_pato(ix + 5, iy + 22 + k * 24, 0.35f,
                      cor_personagem(est->personagens_escolhidos[k]), 0);

            sprintf(buf, "%s: Casa %d",
                    est->jogadores[k].nome, est->jogadores[k].posicao);
            DrawText(buf, ix + 25, iy + 22 + k * 22, 13, cor_jog);
        }
    }

    /* ---- BOTAO PAUSE ---- */
    Rectangle rp = { (float)ix, (float)(ALT - 65), pw - 36, 38 };
    if (botao(rp, "PAUSE", COR_LARANJA_D)) {
        est->tela = TELA_PAUSE;
    }
}

/* ======================================================================== */
/* TELA: JOGO (PRINCIPAL)                                                   */
/* ======================================================================== */

void draw_jogo(Estado *est) {
    draw_board_3d(est);
    draw_painel(est);

    /* Se a tela mudou durante o painel (vitoria), nao avanca turno */
    if (est->tela != TELA_JOGO) return;

    /* Se o jogador ja terminou a acao, passa automaticamente para o proximo */
    if (!est->aguardando_dado && est->partida_ativa) {
        avancar_turno(est);
    }
}

/* ======================================================================== */
/* TELA: PERGUNTA                                                            */
/* ======================================================================== */

void draw_pergunta(Estado *est) {
    char buf[128];
    int i;

    Rectangle bg = { 70, 40, LARG - 140, ALT - 80 };
    DrawRectangleRounded(bg, 0.08f, 6, COR_PRETO_2);
    DrawRectangleRoundedLines(bg, 0.08f, 6, COR_LARANJA);
    DrawRectangleRoundedLines(bg, 0.08f, 6, RGBA(255, 102, 0, 40));

    /* Cabecalho com unidade e dificuldade */
    sprintf(buf, "UNIDADE %d - DIFICULDADE %d",
            est->carta_atual.unidade, est->carta_atual.dificuldade);
    DrawText(buf, 95, 55, 20, COR_LARANJA);

    /* Enunciado da pergunta (com quebra de linha) */
    int py_perg = 100;
    txt_wrap(est->carta_atual.pergunta, 95, py_perg, LARG - 250, 18, WHITE);

    /* Alternativas: calcula o espacamento disponivel */
    int py_alt = py_perg + 50;
    int esp_alt = (ALT - py_alt - 60) / 5;
    if (esp_alt < 44) esp_alt = 44;

    for (i = 0; i < 5; i++) {
        int ya = py_alt + i * esp_alt;
        Rectangle r = { 100, (float)ya, LARG - 260, (float)(esp_alt - 8) };

        Color cor = RGBA(50, 48, 55, 255);
        Color borda = COR_CINZA_ESC;
        if (mouse_em(r)) {
            cor = RGBA(70, 68, 80, 255);
            borda = COR_LARANJA;
        }

        DrawRectangleRounded(r, 0.08f, 6, cor);
        DrawRectangleRoundedLines(r, 0.08f, 6, borda);

        /* Letra da alternativa (A, B, C, D, E) */
        char letra = 'A' + i;
        char alt[8];
        sprintf(alt, "%c)", letra);
        DrawText(alt, r.x + 10, r.y + (r.height - 16) / 2, 16, COR_LARANJA);

        /* Texto da alternativa */
        txt_wrap(est->carta_atual.alternativas[i],
                 r.x + 45, r.y + 6, LARG - 370, 15, COR_BRANCO_S);

        /* Detecta clique do jogador em uma alternativa */
        if (mouse_em(r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            est->resposta_escolhida = i + 1;
            int acertou = (est->resposta_escolhida == est->carta_atual.resposta);

            char turma[50] = "AED-2026.1-T";
            salvar_historico_resposta(
                turma,
                est->jogadores[est->jogador_atual].nome,
                est->carta_atual.id_carta,
                est->carta_atual.unidade,
                est->carta_atual.tema,
                est->carta_atual.subtema,
                est->carta_atual.dificuldade,
                est->resposta_escolhida,
                est->carta_atual.resposta,
                acertou
            );

            est->resultado_perg = acertou ? est->carta_atual.avanco : -1;
            if (est->resultado_perg != 0) {
                est->jogadores[est->jogador_atual].posicao = mover_posicao(
                    &est->tabuleiro,
                    est->jogadores[est->jogador_atual].posicao,
                    est->resultado_perg
                );
                registrar_historico_posicao(
                    &est->jogadores[est->jogador_atual],
                    est->jogadores[est->jogador_atual].posicao
                );
            }

            if (est->jogadores[est->jogador_atual].posicao >= est->tabuleiro.total)
                est->tela = TELA_VENCEDOR;
            else
                est->tela = TELA_RESULTADO;
        }
    }

    /* Patos decorativos */
    draw_pato(LARG - 130, ALT - 160, 1.2f, COR_LARANJA, 0);
    draw_pato(95 + 30, ALT - 130, 0.8f, COR_LARANJA_E, -10);
}

/* ======================================================================== */
/* TELA: RESULTADO DA PERGUNTA                                             */
/* ======================================================================== */

void draw_resultado(Estado *est) {
    int acertou = (est->resultado_perg > 0);
    Color cor = acertou ? COR_VERDE : COR_VERMELHO;

    char buf[256];

    /* Indicador de acerto/erro com sombra */
    DrawText(acertou ? "CORRETA!" : "ERRADA!",
        LARG / 2 - MeasureText(acertou ? "CORRETA!" : "ERRADA!", 50) / 2 + 2,
        202, 50, RGBA(0, 0, 0, 100));
    DrawText(acertou ? "CORRETA!" : "ERRADA!",
        LARG / 2 - MeasureText(acertou ? "CORRETA!" : "ERRADA!", 50) / 2,
        200, 50, cor);

    /* Icone grande de check ou X */
    if (acertou) {
        draw_pato(LARG/2 - 80, 230, 2.5f, COR_VERDE, 0);
    } else {
        DrawRectangleRounded(
            (Rectangle){ LARG/2 - 35, 240, 70, 70 }, 0.15f, 6, COR_VERMELHO);
        DrawText("X",
            LARG/2 - MeasureText("X", 50)/2, 248, 50, WHITE);
    }

    /* Mensagem descritiva */
    {
        int my = acertou ? 310 : 340;
        if (acertou) {
            sprintf(buf, "Avancou %d casa(s)!", est->resultado_perg);
        } else {
            sprintf(buf, "Resposta: %c) %s",
                    'A' + est->carta_atual.resposta - 1,
                    est->carta_atual.alternativas[est->carta_atual.resposta - 1]);
        }
        txt_centro(buf, my, 18, COR_BRANCO_S);
    }

    /* Posicao atual do jogador */
    sprintf(buf, "%s na Casa %d",
            est->jogadores[est->jogador_atual].nome,
            est->jogadores[est->jogador_atual].posicao);
    txt_centro(buf, acertou ? 340 : 370, 18, WHITE);

    /* Pato decorativo */
    draw_pato(LARG / 2 - 100, ALT - 180, 1.5f,
              acertou ? COR_VERDE : COR_VERMELHO, 0);

    /* Botao para continuar */
    Rectangle rc = { LARG / 2 - 100, acertou ? 390 : 420, 200, 45 };
    if (botao(rc, "CONTINUAR", COR_LARANJA)) {
        est->tela = TELA_JOGO;
        avancar_turno(est);
    }
}

/* ======================================================================== */
/* TELA: VENCEDOR / RANKING                                                */
/* ======================================================================== */

void draw_vencedor(Estado *est) {
    char buf[256];
    int i, j, k;

    /* Confete caindo */
    {
        double t = GetTime();
        for (i = 0; i < 60; i++) {
            float px = (float)(fmod(t * 80 + i * 137.5, LARG));
            float py = (float)(fmod(t * 120 + i * 97.3, ALT));
            float raio = 2.5f + (i % 3) * 1.5f;
            Color c_confete = (i % 4 == 0) ? COR_LARANJA :
                              (i % 4 == 1) ? COR_DOURADO :
                              (i % 4 == 2) ? COR_VERDE : COR_VERMELHO;
            DrawCircle(px, py, raio, RGBA(c_confete.r, c_confete.g, c_confete.b, 180));
        }
    }

    /* Patos decorativos comemorando */
    draw_pato(LARG / 2 + 200, 280, 2.5f, COR_DOURADO, 0);
    draw_pato(LARG / 2 - 200, 300, 2.0f, COR_LARANJA, 0);
    draw_pato(LARG / 2 + 120, 200, 1.2f, COR_VERDE, -20);
    draw_pato(LARG / 2 - 120, 210, 1.2f, COR_LARANJA_E, 20);

    /* Titulo com brilho */
    {
        int gt = (int)(sinf(GetTime() * 3.0f) * 40 + 100);
        Color brilho = RGBA(255, 200 + gt/3, 50, 200);
        DrawText("VENCEDOR!",
            LARG/2 - MeasureText("VENCEDOR!", 50)/2 + 2, 52, 50, COR_DOURADO);
        DrawText("VENCEDOR!",
            LARG/2 - MeasureText("VENCEDOR!", 50)/2, 50, 50, brilho);
    }

    sprintf(buf, "%s VENCEU O JOGO!",
            est->jogadores[est->jogador_atual].nome);
    txt_centro(buf, 110, 28, COR_VERDE);

    /* Ranking */
    DrawText("RANKING FINAL:", 60, 160, 22, COR_LARANJA);

    /* Copia o vetor de jogadores para ordenar sem alterar o original */
    tp_player copia[4];
    for (i = 0; i < est->num_jogadores; i++)
        copia[i] = est->jogadores[i];

    /* Bubble sort (apenas 2 a 4 elementos) */
    for (i = 0; i < est->num_jogadores - 1; i++) {
        for (j = 0; j < est->num_jogadores - i - 1; j++) {
            if (copia[j].posicao < copia[j + 1].posicao) {
                tp_player temp = copia[j];
                copia[j] = copia[j + 1];
                copia[j + 1] = temp;
            }
        }
    }

    /* Exibe o ranking */
    int y0 = 200;
    for (i = 0; i < est->num_jogadores; i++) {
        Color cor = (i == 0) ? COR_DOURADO : COR_BRANCO_S;

        int cid = 1;
        for (k = 0; k < est->num_jogadores; k++) {
            if (strcmp(est->jogadores[k].nome, copia[i].nome) == 0) {
                cid = est->personagens_escolhidos[k];
                break;
            }
        }

        /* Trofel para o primeiro */
        if (i == 0) {
            DrawCircle(55, y0 + i * 30 - 10, 8, COR_DOURADO);
        }

        /* Icone do personagem ao lado do nome */
        draw_pato(75, y0 + i * 30 - 3, 0.5f, cor_personagem(cid), 0);
        sprintf(buf, "%do: %s (Casa %d)", i + 1, copia[i].nome, copia[i].posicao);
        DrawText(buf, 110, y0 + i * 30, 18, cor);
    }

    /* Caminhos percorridos */
    int cy = y0 + est->num_jogadores * 30 + 10;
    txt_centro("Caminhos percorridos:", cy, 16, COR_LARANJA_E);

    for (i = 0; i < est->num_jogadores; i++) {
        int cid = 1;
        for (k = 0; k < est->num_jogadores; k++) {
            if (strcmp(est->jogadores[k].nome, copia[i].nome) == 0) {
                cid = est->personagens_escolhidos[k];
                break;
            }
        }

        char caminho[1500] = "";
        for (j = 0; j < copia[i].total_casas_visitadas; j++) {
            char tmp[8];
            sprintf(tmp, "%d", copia[i].historico_casas[j]);
            strcat(caminho, tmp);
            if (j < copia[i].total_casas_visitadas - 1)
                strcat(caminho, " -> ");
        }

        int py = cy + 26 + i * 40;

        draw_pato(60, py - 3, 0.4f, cor_personagem(cid), 0);

        sprintf(buf, "%s:", copia[i].nome);
        Color cor_jog = (i == 0) ? COR_JOG_1 :
                        (i == 1) ? COR_JOG_2 :
                        (i == 2) ? COR_JOG_3 : COR_JOG_4;
        DrawText(buf, 85, py, 14, cor_jog);

        int nome_larg = MeasureText(buf, 14);
        txt_wrap(caminho, 90 + nome_larg, py - 2, LARG - 180, 12, COR_BRANCO_S);
    }

    /* Botao para voltar ao menu */
    int vy = cy + 26 + est->num_jogadores * 40 + 15;
    Rectangle rm = { LARG / 2 - 100, (float)vy, 200, 42 };
    if (botao(rm, "MENU PRINCIPAL", COR_LARANJA)) {
        destruir_tabuleiro(&est->tabuleiro);
        est->tela = TELA_MENU;
    }
}

/* ======================================================================== */
/* TELA: PAUSE                                                              */
/* ======================================================================== */

void draw_pause(Estado *est) {
    /* Overlay escurecido no fundo */
    DrawRectangle(0, 0, LARG, ALT, RGBA(0, 0, 0, 160));

    /* Painel central */
    DrawRectangleRounded((Rectangle){ LARG/2-160, 160, 320, 320 },
                         0.1f, 6, COR_PRETO_2);
    DrawRectangleRoundedLines((Rectangle){ LARG/2-160, 160, 320, 320 },
                              0.1f, 6, COR_LARANJA);

    /* Cantos decorativos */
    int d = 10;
    DrawCircle(LARG/2-160+d, 160+d, 5, COR_LARANJA);
    DrawCircle(LARG/2+160-d, 160+d, 5, COR_LARANJA);
    DrawCircle(LARG/2-160+d, 480-d, 5, COR_LARANJA);
    DrawCircle(LARG/2+160-d, 480-d, 5, COR_LARANJA);

    txt_centro("PAUSE", 190, 40, COR_LARANJA);

    /* Patos decorativos */
    draw_pato(LARG / 2 - 120, 290, 0.9f, COR_LARANJA, 0);
    draw_pato(LARG / 2 + 40, 290, 0.9f, escurecer(COR_LARANJA, 0.7f), 0);

    /* Botoes */
    Rectangle rc = { LARG/2-100, 250, 200, 45 };
    Rectangle rm = { LARG/2-100, 320, 200, 45 };
    Rectangle rs = { LARG/2-100, 390, 200, 45 };

    if (botao(rc, "CONTINUAR", COR_LARANJA))
        est->tela = TELA_JOGO;

    if (botao(rm, "MENU PRINCIPAL", COR_LARANJA_D)) {
        destruir_tabuleiro(&est->tabuleiro);
        est->tela = TELA_MENU;
    }

    if (botao(rs, "SAIR DO JOGO", COR_VERMELHO))
        CloseWindow();
}

/* ======================================================================== */
/* TELA: HISTORICO DE RESPOSTAS                                            */
/* ======================================================================== */

void draw_historico(Estado *est) {
    txt_centro("HISTORICO DE RESPOSTAS", 30, 28, COR_LARANJA);
    draw_pato(LARG - 120, 30, 0.8f, COR_LARANJA, 0);

    /* Carrega o arquivo CSV para o buffer na primeira vez */
    if (!est->hist_pronto) {
        est->hist_buf[0] = '\0';

        FILE *f = fopen("historico_respostas.csv", "r");
        if (f) {
            size_t pos = 0;
            char linha[512];
            while (fgets(linha, sizeof(linha), f) &&
                   pos < sizeof(est->hist_buf) - 512) {
                size_t len = strlen(linha);
                memcpy(est->hist_buf + pos, linha, len);
                pos += len;
            }
            est->hist_buf[pos] = '\0';
            fclose(f);
        }
        est->hist_pronto = 1;
    }

    /* Exibe o conteudo linha a linha */
    if (est->hist_buf[0]) {
        int y = 70;
        char *line = est->hist_buf;
        while (*line && y < ALT - 70) {
            char *nl = strchr(line, '\n');
            int len = nl ? (int)(nl - line) : (int)strlen(line);
            if (len > 0) {
                int cl = (len < 511) ? len : 511;
                char tmp[512];
                strncpy(tmp, line, cl);
                tmp[cl] = '\0';
                DrawText(tmp, 40, y, 13, COR_BRANCO_S);
                y += 16;
            }
            if (nl)
                line = nl + 1;
            else
                break;
        }
    } else {
        txt_centro("Nenhum historico encontrado.", ALT / 2 - 20, 20, COR_LARANJA_E);
    }

    /* Botao voltar */
    Rectangle rv = { LARG / 2 - 70, ALT - 55, 140, 38 };
    if (botao(rv, "VOLTAR", COR_LARANJA)) {
        est->tela = TELA_MENU;
        est->hist_pronto = 0;  /* Recarrega na proxima vez */
    }
}

/* ======================================================================== */
/* FUNCAO PRINCIPAL DA INTERFACE GRAFICA                                   */
/* ======================================================================== */

/*
 * gui_main - Ponto de entrada da interface grafica.
 *
 * Inicializa a janela Raylib, cria o estado local do jogo e
 * executa o loop principal de renderizacao, delegando cada
 * quadro a funcao correspondente a tela atual.
 *
 * Controles da camera 3D durante o jogo:
 *   Setas direcionais: movem a camera no plano XZ
 *   W/S: aproximam/afastam a camera (zoom)
 */
void gui_main() {
    Estado est;  /* Unica instancia do estado do jogo, toda na pilha */

    /* Pre-calcula as posicoes 3D das casas */
    calc_pos_3d(est.pos_3d);

    /* Configura a janela */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(LARG, ALT, "Quack Jogo de Tabuleiro Educativo");
    SetTargetFPS(60);
    SetWindowMinSize(900, 600);

    /* Estado inicial da camera e tela */
    est.cam = (Camera3D){
        { 6, 10, 6 }, { 5.5f, 0, 1.4f }, { 0, 1, 0 }, 50, CAMERA_PERSPECTIVE
    };
    est.tela = TELA_MENU;
    est.hist_pronto = 0;

    /* Loop principal de eventos/renderizacao */
    while (!WindowShouldClose()) {

        /* Controle da camera 3D (apenas na tela de jogo) */
        if (est.tela == TELA_JOGO) {
            float vel = 0.5f * GetFrameTime();
            if (IsKeyDown(KEY_RIGHT)) est.cam.position.x += vel;
            if (IsKeyDown(KEY_LEFT))  est.cam.position.x -= vel;
            if (IsKeyDown(KEY_UP))    est.cam.position.z -= vel;
            if (IsKeyDown(KEY_DOWN))  est.cam.position.z += vel;
            if (IsKeyDown(KEY_W))     est.cam.position.y = fmin(est.cam.position.y + vel*2, 20);
            if (IsKeyDown(KEY_S))     est.cam.position.y = fmax(est.cam.position.y - vel*2, 3);
            est.cam.target = (Vector3){ 5.5f, 0, 1.4f };
        }

        /* Inicia o quadro */
        BeginDrawing();
        ClearBackground(COR_PRETO);

        /* Renderiza a tela atual */
        switch (est.tela) {
            case TELA_MENU:       draw_menu(&est);       break;
            case TELA_NUM_JOG:    draw_num_jog(&est);    break;
            case TELA_PERSONAGEM: draw_personagem(&est); break;
            case TELA_JOGO:       draw_jogo(&est);       break;
            case TELA_PERGUNTA:   draw_pergunta(&est);   break;
            case TELA_RESULTADO:  draw_resultado(&est);  break;
            case TELA_VENCEDOR:   draw_vencedor(&est);   break;
            case TELA_PAUSE:      draw_pause(&est);      break;
            case TELA_HISTORICO:  draw_historico(&est);  break;
        }

        /* Dica de controles na tela de jogo */
        if (est.tela == TELA_JOGO) {
            DrawText("Setas: mover | W/S: zoom",
                     10, ALT - 20, 12, RGBA(255, 255, 255, 35));
        }

        /* Finaliza o quadro */
        EndDrawing();
    }

    /* Fecha a janela ao sair */
    CloseWindow();
}

#endif /* INTERFACE_H */
