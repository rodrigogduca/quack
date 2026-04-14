#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#include "fila.h"
#include "dados.h"

/* Limpa caracteres pendentes na entrada para evitar leituras indevidas. */
void limpar_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* limpa o que sobrou na entrada */
    }
}

/* Exibe o menu principal e retorna a opcao escolhida pelo usuario. */
int menu_principal() {
    int opcao;

    printf("\n================================\n");
    printf("         QUACK QUIZ             \n");
    printf("================================\n");
    printf("1. Novo jogo\n");
    printf("2. Sair\n");
    printf("================================\n");
    printf("Digite alguma opcao: ");
    if (scanf("%d", &opcao) != 1) {
        opcao = 2;
    }
    limpar_entrada();

    return opcao;
}

/* Exibe a mensagem de encerramento do jogo. */
void exibir_mensagem_saida() {
    printf("\nAte mais! Obrigado por jogar QUACK QUIZ!\n");
}

/* Define o numero de jogadores da partida atual. */
int menu_num_jogadores() {
    int quantidade;
    int leitura;

    printf("\n================================\n");
    printf("      NUMERO DE JOGADORES       \n");
    printf("================================\n");
    while (1) {
        printf("Escolha entre 2 e 4 jogadores: ");
        leitura = scanf("%d", &quantidade);
        limpar_entrada();

        if (leitura == 1 && quantidade >= 2 && quantidade <= 4) {
            break;
        }

        printf("Opcao invalida! Digite um numero entre 2 e 4.\n");
    }

    return quantidade;
}

/* Exibe menu de personagens e retorna o personagem escolhido. */
int menu_personagem(int jogador_num) {
    int personagem;
    int leitura;

    printf("\n================================\n");
    printf("  JOGADOR %d - SELECIONE SEU PERSONAGEM!\n", jogador_num);
    printf("================================\n");
    printf("1. Patolino\n");
    printf("2. Pato Donald\n");
    printf("3. Tio Patinhas\n");
    printf("4. Pato\n");
    printf("--------------------------------\n");
    while (1) {
        printf("Digite sua opcao: ");
        leitura = scanf("%d", &personagem);
        limpar_entrada();

        if (leitura == 1 && personagem >= 1 && personagem <= 4) {
            break;
        }

        printf("Resposta invalida! Digite um numero entre 1 e 4.\n");
    }

    return personagem;
}

/* Converte o id do personagem para o nome exibido no jogo. */
void nome_personagem(int id, char nome_saida[]) {
    switch(id) {
        case 1: copiar_texto(nome_saida, 50, "Patolino"); break;
        case 2: copiar_texto(nome_saida, 50, "Pato Donald"); break;
        case 3: copiar_texto(nome_saida, 50, "Tio Patinhas"); break;
        case 4: copiar_texto(nome_saida, 50, "Pato"); break;
        default: copiar_texto(nome_saida, 50, "Desconhecido"); break;
    }
}

/* Menu de pausa ao fim do turno. */
int menu_pause() {
    int opcao;

    printf("\n================================\n");
    printf("              PAUSE              \n");
    printf("================================\n");
    printf("1. Continuar\n");
    printf("2. Sair para o menu principal\n");
    printf("3. Sair do jogo\n");
    printf("--------------------------------\n");
    printf("Digite a opcao desejada: ");
    if (scanf("%d", &opcao) != 1) {
        opcao = 1;
    }
    limpar_entrada();

    if (opcao < 1 || opcao > 3) {
        opcao = 1;
    }

    return opcao;
}

/* Exibe o placar com a posicao atual de cada jogador. */
void exibir_placar(tp_player jogadores[], int num_jogadores) {
    int i;

    printf("\n--- PLACAR ---\n");
    for (i = 0; i < num_jogadores; i++) {
        printf("%s: Casa %d\n", jogadores[i].nome, jogadores[i].posicao);
    }
}

/* Configura jogadores, nomes e fila de turnos no inicio da partida. */
void configurar_jogadores(tp_player jogadores[], tp_fila *fila_turnos, int num_jogadores) {
    int i, personagem;
    char nome_personagem_escolhido[50];

    printf("\n================================\n");
    printf("   SELECIONANDO PERSONAGENS     \n");
    printf("================================\n");

    inicializar_jogadores(jogadores, 4);

    for (i = 0; i < num_jogadores; i++) {
        personagem = menu_personagem(i + 1);
        nome_personagem(personagem, nome_personagem_escolhido);
        printf("\n%s selecionou %s!\n", jogadores[i].nome, nome_personagem_escolhido);
    }

    colocar_jogadores_na_fila(jogadores, num_jogadores, fila_turnos);
}

/* Exibe resumo inicial da partida e quantidade de cartas carregadas. */
void exibir_inicio_partida(tp_player jogadores[], int num_jogadores, tp_pilha *pilha_cartas) {
    int i;

    printf("\n================================\n");
    printf("        JOGO INICIADO!          \n");
    printf("================================\n");
    printf("Jogadores participando:\n");
    for (i = 0; i < num_jogadores; i++) {
        printf("  - %s (Casa %d)\n", jogadores[i].nome, jogadores[i].posicao);
    }
    printf("\nCartas carregadas: %d cartas\n", tamanho_pilha_cartas(pilha_cartas));
}

/* Exibe cabecalho visual da rodada e dados do jogador da vez. */
void exibir_turno_jogador(tp_player jogadores[], int jogador_atual, int rodada) {
    printf("\n========================================\n");
    printf("           RODADA %d                     \n", rodada);
    printf("========================================\n");
    printf("Vez de: %s\n", jogadores[jogador_atual].nome);
    printf("Posicao atual: Casa %d\n", jogadores[jogador_atual].posicao);
    printf("----------------------------------------\n");
}

/* Exibe mensagem de avanco apos resposta correta. */
void exibir_avanco_jogador(tp_player jogadores[], int jogador_atual) {
    printf(
        "\n%s avancou para a casa %d!\n",
        jogadores[jogador_atual].nome,
        jogadores[jogador_atual].posicao
    );
}

/* Exibe menu visual da carta sorteada e suas alternativas. */
void menu_carta_sorteada(tp_carta carta) {
    int i;

    printf("\n========================================\n");
    printf("   CARTA SORTEADA! (Dificuldade %d)     \n", carta.dificuldade);
    printf("========================================\n");
    printf("\n%s\n\n", carta.pergunta);

    for (i = 0; i < NUM_ALTERNATIVAS; i++) {
        printf("%d) %s\n", i + 1, carta.alternativas[i]);
    }
}

/* Coleta a resposta da carta exibida. */
int menu_resposta_carta() {
    int resposta;

    printf("\nDigite o numero da sua resposta: ");
    if (scanf("%d", &resposta) != 1) {
        resposta = -1;
    }
    limpar_entrada();

    return resposta;
}

/* Controla o fluxo visual e retorno da resposta de uma carta. */
int fazer_pergunta(tp_pilha *pilha_cartas) {
    tp_carta carta;
    int resposta;

    if (!sortear_carta(pilha_cartas, &carta)) {
        return 0;
    }

    menu_carta_sorteada(carta);
    resposta = menu_resposta_carta();

    printf("\nRESPOSTA...\n");

    if (resposta == carta.resposta) {
        printf("\n*** CORRETA! ***\n");
        printf("Voce avanca %d casa(s)!\n", carta.avanco);
        return carta.avanco;
    }

    printf("\n*** ERRADA! ***\n");
    printf(
        "A resposta correta era: %d) %s\n",
        carta.resposta,
        carta.alternativas[carta.resposta - 1]
    );

    return 0;
}

/* Exibe tela de vitoria quando um jogador alcanca a casa final. */
void exibir_vencedor(tp_player jogadores[], int jogador_atual) {
    printf("\n========================================\n");
    printf("        VENCEDOR!!!                      \n");
    printf("========================================\n");
    printf("%s VENCEU O JOGO!\n", jogadores[jogador_atual].nome);
    printf("========================================\n");
}

/* Controla o fluxo completo de uma partida. */
int executar_partida() {
    tp_player jogadores[4];
    tp_fila fila_turnos;
    tp_pilha pilha_cartas;
    tp_item id_jogador;
    int num_jogadores, jogador_atual, rodada, resultado, pausa;
    int em_jogo = 1;

    num_jogadores = menu_num_jogadores();

    configurar_jogadores(jogadores, &fila_turnos, num_jogadores);
    carregar_perguntas(&pilha_cartas);
    exibir_inicio_partida(jogadores, num_jogadores, &pilha_cartas);

    rodada = 1;

    while (em_jogo) {
        if (!remove_fila(&fila_turnos, &id_jogador)) {
            break;
        }

        jogador_atual = id_jogador - 1;
        insere_fila(&fila_turnos, id_jogador);

        exibir_turno_jogador(jogadores, jogador_atual, rodada);
        printf("Pressione ENTER para sortear uma pergunta...");
        getchar();

        resultado = fazer_pergunta(&pilha_cartas);

        if (resultado > 0) {
            jogadores[jogador_atual].posicao += resultado;
            exibir_avanco_jogador(jogadores, jogador_atual);

            if (jogadores[jogador_atual].posicao >= 20) {
                exibir_vencedor(jogadores, jogador_atual);
                em_jogo = 0;
                continue;
            }
        }

        exibir_placar(jogadores, num_jogadores);
        pausa = menu_pause();

        if (pausa == 2) {
            em_jogo = 0;
        } else if (pausa == 3) {
            return 0;
        }

        if (jogador_atual == num_jogadores - 1) {
            rodada++;
        }
    }

    return 1;
}

#endif