#include "pilha.h"
#include "fila.h"
#include "menu.h"
#include "dados.h"
#include <locale.h>
#include <time.h>

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "Portuguese");
    
    int opcao_menu;
    int num_jogadores;
    int jogador_atual = 0;
    int rodada = 1;
    int jogo_ativo = 1;
    
    // Fila para controlar a ordem dos turnos
    tp_fila fila_turnos;
    
    // Loop principal do jogo
    while (jogo_ativo) {
        opcao_menu = menu_principal();
        
        if (opcao_menu == 2) {
            printf("\nAte mais! Obrigado por jogar QUACK QUIZ!\n");
            break;
        }
        
        if (opcao_menu == 1) {
            // Novo jogo - selecionar numero de jogadores
            num_jogadores = menu_num_jogadores();
            
            printf("\n================================\n");
            printf("   SELECIONANDO PERSONAGENS     \n");
            printf("================================\n");
            
            // Reinicia posicao dos jogadores usando a estrutura player[] de dados.h
            for (int i = 0; i < 4; i++) {
                player[i].posicao = 1;
            }
            
            // Inicializa fila de turnos
            inicializa_fila(&fila_turnos);
            
            // Selecionar personagem para cada jogador
            for (int i = 0; i < num_jogadores; i++) {
                int personagem = menu_personagem(i + 1);
                sprintf(player[i].nome, "Jogador %d (%s)", i + 1, nome_personagem(personagem));
                printf("\n%s selecionado!\n", player[i].nome);
                
                // Adiciona jogador na fila de turnos
                insere_fila(&fila_turnos, player[i].id_player);
            }
            
            // Reseta perguntas usadas
            resetar_perguntas();
            
            printf("\n================================\n");
            printf("        JOGO INICIADO!          \n");
            printf("================================\n");
            printf("Jogadores participando:\n");
            for (int i = 0; i < num_jogadores; i++) {
                printf("  - %s (Casa %d)\n", player[i].nome, player[i].posicao);
            }
            
            // Loop do jogo
            int em_jogo = 1;
            rodada = 1;
            
            while (em_jogo) {
                // Pega o proximo jogador da fila (sistema circular)
                tp_item id_jogador;
                remove_fila(&fila_turnos, &id_jogador);
                jogador_atual = id_jogador - 1; // Converte ID para indice (ID comeca em 1)
                
                // Reinsere na fila para manter o ciclo
                insere_fila(&fila_turnos, id_jogador);
                
                printf("\n========================================\n");
                printf("           RODADA %d                     \n", rodada);
                printf("========================================\n");
                printf("Vez de: %s\n", player[jogador_atual].nome);
                printf("Posicao atual: Casa %d\n", player[jogador_atual].posicao);
                printf("----------------------------------------\n");
                printf("Pressione ENTER para sortear uma pergunta...");
                getchar(); // limpa buffer
                getchar(); // espera enter
                
                // Faz a pergunta
                int resultado = pergunta_com_retorno();
                
                if (resultado > 0) {
                    player[jogador_atual].posicao += resultado;
                    printf("\n%s avancou para a casa %d!\n", 
                           player[jogador_atual].nome, 
                           player[jogador_atual].posicao);
                    
                    // Verifica vitoria (casa 20)
                    if (player[jogador_atual].posicao >= 20) {
                        printf("\n========================================\n");
                        printf("        VENCEDOR!!!                      \n");
                        printf("========================================\n");
                        printf("%s VENCEU O JOGO!\n", player[jogador_atual].nome);
                        printf("========================================\n");
                        em_jogo = 0;
                        continue;
                    }
                }
                
                // Mostra placar
                printf("\n--- PLACAR ---\n");
                for (int i = 0; i < num_jogadores; i++) {
                    printf("%s: Casa %d\n", player[i].nome, player[i].posicao);
                }
                
                // Menu de pause
                printf("\n[1] Continuar  [2] Menu Principal  [3] Sair\n");
                printf("Opcao: ");
                int pausa;
                scanf("%d", &pausa);
                
                if (pausa == 2) {
                    em_jogo = 0;
                    continue;
                } else if (pausa == 3) {
                    em_jogo = 0;
                    jogo_ativo = 0;
                    continue;
                }
                
                // Verifica se completou uma rodada
                if (jogador_atual == num_jogadores - 1) {
                    rodada++;
                }
            }
        }
    }
    
    return 0;
}
