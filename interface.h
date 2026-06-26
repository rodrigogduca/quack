// interface.h
// Declaracoes da interface grafica principal do QUACK Boardgame.
// Define o enum de telas, o estado global do jogo (Estado) e as funcoes
// de desenho para cada tela (menu, jogo, pergunta, vencedor, etc.).

#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "raylib.h"
#include "jogador.h"
#include "tabuleiro.h"
#include "historico.h"
#include "tema.h"
#include "componentes.h"
#include "animacao.h"

#define QTD_CASAS 30  // Numero total de casas no tabuleiro

// Enum das telas (estados da interface)
typedef enum {
    TELA_MENU,          // Menu principal
    TELA_NUM_JOG,       // Selecao do numero de jogadores
    TELA_PERSONAGEM,    // Selecao de personagem para cada jogador
    TELA_JOGO,          // Tela principal do jogo (tabuleiro 2D + painel)
    TELA_PERGUNTA,      // Tela de pergunta (carta sorteada)
    TELA_RESULTADO,     // Resultado da resposta (acertou/errou)
    TELA_VENCEDOR,      // Tela de vitoria com ranking e confetes
    TELA_PAUSE,         // Tela de pausa (continuar, menu, sair)
    TELA_HISTORICO      // Historico de respostas salvas em CSV
} Tela;

// Estrutura que agrupa TODO o estado da partida e da interface
typedef struct {
    // Jogadores e turnos
    tp_player jogadores[4];         // Dados dos 4 jogadores (posicao, nome, historico)
    tp_fila fila_turnos;            // Fila circular com a ordem dos turnos
    tp_pilha pilhas[3][3];          // Pilhas de cartas: [unidade-1][dificuldade-1]
    tp_carta banco[36];             // Banco de 36 cartas-pergunta
    tp_tabuleiro tabuleiro;         // Tabuleiro (lista duplamente encadeada de 30 casas)

    // Estado da partida
    int num_jogadores;              // Quantidade de jogadores (2 a 4)
    int jogador_atual;              // Indice do jogador da vez
    int rodada;                     // Rodada atual (1-indexada)
    int turnos;                     // Total de turnos ja jogados
    int partida_ativa;              // 1 enquanto a partida esta em andamento
    int ultimo_dado;                // Ultimo valor rolado no dado (1 a 6)
    int aguardando_dado;            // 1 enquanto o jogador precisa rolar o dado

    // Estado da pergunta
    int resposta_escolhida;         // Alternativa escolhida pelo jogador (1 a 5)
    int resultado_perg;             // >0 se acertou (avancar N casas), -1 se errou
    tp_carta carta_atual;           // Carta sorteada na pergunta atual

    // Personagens
    int personagem_idx;             // Indice do personagem sendo selecionado
    int personagens_escolhidos[4];  // IDs dos personagens escolhidos (1 a 4)

    // Navegacao de telas
    Tela tela;                      // Tela atual sendo exibida

    // Historico de respostas (CSV)
    char hist_buf[15000];           // Buffer para ler o arquivo CSV
    int hist_pronto;                // 1 quando o historico ja foi carregado

    // Navegacao
    int tela_anterior;              // Tela anterior (para voltar do pause)

    // Fontes
    Font font_titulo;               // Fonte personalizada para titulos
    Font font_corpo;                // Fonte personalizada para corpo
    int font_carregada;             // 1 se as fontes foram carregadas com sucesso

    // Animacoes e efeitos
    ParticlePool particles;         // Pool de particulas (resultado, explosao)
    double ultima_particula;        // Timestamp da ultima particula emitida
    ConfetePool confetes;           // Pool de confetes (tela de vencedor)
    int confete_iniciado;           // 1 quando os confetes ja foram inicializados
    Toast toast;                    // Notificacao toast
    Tween transicao;                // Tween de transicao entre telas (fade)
    int transicao_ativa;            // 1 enquanto a transicao esta rodando
    Color transicao_cor;            // Cor do fade de transicao
    Tween dado_tween;               // Tween para animacao do dado
    int dado_valor_exibido;         // Valor exibido durante animacao do dado

    // Musica de fundo
    Music musica;                   // Musica de fundo do jogo (streaming OGG)

    // Efeitos sonoros
    Sound som_clique;              // Som de clique para botoes
    Sound som_fail;                // Som de fail (casa vermelha / erro)
    Sound som_acerto;             // Som de comemoração (acerto pergunta)
    Sound som_alerta;             // Som de alerta pin pin (casa pergunta)

    // Recursos graficos
    Texture2D textura_pato;         // Textura do pato (pato.png)

    // Dimensoes da janela (mudam com fullscreen F11)
    int larg;                       // Largura atual da janela
    int alt;                        // Altura atual da janela

    // Acessibilidade
    int reduced_motion;             // 1 se animacoes reduzidas estao ativas (Ctrl+R)
} Estado;

// Verifica se o mouse esta sobre um retangulo
int mouse_em(Rectangle r);

// Desenha texto com a fonte carregada (ComicNeue se disponivel)
void txt_draw(Estado *estado, char *texto, int x, int y, int tamanho, Color cor);

// Mede largura do texto com a fonte carregada
int txt_measure(Estado *estado, char *texto, int tamanho);

// Desenha texto centralizado horizontalmente
void txt_centro(Estado *estado, char *texto, int y, int tamanho, Color cor);

// Desenha texto com quebra de linha automatica (word wrap)
void txt_wrap(Estado *estado, char *texto, int x, int y, int largura_max, int tamanho, Color cor);

// Inicia uma transicao de fade entre telas
void iniciar_transicao(Estado *estado);

// Desenha a transicao (fade preto) e atualiza seu progresso
void draw_transicao(Estado *estado, float dt);

// Desenha o mascote pato em 2D com suporte a rotacao e animacao
void draw_pato(Estado *estado, int x, int y, float escala, Color corpo, int angulo);

// Reinicia o estado para uma nova partida
void reiniciar(Estado *estado);

// Inicia a partida apos a selecao de personagens
void comecar(Estado *estado);

// Avanca para o proximo jogador na fila de turnos
void avancar_turno(Estado *estado);

// Verifica se um ID de personagem ja foi escolhido
int disponivel(Estado *estado, int id);

// Funcoes de desenho de cada tela
void draw_menu(Estado *estado);          // Tela: menu principal
void draw_num_jog(Estado *estado);       // Tela: numero de jogadores
void draw_personagem(Estado *estado);    // Tela: selecao de personagem
void draw_board(Estado *estado);         // Tabuleiro 2D com casas e jogadores
void draw_dado(int centro_x, int centro_y, int tamanho, int valor);  // Dado 2D
void draw_painel(Estado *estado);        // Painel lateral da tela de jogo
void draw_jogo(Estado *estado);          // Tela: jogo (tabuleiro + painel)
void draw_pergunta(Estado *estado);      // Tela: pergunta com alternativas
void draw_resultado(Estado *estado);     // Tela: resultado da pergunta
void draw_vencedor(Estado *estado);      // Tela: vencedor com ranking e confetes
void draw_pause(Estado *estado);         // Tela: pausa
void draw_historico(Estado *estado);     // Tela: historico de respostas

// Funcao principal da interface grafica (loop do jogo)
void gui_main(void);

#endif
