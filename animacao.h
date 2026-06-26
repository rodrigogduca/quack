#ifndef ANIMACAO_H
#define ANIMACAO_H

#include "raylib.h"
#include <math.h>

// animacao.h
// Sistema de animacao do QUACK Boardgame: funcoes de easing (interpolacao),
// tweens (animacao generica de valores), particulas (explosoes) e confetes
// (chuva infinita para tela de vencedor). Tudo header-only (inline).

// ========================================================================
// SISTEMA DE ANIMACAO - Easing, Tween, Particulas
// ========================================================================

// ------------------------------------------------------------------------
// EASING FUNCTIONS - Recebem t (0..1) e retornam valor interpolado (0..1)
// ------------------------------------------------------------------------
// Linear: sem suavizacao
float ease_linear(float t)       { return t; }
// Quadratica: aceleracao suave (entrada)
float ease_quad_in(float t)      { return t * t; }
// Quadratica: desaceleracao suave (saida)
float ease_quad_out(float t)     { return t * (2 - t); }
// Cubica: aceleracao mais pronunciada (entrada)
float ease_cubic_in(float t)     { return t * t * t; }
// Cubica: desaceleracao mais pronunciada (saida)
float ease_cubic_out(float t)    { float f = t - 1; return f * f * f + 1; }
// Senoidal: aceleracao suave (entrada)
float ease_sine_in(float t)      { return 1 - cosf(t * PI / 2); }
// Senoidal: desaceleracao suave (saida)
float ease_sine_out(float t)     { return sinf(t * PI / 2); }
// Bounce: efeito de quique ao final (saida)
float ease_bounce_out(float t) {
    if (t < 1/2.75f)      return 7.5625f * t * t;
    else if (t < 2/2.75f) { t -= 1.5f/2.75f; return 7.5625f * t * t + 0.75f; }
    else if (t < 2.5/2.75f){ t -= 2.25f/2.75f; return 7.5625f * t * t + 0.9375f; }
    else                  { t -= 2.625f/2.75f; return 7.5625f * t * t + 0.984375f; }
}
// Elastico: ultrapassa o valor final com oscilacao (saida)
float ease_elastic_out(float t) {
    if (t == 0 || t == 1) return t;
    return powf(2, -10 * t) * sinf((t - 0.075f) * (2 * PI) / 0.3f) + 1;
}

/* ------------------------------------------------------------------------ */
/* TWEEN - Animacao generica de um valor float                              */
/* ------------------------------------------------------------------------ */

// Estrutura que anima um valor float entre 'start' e 'end' ao longo de 'duration'
// usando uma funcao de easing personalizada.
typedef struct {
    float start, end, duration, elapsed;  // Estado da animacao
    float (*ease)(float);                 // Funcao de easing (interpolacao)
    int active;                           // 1 enquanto a animacao esta rodando
} Tween;

// Inicia um tween com os parametros especificados
void tween_start(Tween *t, float start, float end, float duration, float (*ease)(float)) {
    t->start = start;
    t->end = end;
    t->duration = duration;
    t->elapsed = 0;
    t->ease = ease ? ease : ease_linear;
    t->active = 1;
}

// Atualiza o tween com o delta time. Retorna o valor interpolado atual.
// Quando a duracao se esgota, marca como inativo e retorna o valor final.
float tween_update(Tween *t, float dt) {
    if (!t->active) return t->end;
    t->elapsed += dt;
    float p = (t->duration > 0) ? fminf(t->elapsed / t->duration, 1.0f) : 1.0f;
    float v = t->start + (t->end - t->start) * t->ease(p);
    if (p >= 1.0f) {
        v = t->end;
        t->active = 0;
    }
    return v;
}

// Retorna 1 se o tween ja terminou, 0 caso ainda esteja ativo
int tween_done(Tween *t) {
    return !t->active;
}

/* ------------------------------------------------------------------------ */
/* SISTEMA DE PARTICULAS SIMPLES (pool estatico de ate 120 particulas)      */
/* ------------------------------------------------------------------------ */

// Uma particula individual com posicao, velocidade, cor, tempo de vida e tamanho
typedef struct {
    Vector2 pos;        // Posicao atual (x, y)
    Vector2 vel;        // Velocidade (pixels/s)
    Color cor;          // Cor da particula
    float vida;         // Tempo de vida restante (segundos)
    float vida_max;     // Tempo de vida inicial (para calcular fade)
    float tamanho;      // Raio em pixels
    int ativo;          // 1 se a particula esta ativa
} Particula;

// Tamanho maximo do pool de particulas
#define MAX_PARTICULAS 120

// Pool estatico de particulas (pre-alocado, sem malloc)
typedef struct {
    Particula p[MAX_PARTICULAS];  // Vetor de particulas
    int count;                    // Quantidade de particulas ativas no momento
} ParticlePool;

// Inicializa o pool zerando o contador de ativas
void particle_init(ParticlePool *pool) {
    pool->count = 0;
}

// Cria uma particula no pool (procura a primeira inativa)
void particle_emit(ParticlePool *pool, Vector2 pos, Vector2 vel, Color cor, float vida, float tamanho) {
    int i;
    for (i = 0; i < MAX_PARTICULAS; i++) {
        if (!pool->p[i].ativo) {
            pool->p[i].pos = pos;
            pool->p[i].vel = vel;
            pool->p[i].cor = cor;
            pool->p[i].vida = vida;
            pool->p[i].vida_max = vida;
            pool->p[i].tamanho = tamanho;
            pool->p[i].ativo = 1;
            pool->count++;
            return;
        }
    }
}

// Cria uma explosao de 'quantidade' particulas em circulo com direcao aleatoria
void particle_burst(ParticlePool *pool, Vector2 pos, Color cor, int quantidade, float forca) {
    int i;
    for (i = 0; i < quantidade; i++) {
        float ang = (float)rand() / RAND_MAX * 2 * PI;     // Direcao aleatoria
        float vel = ((float)rand() / RAND_MAX) * forca;    // Velocidade aleatoria
        Vector2 v = { cosf(ang) * vel, sinf(ang) * vel - forca * 0.3f };
        float vida = 0.5f + ((float)rand() / RAND_MAX) * 1.0f;  // 0.5 a 1.5s
        float tam = 2.0f + ((float)rand() / RAND_MAX) * 4.0f;   // 2 a 6 pixels
        particle_emit(pool, pos, v, cor, vida, tam);
    }
}

// Atualiza posicao (com gravidade), vida e desenha todas as particulas ativas
void particle_update_and_draw(ParticlePool *pool, float dt) {
    int i;
    for (i = 0; i < MAX_PARTICULAS; i++) {
        if (!pool->p[i].ativo) continue;
        // Movimento com aceleracao gravitacional
        pool->p[i].pos.x += pool->p[i].vel.x * dt;
        pool->p[i].pos.y += pool->p[i].vel.y * dt;
        pool->p[i].vel.y += 120 * dt; // Gravidade: 120 px/s^2
        pool->p[i].vida -= dt;
        // Se morreu, desativa e continua
        if (pool->p[i].vida <= 0) {
            pool->p[i].ativo = 0;
            pool->count--;
            continue;
        }
        // Fade-out baseado na vida restante
        float alpha_f = pool->p[i].vida / pool->p[i].vida_max;
        Color c = pool->p[i].cor;
        DrawCircle((int)pool->p[i].pos.x, (int)pool->p[i].pos.y,
                   pool->p[i].tamanho, RGBA(c.r, c.g, c.b, (int)(alpha_f * 255)));
    }
}

/* ------------------------------------------------------------------------ */
/* CONFETES INFINITOS PARA TELA DE VENCEDOR                                 */
/* ------------------------------------------------------------------------ */

// Um confete individual com fisica, rotacao e reposicao automatica
typedef struct {
    float x, y;            // Posicao na tela
    float vel_x, vel_y;    // Velocidade (pixels/s)
    float rot, rot_vel;    // Rotacao atual e velocidade angular
    float tamanho;         // Tamanho do retangulo
    Color cor;             // Cor do confete
    int ativo;             // 1 se esta ativo
} Confete;

// Numero maximo de confetes simultaneos
#define MAX_CONFETES 80

// Pool de confetes (chuva infinita na tela de vencedor)
typedef struct {
    Confete c[MAX_CONFETES];
} ConfetePool;

// Inicializa todos os confetes com posicoes e velocidades aleatorias
void confete_init(ConfetePool *pool) {
    int i;
    for (i = 0; i < MAX_CONFETES; i++) {
        pool->c[i].x = (float)(rand() % GetScreenWidth());
        pool->c[i].y = (float)(rand() % GetScreenHeight());
        pool->c[i].vel_x = -30 + (float)(rand() % 60);
        pool->c[i].vel_y = 40 + (float)(rand() % 80);
        pool->c[i].rot = (float)(rand() % 360);
        pool->c[i].rot_vel = -180 + (float)(rand() % 360);
        pool->c[i].tamanho = 3 + (rand() % 4);
        pool->c[i].ativo = 1;
        int ci = rand() % 4;
        Color cores[] = { COR_PRIMARY, COR_WARNING, COR_SUCCESS, COR_DANGER };
        pool->c[i].cor = cores[ci];
    }
}

// Atualiza fisica e reposicao, e desenha todos os confetes como retangulos rotacionados
void confete_update_and_draw(ConfetePool *pool, float dt) {
    int i;
    for (i = 0; i < MAX_CONFETES; i++) {
        // Fisica: movimento com gravidade
        pool->c[i].x += pool->c[i].vel_x * dt;
        pool->c[i].y += pool->c[i].vel_y * dt;
        pool->c[i].vel_y += 60 * dt;  // Gravidade mais suave que particulas
        pool->c[i].rot += pool->c[i].rot_vel * dt;
        // Reposicao ao sair da tela (ciclo infinito)
        if (pool->c[i].y > GetScreenHeight() + 20) {
            pool->c[i].y = -20;
            pool->c[i].x = (float)(rand() % GetScreenWidth());
            pool->c[i].vel_y = 40 + (float)(rand() % 80);
        }
        if (pool->c[i].x < -20) pool->c[i].x = GetScreenWidth() + 20;
        if (pool->c[i].x > GetScreenWidth() + 20) pool->c[i].x = -20;

        // Desenha confete como retangulo rotacionado (efeito de papel caindo)
        Rectangle r = { pool->c[i].x, pool->c[i].y, pool->c[i].tamanho, pool->c[i].tamanho * 0.6f };
        DrawRectanglePro(r, (Vector2){ r.width/2, r.height/2 }, pool->c[i].rot, pool->c[i].cor);
    }
}

#endif /* ANIM_H */
