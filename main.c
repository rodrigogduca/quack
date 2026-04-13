#include "pilha.h"
#include "fila.h"
#include "menu.h"
#include "dados.h"

int main() {
    tp_player jogadores[4];
    tp_pilha_cartas pilha_facil, pilha_medio, pilha_dificil;
    tp_fila fila_turnos;
    tp_item id_jogador;
    char nome_pers[50];
    int opcao_menu, num_jogadores, jogador_atual, rodada, resultado, pausa, personagem, i;
    int jogo_ativo = 1;
    int em_jogo = 0;
    
    inicializar_aleatorio();
    
    while (jogo_ativo) {
        opcao_menu = menu_principal();
        
        if (opcao_menu == 2) {
            printf("\nAte mais! Obrigado por jogar QUACK QUIZ!\n");
            break;
        }
        
        if (opcao_menu == 1) {
            num_jogadores = menu_num_jogadores();
            
            printf("\n================================\n");
            printf("   SELECIONANDO PERSONAGENS     \n");
            printf("================================\n");
            
            inicializar_jogadores(jogadores, 4);
            inicializa_fila(&fila_turnos);
            
            for (i = 0; i < num_jogadores; i++) {
                personagem = menu_personagem(i + 1);
                nome_personagem(personagem, nome_pers);
                sprintf(jogadores[i].nome, "Jogador %d (%s)", i + 1, nome_pers);
                printf("\n%s selecionado!\n", jogadores[i].nome);
                insere_fila(&fila_turnos, jogadores[i].id_player);
            }
            
            carregar_perguntas(&pilha_facil, &pilha_medio, &pilha_dificil);
            embaralhar_pilha(&pilha_facil);
            embaralhar_pilha(&pilha_medio);
            embaralhar_pilha(&pilha_dificil);
            
            printf("\n================================\n");
            printf("        JOGO INICIADO!          \n");
            printf("================================\n");
            printf("Jogadores participando:\n");
            for (i = 0; i < num_jogadores; i++) {
                printf("  - %s (Casa %d)\n", jogadores[i].nome, jogadores[i].posicao);
            }
            printf("\nCartas carregadas:\n");
            printf("  - Facil: %d cartas\n", tamanho_pilha_cartas(&pilha_facil));
            printf("  - Medio: %d cartas\n", tamanho_pilha_cartas(&pilha_medio));
            printf("  - Dificil: %d cartas\n", tamanho_pilha_cartas(&pilha_dificil));
            
            em_jogo = 1;
            rodada = 1;
            
            while (em_jogo) {
                remove_fila(&fila_turnos, &id_jogador);
                jogador_atual = id_jogador - 1;
                insere_fila(&fila_turnos, id_jogador);
                
                printf("\n========================================\n");
                printf("           RODADA %d                     \n", rodada);
                printf("========================================\n");
                printf("Vez de: %s\n", jogadores[jogador_atual].nome);
                printf("Posicao atual: Casa %d\n", jogadores[jogador_atual].posicao);
                printf("----------------------------------------\n");
                printf("Pressione ENTER para sortear uma pergunta...");
                getchar();
                getchar();
                
                resultado = fazer_pergunta(&pilha_facil, &pilha_medio, &pilha_dificil);
                
                if (resultado > 0) {
                    jogadores[jogador_atual].posicao += resultado;
                    printf("\n%s avancou para a casa %d!\n", 
                           jogadores[jogador_atual].nome, 
                           jogadores[jogador_atual].posicao);
                    
                    if (jogadores[jogador_atual].posicao >= 20) {
                        printf("\n========================================\n");
                        printf("        VENCEDOR!!!                      \n");
                        printf("========================================\n");
                        printf("%s VENCEU O JOGO!\n", jogadores[jogador_atual].nome);
                        printf("========================================\n");
                        em_jogo = 0;
                        continue;
                    }
                }
                
                printf("\n--- PLACAR ---\n");
                for (i = 0; i < num_jogadores; i++) {
                    printf("%s: Casa %d\n", jogadores[i].nome, jogadores[i].posicao);
                }
                
                printf("\n[1] Continuar  [2] Menu Principal  [3] Sair\n");
                printf("Opcao: ");
                scanf("%d", &pausa);
                
                if (pausa == 2) {
                    em_jogo = 0;
                } else if (pausa == 3) {
                    em_jogo = 0;
                    jogo_ativo = 0;
                }
                
                if (jogador_atual == num_jogadores - 1) {
                    rodada++;
                }
            }
        }
    }
    
    return 0;
}
