// main.c
// Novo ponto de entrada com Interface Grafica (Raylib)

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dados.h"
#include "tabuleiro.h"
#include "fila.h"
#include "estatisticas.h"

// Máquina de estados para controlar as telas do jogo
typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_PERGUNTA,
    TELA_RESULTADO,
    TELA_VITORIA
} EstadoJogo;

// Paleta de cores do tabuleiro
#define COR_FUNDO       (Color){ 30, 30, 40, 255 }
#define COR_CASA_NORMAL (Color){ 200, 200, 200, 255 }
#define COR_CASA_PERG   (Color){ 253, 249, 0, 255 }
#define COR_CASA_AVANCO (Color){ 0, 228, 48, 255 }
#define COR_CASA_RECUO  (Color){ 230, 41, 55, 255 }

// Função para desenhar o tabuleiro utilizando Lista Duplamente Encadeada
void DesenharTabuleiroGrafico(tp_tabuleiro *tabuleiro, tp_player jogadores[], int num_jogadores) {
    if (tabuleiro == NULL || tabuleiro->inicio == NULL) return;

    tp_casa *atual = tabuleiro->inicio;
    int x_inicial = 50;
    int y_inicial = 120;
    int tamanho_casa = 65;
    int espacamento = 15;
    int colunas = 10;

    while (atual != NULL) {
        int linha = (atual->numero - 1) / colunas;
        int coluna = (atual->numero - 1) % colunas;

        // Efeito de zigue-zague no tabuleiro
        if (linha % 2 != 0) {
            coluna = (colunas - 1) - coluna;
        }

        int pos_x = x_inicial + (coluna * (tamanho_casa + espacamento));
        int pos_y = y_inicial + (linha * (tamanho_casa + espacamento));

        Color cor_casa = COR_CASA_NORMAL;
        if (atual->tipo == 1) cor_casa = COR_CASA_PERG;
        else if (atual->tipo == 2) cor_casa = COR_CASA_AVANCO;
        else if (atual->tipo == 3) cor_casa = COR_CASA_RECUO;

        DrawRectangle(pos_x, pos_y, tamanho_casa, tamanho_casa, cor_casa);
        DrawRectangleLines(pos_x, pos_y, tamanho_casa, tamanho_casa, BLACK);
        DrawText(TextFormat("%d", atual->numero), pos_x + 5, pos_y + 5, 20, DARKGRAY);

        // Desenha os patos (jogadores)
        for (int i = 0; i < num_jogadores; i++) {
            if (jogadores[i].posicao == atual->numero) {
                Color cor_joga = (i == 0) ? BLUE : (i == 1) ? RED : (i == 2) ? PURPLE : ORANGE;
                DrawCircle(pos_x + 15 + (i * 12), pos_y + 45, 8, cor_joga);
            }
        }
        atual = atual->proxima_casa;
    }
}

int main(void) {
    // 1. Configuração Inicial da Raylib
    const int largura_tela = 1024;
    const int altura_tela = 768;
    InitWindow(largura_tela, altura_tela, "Quack Quiz - Boardgame");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    // 2. Variáveis do Backend 
    EstadoJogo estadoAtual = TELA_MENU;
    tp_tabuleiro tabuleiro;
    tp_player jogadores[4];
    tp_fila fila_turnos;
    tp_pilha pilhas_perguntas[3][3];
    tp_carta banco[36];
    tp_carta carta_atual;
    
    int num_jogadores = 2;
    int jogador_da_vez = 0;
    int valor_dado = 0;
    char mensagem_status[250] = "Bem-vindo! Pressione ESPACO para rolar o dado.";

    // Prepara o banco de dados no início do programa
    montar_banco_cartas(banco);
    carregar_perguntas(pilhas_perguntas, banco, 36);

    // 3. Loop do Jogo
    while (!WindowShouldClose()) {
        
        // ==========================================
        // LÓGICA DE ATUALIZAÇÃO (UPDATE)
        // ==========================================
        switch (estadoAtual) {
            case TELA_MENU:
                if (IsKeyPressed(KEY_UP) && num_jogadores < 4) num_jogadores++;
                if (IsKeyPressed(KEY_DOWN) && num_jogadores > 2) num_jogadores--;
                
                if (IsKeyPressed(KEY_ENTER)) {
                    criar_tabuleiro(&tabuleiro);
                    inicializar_jogadores(jogadores, num_jogadores);
                    colocar_jogadores_na_fila(jogadores, num_jogadores, &fila_turnos);
                    estadoAtual = TELA_JOGO;
                }
                break;

            case TELA_JOGO:
                if (IsKeyPressed(KEY_SPACE)) {
                    tp_item id_jogador;
                    if (remove_fila(&fila_turnos, &id_jogador)) {
                        jogador_da_vez = id_jogador - 1;
                        valor_dado = rolar_dado();
                        
                        // Move o jogador
                        jogadores[jogador_da_vez].posicao = mover_posicao(&tabuleiro, jogadores[jogador_da_vez].posicao, valor_dado);
                        sprintf(mensagem_status, "%s rolou %d e parou na casa %d.", jogadores[jogador_da_vez].nome, valor_dado, jogadores[jogador_da_vez].posicao);

                        // Avalia a casa atual
                        tp_casa *casa_atual = obter_casa(&tabuleiro, jogadores[jogador_da_vez].posicao);
                        
                        if (casa_atual != NULL) {
                            if (casa_atual->tipo == 1) { // Casa de Pergunta
                                int nivel = sortear_dificuldade();
                                sortear_carta(pilhas_perguntas, banco, 36, casa_atual->unidade, nivel, &carta_atual);
                                estadoAtual = TELA_PERGUNTA;
                            } else if (casa_atual->tipo == 2) { // Bônus
                                jogadores[jogador_da_vez].posicao = mover_posicao(&tabuleiro, jogadores[jogador_da_vez].posicao, casa_atual->efeito);
                                sprintf(mensagem_status, "BÔNUS! %s rolou %d e avançou mais %d casas.", jogadores[jogador_da_vez].nome, valor_dado, casa_atual->efeito);
                            } else if (casa_atual->tipo == 3) { // Penalidade
                                jogadores[jogador_da_vez].posicao = mover_posicao(&tabuleiro, jogadores[jogador_da_vez].posicao, -casa_atual->efeito);
                                sprintf(mensagem_status, "PENALIDADE! %s rolou %d e recuou %d casas.", jogadores[jogador_da_vez].nome, valor_dado, casa_atual->efeito);
                            }
                        }

                        // Verifica fim de jogo e recoloca na fila
                        if (jogadores[jogador_da_vez].posicao >= tabuleiro.total) {
                            estadoAtual = TELA_VITORIA;
                        } else if (estadoAtual != TELA_PERGUNTA) {
                            insere_fila(&fila_turnos, id_jogador);
                        }
                    }
                }
                break;

            case TELA_PERGUNTA:
                {
                    int resposta = 0;
                    if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) resposta = 1;
                    if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) resposta = 2;
                    if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) resposta = 3;
                    if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) resposta = 4;
                    if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) resposta = 5;

                    if (resposta != 0) {
                        int acertou = (resposta == carta_atual.resposta);
                        
                        // Integração com o CSV!
                        salvar_historico_resposta("Turma", jogadores[jogador_da_vez].nome, carta_atual.id_carta, carta_atual.unidade, carta_atual.tema, carta_atual.subtema, carta_atual.dificuldade, resposta, carta_atual.resposta, acertou);

                        if (acertou) {
                            sprintf(mensagem_status, "RESPOSTA CORRETA! %s avança %d casas.", jogadores[jogador_da_vez].nome, carta_atual.avanco);
                            jogadores[jogador_da_vez].posicao = mover_posicao(&tabuleiro, jogadores[jogador_da_vez].posicao, carta_atual.avanco);
                        } else {
                            sprintf(mensagem_status, "RESPOSTA ERRADA! A correta era a %d. %s recua 1 casa.", carta_atual.resposta, jogadores[jogador_da_vez].nome);
                            jogadores[jogador_da_vez].posicao = mover_posicao(&tabuleiro, jogadores[jogador_da_vez].posicao, -1);
                        }

                        // Verifica vitória pós-pergunta
                        if (jogadores[jogador_da_vez].posicao >= tabuleiro.total) {
                            estadoAtual = TELA_VITORIA;
                        } else {
                            insere_fila(&fila_turnos, jogadores[jogador_da_vez].id_player);
                            estadoAtual = TELA_RESULTADO;
                        }
                    }
                }
                break;

            case TELA_RESULTADO:
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
                    sprintf(mensagem_status, "Próximo turno! Pressione ESPACO para rolar o dado.");
                    estadoAtual = TELA_JOGO;
                }
                break;

            case TELA_VITORIA:
                if (IsKeyPressed(KEY_ENTER)) {
                    destruir_tabuleiro(&tabuleiro);
                    estadoAtual = TELA_MENU; // Reinicia
                }
                break;
        }

        // ==========================================
        // RENDERIZAÇÃO VISUAL (DRAW)
        // ==========================================
        BeginDrawing();
        ClearBackground(COR_FUNDO);

        switch (estadoAtual) {
            case TELA_MENU:
                DrawText("🦆 QUACK QUIZ", 350, 200, 50, YELLOW);
                DrawText(TextFormat("Jogadores: %d  (Use Setas Cima/Baixo)", num_jogadores), 350, 350, 20, LIGHTGRAY);
                DrawText("Pressione [ENTER] para Iniciar", 350, 450, 20, WHITE);
                break;

            case TELA_JOGO:
                DrawText(TextFormat("Turno de: %s", jogadores[jogador_da_vez].nome), 50, 20, 30, SKYBLUE);
                DrawText(mensagem_status, 50, 60, 20, YELLOW);
                
                DesenharTabuleiroGrafico(&tabuleiro, jogadores, num_jogadores);
                
                // Legenda
                DrawRectangle(50, 650, 20, 20, COR_CASA_PERG);
                DrawText("Pergunta (Amarelo)", 80, 650, 20, WHITE);
                DrawRectangle(300, 650, 20, 20, COR_CASA_AVANCO);
                DrawText("Bônus (Verde)", 330, 650, 20, WHITE);
                DrawRectangle(500, 650, 20, 20, COR_CASA_RECUO);
                DrawText("Penalidade (Vermelho)", 530, 650, 20, WHITE);
                break;

            case TELA_PERGUNTA:
                DrawRectangle(50, 50, largura_tela - 100, altura_tela - 100, RAYWHITE);
                DrawRectangleLines(50, 50, largura_tela - 100, altura_tela - 100, BLACK);
                
                DrawText(TextFormat("Tema: %s | Dificuldade: %d", carta_atual.tema, carta_atual.dificuldade), 80, 80, 20, DARKGRAY);
                DrawText(carta_atual.pergunta, 80, 130, 24, BLACK);
                
                for(int i = 0; i < 5; i++) {
                    DrawText(TextFormat("%d) %s", i+1, carta_atual.alternativas[i]), 80, 200 + (i * 60), 22, DARKBLUE);
                }
                DrawText("Pressione de 1 a 5 no teclado para responder.", 80, 650, 20, RED);
                break;

            case TELA_RESULTADO:
                DrawText("RESULTADO DA PERGUNTA", 50, 50, 30, WHITE);
                DrawText(mensagem_status, 50, 150, 25, YELLOW);
                DrawText("Pressione [ESPACO] para continuar o jogo", 50, 250, 20, LIGHTGRAY);
                break;

            case TELA_VITORIA:
                DrawText("🏆 FIM DE JOGO!", 350, 250, 50, GREEN);
                DrawText(TextFormat("%s CHEGOU À CASA FINAL!", jogadores[jogador_da_vez].nome), 250, 350, 30, YELLOW);
                DrawText("Pressione [ENTER] para voltar ao Menu", 300, 450, 20, WHITE);
                break;
        }

        EndDrawing();
    }

    if (estadoAtual != TELA_MENU) destruir_tabuleiro(&tabuleiro);
    CloseWindow();
    return 0;
}
