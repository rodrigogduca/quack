// tema.h
// Design System (Design Tokens) do QUACK Boardgame.
// Centraliza todas as cores, dimensoes e utilitarios visuais para garantir
// consistencia entre todas as telas e componentes da interface grafica.

#ifndef TEMA_H
#define TEMA_H

#include "raylib.h"

/* ======================================================================== */
/* DESIGN TOKENS - Tudo centralizado para consistencia visual                */
/* ======================================================================== */

/* ------------------------------------------------------------------------ */
/* PALETA PRINCIPAL                                                         */
/* ------------------------------------------------------------------------ */

// Macro para criar cor RGBA (0-255) e RGB (alfa=255)
#define RGBA(r,g,b,a) ((Color){(r),(g),(b),(a)})
#define RGB(r,g,b)    RGBA(r,g,b,255)

// Cores primarias - laranja como cor principal do jogo
#define COR_PRIMARY       RGB(255, 102, 0)   // Laranja principal (botoes, titulos, acentos)
#define COR_PRIMARY_LIGHT RGB(255, 153, 51)  // Laranja claro (hover, detalhes suaves)
#define COR_PRIMARY_DARK  RGB(200, 80, 0)    // Laranja escuro (sombras, bordas fortes)
#define COR_PRIMARY_MUTED RGB(204, 82, 0)    // Laranja opaco (bordas de cards, elementos secundarios)

// Cores neutras - fundo, superficie e texto
#define COR_BG            RGB(18, 18, 20)     // Fundo da tela (quase preto)
#define COR_SURFACE       RGB(25, 25, 28)     // Cards, paineis laterais
#define COR_SURFACE_ALT   RGB(35, 35, 40)     // Alternativa mais clara para contraste
#define COR_SURFACE_LIGHT RGB(40, 40, 45)     // Hover em superficies
#define COR_BORDER        RGB(60, 60, 65)     // Bordas padrao
#define COR_TEXT          RGB(220, 220, 225)  // Texto principal (branco suave)
#define COR_TEXT_MUTED    RGB(140, 140, 145)  // Texto secundario (cinza medio)
#define COR_TEXT_DIM      RGB(100, 100, 105)  // Texto terciario (cinza escuro)

// Cores funcionais - feedback semantico para o jogador
#define COR_SUCCESS       RGB(50, 205, 50)    // Acertou pergunta / bonus
#define COR_DANGER        RGB(220, 50, 50)    // Errou pergunta / penalidade
#define COR_WARNING       RGB(255, 200, 50)   // Atencao / destaque especial
#define COR_INFO          RGB(80, 200, 255)   // Informacao / dica

// Cores dos personagens (modelos 3D no tabuleiro)
#define COR_PERS_1        RGB(35, 35, 40)     // Personagem 1: cinza escuro
#define COR_PERS_2        RGB(230, 225, 230)  // Personagem 2: branco suave
#define COR_PERS_3        RGB(210, 175, 30)   // Personagem 3: dourado
#define COR_PERS_4        RGB(150, 100, 50)   // Personagem 4: marrom

// Cores dos jogadores (usadas no placar e painel lateral)
#define COR_JOG_1         RGB(255, 140, 50)   // Jogador 1: laranja
#define COR_JOG_2         RGB(255, 200, 50)   // Jogador 2: amarelo
#define COR_JOG_3         RGB(80, 200, 255)   // Jogador 3: azul claro
#define COR_JOG_4         RGB(255, 100, 100)  // Jogador 4: vermelho claro

// Cores das casas do tabuleiro 3D
#define COR_CASA_NORMAL   RGB(70, 63, 50)     // Casa normal (marrom escuro)
#define COR_CASA_PERG     RGB(255, 140, 0)    // Casa de pergunta (laranja)
#define COR_CASA_BONUS    RGB(0, 180, 60)     // Casa de bonus (verde)
#define COR_CASA_PENAL    RGB(200, 40, 40)    // Casa de penalidade (vermelho)

/* ------------------------------------------------------------------------ */
/* ESPACAMENTO (padding/margin/gap em pixels)                               */
/* ------------------------------------------------------------------------ */
#define SPC_XXS  2     // Extra extra pequeno
#define SPC_XS   4     // Extra pequeno
#define SPC_SM   8     // Pequeno
#define SPC_MD   16    // Medio (padrao para padding interno)
#define SPC_LG   24    // Grande
#define SPC_XL   32    // Extra grande
#define SPC_XXL  48    // Dobrado
#define SPC_XXXL 64    // Triplo

/* ------------------------------------------------------------------------ */
/* RAIO DE BORDA (arredondamento de cantos)                                 */
/* ------------------------------------------------------------------------ */
#define RAD_SM  4      // Pequeno (inputs, badges)
#define RAD_MD  8      // Medio (botoes, cards)
#define RAD_LG  12     // Grande (cards grandes, paineis)
#define RAD_XL  16     // Extra grande (modais)
#define RAD_FULL 999   // Totalmente arredondado (pills)

/* ------------------------------------------------------------------------ */
/* TIPOGRAFIA - Tamanhos de fonte (em pixels)                               */
/* ------------------------------------------------------------------------ */
#define FONT_XS  13    // Pequena (n° das casas no tabuleiro)
#define FONT_SM  17    // Pequena (texto de dica)
#define FONT_MD  24    // Media (botoes, corpo de texto)
#define FONT_LG  26    // Grande (sub-titulos)
#define FONT_XL  36    // Extra grande (titulos de secao)
#define FONT_XXL 52    // Dobrado (titulos de tela)
#define FONT_XXXL 72   // Triplo (VENCEDOR!)
#define FONT_HERO 94   // Hero (QUACK no menu)

/* ------------------------------------------------------------------------ */
/* JANELA - Dimensoes da janela do jogo                                     */
/* ------------------------------------------------------------------------ */
// LARG e ALT removidos - usam estado->larg/alt (interface.h) ou GetScreenWidth/Height
#define JANELA_MIN_LARG 1000  // Largura minima (redimensionamento)
#define JANELA_MIN_ALT  700  // Altura minima (redimensionamento)

/* ------------------------------------------------------------------------ */
/* TABULEIRO 2D - Layout das casas no tabuleiro                             */
/* ------------------------------------------------------------------------ */
#define TAB_X_START   70        // X inicial da primeira casa (esquerda)
#define TAB_Y_FILEIRA 150       // Y da primeira fileira
#define TAB_ESPACO_Y  270       // Espacamento entre fileiras
#define TAB_CELL      78        // Largura/altura de cada casa
#define TAB_GAP       16        // Espaco entre casas
#define TAB_STEP      (TAB_CELL + TAB_GAP)  // Paso entre centros de casas
#define TAB_ROWS      3         // 3 fileiras de 10 casas
#define TAB_COLS      10        // 10 casas por fileira

/* ------------------------------------------------------------------------ */
/* FUNCOES DE COR                                                            */
/* ------------------------------------------------------------------------ */

// Clareia a cor somando 'delta' em cada canal RGB (limitado a 255)
Color cor_clarear(Color c, int delta) {
    return RGBA(
        (c.r + delta > 255) ? 255 : c.r + delta,
        (c.g + delta > 255) ? 255 : c.g + delta,
        (c.b + delta > 255) ? 255 : c.b + delta,
        c.a
    );
}

// Escurece multiplicando cada canal RGB por 'f' (0.0 = preto, 1.0 = original)
Color cor_escurecer(Color c, float f) {
    return RGBA((int)(c.r * f), (int)(c.g * f), (int)(c.b * f), c.a);
}

// Aplica um novo valor alfa (0-255) a uma cor existente
Color cor_alpha(Color c, int a) {
    return RGBA(c.r, c.g, c.b, a);
}

// Interpola linearmente entre duas cores (t=0 -> a, t=1 -> b)
Color cor_lerp(Color a, Color b, float t) {
    return RGBA(
        (int)(a.r + (b.r - a.r) * t),
        (int)(a.g + (b.g - a.g) * t),
        (int)(a.b + (b.b - a.b) * t),
        (int)(a.a + (b.a - a.a) * t)
    );
}

// Retorna a cor do jogador pelo seu indice na partida (0 a 3)
// Usado no placar e painel lateral para identificar cada jogador
Color cor_jogador(int idx) {
    Color cores[] = { COR_JOG_1, COR_JOG_2, COR_JOG_3, COR_JOG_4 };
    return cores[idx % 4];
}

// Retorna a cor do personagem pelo seu ID (1 a 4)
// Usado no token 3D e na tela de selecao de personagem
Color cor_personagem(int id) {
    switch (id) {
        case 1: return COR_PERS_1;
        case 2: return COR_PERS_2;
        case 3: return COR_PERS_3;
        case 4: return COR_PERS_4;
        default: return COR_BORDER;
    }
}

// Retorna uma cor brilhante para o pato de cada personagem (visivel com tint do DrawTexturePro)
Color cor_duck(int id) {
    switch (id) {
        case 1: return RGB(255, 255, 255);   // Patolino: branco
        case 2: return RGB(80, 140, 255);   // Pato Donald: azul vivo
        case 3: return RGB(230, 190, 60);   // Tio Patinhas: dourado
        case 4: return RGB(60, 210, 100);  // Pato: verde vivo
        default: return WHITE;
    }
}

// Retorna o nome do personagem dado seu ID (para exibicao na interface)
char* nome_personagem(int id) {
    switch (id) {
        case 1: return "Patolino";
        case 2: return "Pato Donald";
        case 3: return "Tio Patinhas";
        case 4: return "Pato";
        default: return "?";
    }
}

/* ------------------------------------------------------------------------ */
/* FUNCOES DE SOMBRA (desenha camadas de retangulo semi-transparente)       */
/* ------------------------------------------------------------------------ */

// Desenha sombra suave ao redor de um retangulo arredondado.
// Cria multiplas camadas com alfa decrescente quadraticamente para efeito
// de desfoque (drop shadow).
// r = retangulo base, radius = arredondamento, segments = segmentos da borda,
// offsetY = deslocamento vertical, alpha = intensidade maxima, layers = n° de camadas
void DrawShadow(Rectangle r, float radius, int segments, int offsetY, int alpha, int layers) {
    int i;
    for (i = layers; i >= 0; i--) {
        float f = (float)i / layers;
        int a = (int)(alpha * (1.0f - f * f));
        Rectangle sr = {
            r.x + 2 * f,
            r.y + offsetY + 2 * f,
            r.width  - 4 * f,
            r.height - 4 * f
        };
        DrawRectangleRounded(sr, radius, 24, RGBA(0, 0, 0, a));
    }
}

#endif /* TEMA_H */
