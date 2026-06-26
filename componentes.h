#ifndef COMPONENTES_INTERFACE_H
#define COMPONENTES_INTERFACE_H

#include "raylib.h"
#include <string.h>
#include <math.h>

#include "tema.h"

// componentes_interface.h
// Sistema de componentes de interface reutilizaveis para o QUACK Boardgame.
// Implementa botoes, cards, avatares, barras de progresso, badges, toasts
// e campos de texto. Todos sao header-only (inline) para facil uso.

// ========================================================================
// SISTEMA DE COMPONENTES REUTILIZAVEIS
// ========================================================================

// ------------------------------------------------------------------------
// BOTAO - Variantes e estados
// ------------------------------------------------------------------------

// Variantes visuais do botao
typedef enum {
    BTN_PRIMARY,    // Fundo laranja, texto branco (acao principal)
    BTN_SECONDARY,  // Fundo escuro, borda laranja (acao secundaria)
    BTN_GHOST,      // Sem fundo, texto laranja (acao terciaria)
    BTN_DANGER,     // Fundo vermelho, texto branco (acao destrutiva)
    BTN_SUCCESS     // Fundo verde, texto branco (confirmacao)
} ButtonVariant;

// Estrutura completa de um botao com suporte a animacao de clique
typedef struct {
    Rectangle rect;       // Area de clique e desenho
    char *texto;    // Rotulo do botao
    ButtonVariant variant; // Estilo visual
    int hovered;          // 1 se o mouse esta sobre o botao
    int pressed;          // 1 se o botao esta pressionado
    float scale_anim;     // Fator de escala para animacao de clique
} Button;

// Inicializa um botao com valores padrao
void btn_init(Button *b, Rectangle rect, char *texto, ButtonVariant variant) {
    b->rect = rect;
    b->texto = texto;
    b->variant = variant;
    b->hovered = 0;
    b->pressed = 0;
    b->scale_anim = 1.0f;
}

// Retorna as cores (fundo, texto, borda) para uma variante e estado (hover ou nao)
void btn_cores(ButtonVariant v, int hovered, Color *fundo, Color *texto, Color *borda) {
    switch (v) {
        case BTN_PRIMARY:
            *fundo  = hovered ? cor_clarear(COR_PRIMARY, 30) : COR_PRIMARY;
            *texto  = WHITE;
            *borda  = hovered ? COR_PRIMARY_LIGHT : RGBA(255,255,255,80);
            break;
        case BTN_SECONDARY:
            *fundo  = hovered ? COR_SURFACE_ALT : COR_SURFACE;
            *texto  = hovered ? COR_PRIMARY_LIGHT : COR_TEXT;
            *borda  = hovered ? COR_PRIMARY_LIGHT : COR_PRIMARY_MUTED;
            break;
        case BTN_GHOST:
            *fundo  = hovered ? RGBA(255,102,0,30) : BLANK;
            *texto  = hovered ? COR_PRIMARY_LIGHT : COR_PRIMARY;
            *borda  = BLANK;
            break;
        case BTN_DANGER:
            *fundo  = hovered ? cor_clarear(COR_DANGER, 30) : COR_DANGER;
            *texto  = WHITE;
            *borda  = hovered ? RGBA(255,255,255,100) : RGBA(255,255,255,40);
            break;
        case BTN_SUCCESS:
            *fundo  = hovered ? cor_clarear(COR_SUCCESS, 30) : COR_SUCCESS;
            *texto  = WHITE;
            *borda  = hovered ? RGBA(255,255,255,100) : RGBA(255,255,255,40);
            break;
    }
}

// Desenha o botao na tela, gerencia hover e animacao de clique.
// Retorna 1 se o botao foi clicado neste frame, 0 caso contrario.
int btn_draw(Button *b) {
    // Verifica se o mouse esta sobre o botao
    b->hovered = CheckCollisionPointRec(GetMousePosition(), b->rect);
    int clicked = b->hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // Animacao de escala: encolhe levemente ao pressionar
    if (b->hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        b->scale_anim = 0.95f;
    else
        b->scale_anim = 1.0f;

    Color fundo, texto, borda;
    btn_cores(b->variant, b->hovered, &fundo, &texto, &borda);

    // Aplica o fator de escala centralizado
    float sx = b->rect.width  * (1 - b->scale_anim) / 2;
    float sy = b->rect.height * (1 - b->scale_anim) / 2;
    Rectangle r = {
        b->rect.x + sx,
        b->rect.y + sy,
        b->rect.width  * b->scale_anim,
        b->rect.height * b->scale_anim
    };

    // Desenha sombra, fundo arredondado e borda
    DrawShadow(r, RAD_MD, 24, 3, 40, 4);
    DrawRectangleRounded(r, RAD_MD, 24, fundo);
    if (borda.a > 0) {
        DrawRectangleRoundedLines(r, RAD_MD, 24, borda);
    }

    // Desenha o texto centralizado
    int tam = (int)((b->rect.height > 40) ? FONT_MD : FONT_SM);
    int tx = (int)(r.x + r.width / 2 - MeasureText(b->texto, tam) / 2);
    int ty = (int)(r.y + r.height / 2 - tam / 2);
    DrawText(b->texto, tx, ty, tam, texto);

    return clicked;
}

// Atalho para criar e desenhar um botao em uma unica linha.
// Uso tipico: if (botao((Rectangle){x,y,w,h}, "Texto", BTN_PRIMARY)) { ... }
int botao(Rectangle r, char *txt, ButtonVariant variant, Sound *som_clique) {
    Button b;
    btn_init(&b, r, txt, variant);
    int clicked = btn_draw(&b);
    if (clicked && som_clique) PlaySound(*som_clique);
    return clicked;
}

/* ------------------------------------------------------------------------ */
/* CARD - Container com padding, sombra e hover elevado                     */
/* ------------------------------------------------------------------------ */

// Estrutura de um card com efeito de elevacao no hover
typedef struct {
    Rectangle rect;        // Area do card
    Color cor_fundo;       // Cor de fundo
    Color cor_borda;       // Cor da borda
    float radius;          // Raio de arredondamento
    int elevado;           // 1 = ativa efeito de levantar no hover
    float hover_offset;    // Quantidade de pixels para levantar (negativo = sobe)
} Card;

// Inicializa um card com valores padrao (elevado = 1, offset = -3)
void card_init(Card *c, Rectangle rect, Color fundo, Color borda, float radius) {
    c->rect = rect;
    c->cor_fundo = fundo;
    c->cor_borda = borda;
    c->radius = radius;
    c->elevado = 1;
    c->hover_offset = -3;
}

// Desenha o card com sombra dinamica e efeito de elevacao no hover
void card_draw(Card *c) {
    int hovered = CheckCollisionPointRec(GetMousePosition(), c->rect);
    float offset = (c->elevado && hovered) ? c->hover_offset : 0;
    Rectangle r = { c->rect.x, c->rect.y + offset, c->rect.width, c->rect.height };

    DrawShadow(r, c->radius, 24, hovered ? 6 : 3, hovered ? 60 : 30, 5);
    DrawRectangleRounded(r, c->radius, 24, c->cor_fundo);
    if (c->cor_borda.a > 0)
        DrawRectangleRoundedLines(r, c->radius, 24, c->cor_borda);
}

/* ------------------------------------------------------------------------ */
/* AVATAR - Circulo com cor de fundo e iniciais do nome                     */
/* ------------------------------------------------------------------------ */

// Desenha um avatar circular com sombra, cor de fundo e iniciais centralizadas
void avatar_draw(Vector2 centro, float raio, Color cor_fundo, char *iniciais) {
    DrawShadow((Rectangle){ centro.x - raio, centro.y - raio, raio*2, raio*2 }, raio, 12, 3, 40, 3);
    DrawCircle((int)centro.x, (int)centro.y, raio, cor_fundo);
    DrawCircleLines((int)centro.x, (int)centro.y, raio, cor_clarear(cor_fundo, -40));
    if (iniciais && iniciais[0]) {
        int tam = (int)(raio * 0.7f);
        if (tam < 8) tam = 8;
        char *nome = iniciais;
        DrawText(nome, (int)(centro.x - MeasureText(nome, tam)/2),
                 (int)(centro.y - tam/2), tam, WHITE);
    }
}

/* ------------------------------------------------------------------------ */
/* PROGRESS BAR - Barra de progresso horizontal animada                     */
/* ------------------------------------------------------------------------ */

// Estrutura da barra de progresso com animacao suave (lerp)
typedef struct {
    Rectangle rect;        // Area da barra
    float progresso;       // Valor alvo (0.0 a 1.0)
    Color cor_barra;       // Cor do preenchimento
    Color cor_fundo;       // Cor do fundo (vazio)
    char *label;     // Rotulo opcional centralizado
    float anim_progresso;  // Valor atual animado (interpolado suavemente)
} ProgressBar;

// Inicializa a barra de progresso com valores padrao
void pbar_init(ProgressBar *p, Rectangle rect, Color cor) {
    p->rect = rect;
    p->progresso = 0;
    p->cor_barra = cor;
    p->cor_fundo = COR_SURFACE_ALT;
    p->label = NULL;
    p->anim_progresso = 0;
}

// Desenha a barra de progresso com animacao suave (lerp de 10% por frame)
void pbar_draw(ProgressBar *p) {
    // Interpolacao linear suave em direcao ao valor alvo
    p->anim_progresso += (p->progresso - p->anim_progresso) * 0.1f;
    if (fabsf(p->anim_progresso - p->progresso) < 0.001f)
        p->anim_progresso = p->progresso;

    float val = p->anim_progresso;
    if (val < 0) val = 0;
    if (val > 1) val = 1;

    // Fundo (vazio)
    DrawRectangleRounded(p->rect, RAD_FULL, 24, p->cor_fundo);
    // Preenchimento
    Rectangle fill = { p->rect.x, p->rect.y, p->rect.width * val, p->rect.height };
    if (val > 0.01f) {
        DrawRectangleRounded(fill, RAD_FULL, 24, p->cor_barra);
    }
    // Rotulo opcional
    if (p->label) {
        int tam = (int)(p->rect.height * 0.7f);
        if (tam < 10) tam = 10;
        int tx = (int)(p->rect.x + p->rect.width / 2 - MeasureText(p->label, tam) / 2);
        int ty = (int)(p->rect.y + p->rect.height / 2 - tam / 2);
        DrawText(p->label, tx, ty, tam, WHITE);
    }
}

/* ------------------------------------------------------------------------ */
/* BADGE - Tag colorida para informacoes curtas                             */
/* ------------------------------------------------------------------------ */

// Variantes de cor para badges
typedef enum {
    BADGE_INFO,     // Azul informativo
    BADGE_SUCCESS,  // Verde sucesso
    BADGE_WARNING,  // Amarelo atencao
    BADGE_DANGER,   // Vermelho perigo
    BADGE_PRIMARY   // Laranja principal
} BadgeVariant;

// Retorna a cor associada a uma variante de badge
Color badge_cor(BadgeVariant v) {
    switch (v) {
        case BADGE_INFO:    return COR_INFO;
        case BADGE_SUCCESS: return COR_SUCCESS;
        case BADGE_WARNING: return COR_WARNING;
        case BADGE_DANGER:  return COR_DANGER;
        case BADGE_PRIMARY: return COR_PRIMARY;
        default: return COR_BORDER;
    }
}

// Desenha um badge (tag) na posicao especificada com texto e cor
void badge_draw(Vector2 pos, char *texto, BadgeVariant variant) {
    Color c = badge_cor(variant);
    int tam = FONT_XS;
    int larg = MeasureText(texto, tam) + SPC_MD;
    int alt = tam + SPC_SM;
    Rectangle r = { pos.x, pos.y, larg, alt };
    DrawRectangleRounded(r, RAD_FULL, 24, cor_alpha(c, 220));
    DrawText(texto, (int)(pos.x + SPC_SM), (int)(pos.y + SPC_XS), tam, WHITE);
}

/* ------------------------------------------------------------------------ */
/* TOAST - Notificacao temporaria (auto-destroi apos duracao)               */
/* ------------------------------------------------------------------------ */

// Estrutura de um toast (notificacao que aparece e desaparece)
typedef struct {
    char mensagem[200];   // Texto da notificacao
    float duracao;        // Tempo total de exibicao (segundos)
    float elapsed;        // Tempo ja decorrido
    int ativo;            // 1 enquanto deve ser exibido
} Toast;

// Ativa um toast com a mensagem e duracao especificadas
void toast_show(Toast *t, char *msg, float duracao) {
    strncpy(t->mensagem, msg, sizeof(t->mensagem) - 1);
    t->mensagem[sizeof(t->mensagem) - 1] = '\0';
    t->duracao = duracao;
    t->elapsed = 0;
    t->ativo = 1;
}

// Atualiza o temporizador e desenha o toast. Faz fade-out nos ultimos 0.5s.
void toast_update_and_draw(Toast *t, float dt) {
    if (!t->ativo) return;
    t->elapsed += dt;
    // Calcula alfa para fade-out nos ultimos 0.5 segundos
    float alpha = 1.0f;
    if (t->elapsed > t->duracao - 0.5f) {
        alpha = (t->duracao - t->elapsed) / 0.5f;
    }
    if (t->elapsed >= t->duracao) {
        t->ativo = 0;
        return;
    }
    if (alpha < 0) alpha = 0;

    // Posiciona centralizado na parte inferior da tela
    int tam = FONT_MD;
    int larg = MeasureText(t->mensagem, tam) + SPC_XL;
    int alt = tam + SPC_MD;
    int x = (GetScreenWidth() - larg) / 2;
    int y = GetScreenHeight() - alt - SPC_LG;

    Rectangle r = { x, y, larg, alt };
    DrawShadow(r, RAD_MD, 24, 3, 50, 4);
    DrawRectangleRounded(r, RAD_MD, 24, COR_SURFACE);
    DrawRectangleRoundedLines(r, RAD_MD, 24, cor_alpha(COR_PRIMARY, (int)(alpha * 150)));

    int tx = x + SPC_MD;
    int ty = y + SPC_SM;
    DrawText(t->mensagem, tx, ty, tam, cor_alpha(COR_TEXT, (int)(alpha * 255)));
}

/* ------------------------------------------------------------------------ */
/* INPUT - Campo de texto simples para entrada do usuario                   */
/* ------------------------------------------------------------------------ */

// Estrutura de um campo de texto com cursor e gerenciamento de foco
typedef struct {
    Rectangle rect;       // Area do campo
    char buffer[256];     // Texto digitado
    int cursor_pos;       // Posicao do cursor (proximo caractere)
    int focado;           // 1 se o campo esta recebendo entrada
} TextInput;

// Inicializa um campo de texto vazio
void input_init(TextInput *in, Rectangle rect) {
    in->rect = rect;
    in->buffer[0] = '\0';
    in->cursor_pos = 0;
    in->focado = 0;
}

// Atualiza (captura teclas) e desenha o campo de texto.
// Retorna 1 se o usuario pressionou Enter (confirmacao).
int input_update_and_draw(TextInput *in) {
    int confirm = 0;
    // Ativa foco se clicou no campo
    in->focado = CheckCollisionPointRec(GetMousePosition(), in->rect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (in->focado) {
        // Captura caracteres imprimiveis (codigo 32 a 126)
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 126 && in->cursor_pos < 255) {
                in->buffer[in->cursor_pos++] = (char)key;
                in->buffer[in->cursor_pos] = '\0';
            }
            key = GetCharPressed();
        }
        // Backspace apaga o ultimo caractere
        if (IsKeyPressed(KEY_BACKSPACE) && in->cursor_pos > 0) {
            in->buffer[--in->cursor_pos] = '\0';
        }
        // Enter confirma e perde o foco
        if (IsKeyPressed(KEY_ENTER)) {
            confirm = 1;
            in->focado = 0;
        }
    }

    // Desenha o campo com sombra, fundo e borda
    DrawShadow(in->rect, RAD_SM, 24, 2, 30, 4);
    Color fundo = in->focado ? COR_SURFACE_LIGHT : COR_SURFACE;
    DrawRectangleRounded(in->rect, RAD_SM, 24, fundo);
    Color borda = in->focado ? COR_PRIMARY : COR_BORDER;
    DrawRectangleRoundedLines(in->rect, RAD_SM, 24, borda);

    // Texto digitado
    int tam = FONT_MD;
    int ty = (int)(in->rect.y + in->rect.height / 2 - tam / 2);
    DrawText(in->buffer, (int)(in->rect.x + SPC_SM), ty, tam, COR_TEXT);

    // Cursor piscante (alterna a cada 0.5s) quando focado
    if (in->focado && ((int)(GetTime() * 2) % 2)) {
        int cx = (int)(in->rect.x + SPC_SM + MeasureText(in->buffer, tam));
        DrawLine(cx, ty, cx, ty + tam, COR_TEXT);
    }

    return confirm;
}

#endif /* UI_H */
