#ifndef MENU_H
#define MENU_H

// menu.h
// ---------------------------------------------------------------------------
// Interface de menus e fluxo principal da partida.

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "fila.h"
#include "dados.h"
#include "tabuleiro.h"
#include "estatisticas.h"

// Limpa caracteres pendentes na entrada para evitar leituras indevidas.
void limpar_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Limpa o que sobrou na entrada.
    }
}

// Limpa o terminal para exibir apenas o menu atual.
void limpar_tela() {
    system("cls");
}

// Exibe o menu principal e retorna a opcao escolhida pelo usuario.
int menu_principal() {
    int opcao;
    int leitura;

    while (1) {
        limpar_tela();
        printf("\n============================================\n");
        printf("            QUACK BOARDGAME                \n");
        printf("============================================\n");
        printf("              __                            \n");
        printf("           __(o )>  QUACK!                  \n");
        printf("           \\ <_. )                         \n");
        printf("            `---'                           \n");
        printf("============================================\n");
        printf("1. Novo jogo\n");
        printf("2. Sair\n");
        printf("============================================\n");
        printf("Digite a opcao (1 ou 2): ");
        leitura = scanf("%d", &opcao);
        limpar_entrada();

        if (leitura == 1 && (opcao == 1 || opcao == 2)) {
            break;
        }

        printf("Opcao invalida! Digite 1 ou 2.\n");
        printf("Pressione ENTER para tentar novamente...");
        getchar();
    }

    return opcao;
}

// Exibe a mensagem de encerramento do jogo.
void exibir_mensagem_saida() {
    printf("\nAte mais! Obrigado por jogar QUACK QUIZ!\n");
}

// Define o numero de jogadores da partida atual.
int menu_num_jogadores() {
    int quantidade;
    int leitura;

    // Repete ate receber um numero valido.
    while (1) {
        limpar_tela();
        printf("\n================================\n");
        printf("      NUMERO DE JOGADORES       \n");
        printf("================================\n");
        printf("Escolha entre 2 e 4 jogadores: ");
        leitura = scanf("%d", &quantidade);
        limpar_entrada();

        if (leitura == 1 && quantidade >= 2 && quantidade <= 4) {
            break;
        }

        printf("Opcao invalida! Digite um numero entre 2 e 4.\n");
        printf("Pressione ENTER para tentar novamente...");
        getchar();
    }

    return quantidade;
}

// Verifica se o personagem ja foi escolhido por outro jogador.
int personagem_ja_escolhido(int personagem, int personagens_escolhidos[], int total_escolhidos) {
    int i;

    for (i = 0; i < total_escolhidos; i++) {
        if (personagens_escolhidos[i] == personagem) {
            return 1;
        }
    }

    return 0;
}

// Exibe menu de personagens e retorna o personagem escolhido.
int menu_personagem(int jogador_num, int personagens_escolhidos[], int total_escolhidos) {
    int personagem;
    int leitura;

    // Mantem o menu ate o jogador escolher uma opcao valida.
    while (1) {
        limpar_tela();
        printf("\n================================\n");
        printf("  JOGADOR %d - SELECIONANDO PERSONAGEM\n", jogador_num);
        printf("================================\n");
        printf(
            "1. Patolino%s\n",
            personagem_ja_escolhido(1, personagens_escolhidos, total_escolhidos) ? " (indisponivel)" : ""
        );
        printf(
            "2. Pato Donald%s\n",
            personagem_ja_escolhido(2, personagens_escolhidos, total_escolhidos) ? " (indisponivel)" : ""
        );
        printf(
            "3. Tio Patinhas%s\n",
            personagem_ja_escolhido(3, personagens_escolhidos, total_escolhidos) ? " (indisponivel)" : ""
        );
        printf(
            "4. Pato%s\n",
            personagem_ja_escolhido(4, personagens_escolhidos, total_escolhidos) ? " (indisponivel)" : ""
        );
        printf("--------------------------------\n");
        printf("Digite sua opcao: ");
        leitura = scanf("%d", &personagem);
        limpar_entrada();

        if (leitura == 1 && personagem >= 1 && personagem <= 4
            && !personagem_ja_escolhido(personagem, personagens_escolhidos, total_escolhidos)) {
            break;
        }

        if (leitura == 1 && personagem >= 1 && personagem <= 4) {
            printf("Personagem indisponivel! Escolha outro.\n");
        } else {
            printf("Resposta invalida! Digite um numero entre 1 e 4.\n");
        }
        printf("Pressione ENTER para tentar novamente...");
        getchar();
    }

    return personagem;
}

// Converte o id do personagem para o nome exibido no jogo.
void nome_personagem(int id, char nome_saida[]) {
    switch(id) {
        case 1: copiar_texto(nome_saida, 50, "Patolino"); break;
        case 2: copiar_texto(nome_saida, 50, "Pato Donald"); break;
        case 3: copiar_texto(nome_saida, 50, "Tio Patinhas"); break;
        case 4: copiar_texto(nome_saida, 50, "Pato"); break;
        default: copiar_texto(nome_saida, 50, "Desconhecido"); break;
    }
}

// Menu de pausa ao fim do turno.
int menu_pause() {
    int opcao;

    limpar_tela();
    printf("\n================================\n");
    printf("              PAUSE              \n");
    printf("================================\n");
    printf("1. Continuar\n");
    printf("2. Sair para o menu principal\n");
    printf("3. Sair do jogo\n");
    printf("--------------------------------\n");
    printf("Digite a opcao desejada: ");
    // Leitura defensiva para evitar lixo na entrada.
    if (scanf("%d", &opcao) != 1) {
        opcao = 1;
    }
    limpar_entrada();

    // Corrige valores fora da faixa.
    if (opcao < 1 || opcao > 3) {
        opcao = 1;
    }

    return opcao;
}

// Exibe o placar com a posicao atual de cada jogador.
void exibir_placar(tp_player jogadores[], int num_jogadores, tp_tabuleiro *tabuleiro) {
    int i;
    tp_casa *casa;

    printf("\n--- PLACAR ---\n");
    // Lista cada jogador e a unidade correspondente.
    for (i = 0; i < num_jogadores; i++) {
        casa = obter_casa(tabuleiro, jogadores[i].posicao);
        if (casa != NULL) {
            printf(
                "%s: Casa %d (Unidade %d)\n",
                jogadores[i].nome,
                jogadores[i].posicao,
                casa->unidade
            );
        } else {
            printf("%s: Casa %d\n", jogadores[i].nome, jogadores[i].posicao);
        }
    }
}

// Configura jogadores, nomes e fila de turnos no inicio da partida.
void configurar_jogadores(tp_player jogadores[], tp_fila *fila_turnos, int num_jogadores) {
    int i, personagem;
    int personagens_escolhidos[4];
    char nome_personagem_escolhido[50];

    printf("\n================================\n");
    printf("   SELECIONANDO PERSONAGENS     \n");
    printf("================================\n");

    // Inicializa dados basicos dos jogadores.
    inicializar_jogadores(jogadores, num_jogadores);

    // Inicializa o controle de personagens escolhidos.
    for (i = 0; i < 4; i++) {
        personagens_escolhidos[i] = 0;
    }

    // Cada jogador escolhe o personagem.
    for (i = 0; i < num_jogadores; i++) {
        personagem = menu_personagem(i + 1, personagens_escolhidos, i);
        nome_personagem(personagem, nome_personagem_escolhido);
        printf("\n%s selecionou %s!\n", jogadores[i].nome, nome_personagem_escolhido);
        personagens_escolhidos[i] = personagem;
    }

    // Mantem a ordem de jogada na fila.
    colocar_jogadores_na_fila(jogadores, num_jogadores, fila_turnos);
}

// Exibe resumo inicial da partida e configuracoes do tabuleiro.
void exibir_inicio_partida(tp_player jogadores[], int num_jogadores, tp_tabuleiro *tabuleiro) {
    int i;

    printf("\n================================\n");
    printf("        JOGO INICIADO!          \n");
    printf("================================\n");
    printf("Jogadores participando:\n");
    // Mostra a posicao inicial de cada jogador.
    for (i = 0; i < num_jogadores; i++) {
        printf("  - %s (Casa %d)\n", jogadores[i].nome, jogadores[i].posicao);
    }
    // Exibe configuracoes basicas do tabuleiro e perguntas.
    printf("\nTabuleiro: %d casas (%d por unidade)\n", tabuleiro->total, CASAS_POR_UNIDADE);
    printf("Perguntas carregadas: %d (6 por unidade)\n", TOTAL_PERGUNTAS);
}

// Exibe o tabuleiro em formato visual com jogadores e casas especiais.
void exibir_tabuleiro_visual(tp_tabuleiro *tabuleiro, tp_player jogadores[], int num_jogadores) {
    int pos;
    int row_start;
    int row_end;
    int i;
    tp_casa *casa;

    if (tabuleiro == NULL) {
        return;
    }

    printf("\nTABULEIRO\n");
    printf("Legenda: P=Pergunta A=Avanco R=Recuo .=Normal | Jogadores 1..4\n");

    for (row_start = 1; row_start <= tabuleiro->total; row_start += CASAS_POR_UNIDADE) {
        row_end = row_start + CASAS_POR_UNIDADE - 1;
        if (row_end > tabuleiro->total) {
            row_end = tabuleiro->total;
        }

        // Linha 1: numeros das casas com colunas.
        printf("\nUNIDADE %d\n", unidade_por_posicao(row_start));
        printf("+-----+");
        for (pos = row_start; pos <= row_end; pos++) {
            printf("-----+");
        }
        printf("\n");

        printf("|%-5s|", "CASA");
        for (pos = row_start; pos <= row_end; pos++) {
            printf(" %02d  |", pos);
        }
        printf("\n");

        // Linha 2: tipo de casa (P=pergunta, A=avanco, R=recuo, .=normal).
        printf("|%-5s|", "TIPO");
        for (pos = row_start; pos <= row_end; pos++) {
            char tipo = '.';

            casa = obter_casa(tabuleiro, pos);
            if (casa != NULL) {
                if (casa->tipo == CASA_PERGUNTA) {
                    tipo = 'P';
                } else if (casa->tipo == CASA_AVANCO) {
                    tipo = 'A';
                } else if (casa->tipo == CASA_RECUO) {
                    tipo = 'R';
                }
            }

            printf("  %c  |", tipo);
        }
        printf("|\n");

        // Linha 3: jogadores presentes em cada casa.
        printf("|%-5s|", "JOGS");
        for (pos = row_start; pos <= row_end; pos++) {
            char ocupacao[5] = {'.', '.', '.', '.', '\0'};
            int indice = 0;

            for (i = 0; i < num_jogadores; i++) {
                if (jogadores[i].posicao == pos && indice < 4) {
                    ocupacao[indice] = (char)('1' + i);
                    indice++;
                }
            }

            printf("%-5s|", ocupacao);
        }
        printf("|\n");

        printf("+-----+");
        for (pos = row_start; pos <= row_end; pos++) {
            printf("-----+");
        }
        printf("\n");
    }
}

// Exibe cabecalho visual da rodada e dados do jogador da vez.
void exibir_turno_jogador(tp_player jogadores[], int jogador_atual, int rodada) {
    // Limpa o terminal para destacar o turno atual.
    limpar_tela();
    // Cabecalho visual da rodada.
    printf("\n========================================\n");
    printf("           RODADA %d                     \n", rodada);
    printf("========================================\n");
    printf("Vez de: %s\n", jogadores[jogador_atual].nome);
    printf("Posicao atual: Casa %d\n", jogadores[jogador_atual].posicao);
    printf("----------------------------------------\n");
}

// Exibe mensagem de avanco apos resposta correta.
void exibir_avanco_jogador(tp_player jogadores[], int jogador_atual) {
    // Mensagem resumida de avancos.
    printf(
        "\n%s avancou para a casa %d!\n",
        jogadores[jogador_atual].nome,
        jogadores[jogador_atual].posicao
    );
}

// Exibe mensagem de recuo quando o jogador sofre penalidade.
void exibir_recuo_jogador(tp_player jogadores[], int jogador_atual) {
    // Mensagem resumida de recuos.
    printf(
        "\n%s recuou para a casa %d!\n",
        jogadores[jogador_atual].nome,
        jogadores[jogador_atual].posicao
    );
}

// Exibe menu visual da carta sorteada e suas alternativas.
void menu_carta_sorteada(tp_carta carta) {
    int i;

    limpar_tela();
    printf("\n========================================\n");
    printf(" CARTA SORTEADA! (Unidade %d - Dificuldade %d)\n", carta.unidade, carta.dificuldade);
    printf("========================================\n");
    printf("\n%s\n\n", carta.pergunta);

    // Exibe as alternativas numeradas.
    for (i = 0; i < NUM_ALTERNATIVAS; i++) {
        printf("%d) %s\n", i + 1, carta.alternativas[i]);
    }
}

// Coleta a resposta da carta exibida.
int menu_resposta_carta() {
    int resposta;

    printf("\nDigite o numero da sua resposta: ");
    // Caso a leitura falhe, retorna resposta invalida.
    if (scanf("%d", &resposta) != 1) {
        resposta = -1;
    }
    limpar_entrada();

    return resposta;
}

// Controla o fluxo visual e retorno da resposta de uma carta.
int fazer_pergunta(
    tp_pilha pilhas[NUM_UNIDADES][NUM_DIFICULDADES],
    tp_carta banco[],
    int total,
    int unidade,
    int *estado,
    int *acertou
) {
    tp_carta carta;
    int resposta;
    int dificuldade;

    // Inicializa o sinalizador de acerto.
    if (acertou != NULL) {
        *acertou = 0;
    }

    // Sorteia o nivel de dificuldade dentro da unidade atual.
    dificuldade = sortear_dificuldade(estado);
    printf("\nDificuldade sorteada: %d\n", dificuldade);

    if (!sortear_carta(pilhas, banco, total, unidade, dificuldade, estado, &carta)) {
        return 0;
    }

    // Exibe a carta e captura a resposta.
    menu_carta_sorteada(carta);
    resposta = menu_resposta_carta();

    printf("\nRESPOSTA...\n");

    // Avalia a resposta e calcula a movimentacao.
    if (resposta == carta.resposta) {
        if (acertou != NULL) {
            *acertou = 1;
        }
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
    printf("Penalidade: retorna %d casa(s).\n", PENALIDADE_ERRO);

    return -PENALIDADE_ERRO;
}

// Exibe tela de vitoria quando um jogador alcanca a casa final.
void exibir_vencedor(tp_player jogadores[], int jogador_atual) {
    printf("\n========================================\n");
    printf("        VENCEDOR!!!                      \n");
    printf("========================================\n");
    printf("%s VENCEU O JOGO!\n", jogadores[jogador_atual].nome);
    printf("========================================\n");
}

// Exibe a posicao atual do jogador e a unidade correspondente.
void exibir_posicao_atual(tp_tabuleiro *tabuleiro, tp_player jogadores[], int jogador_atual) {
    // Identifica a casa para mostrar unidade e numero.
    tp_casa *casa = obter_casa(tabuleiro, jogadores[jogador_atual].posicao);

    if (casa != NULL) {
        printf("Parou na casa %d (Unidade %d)\n", casa->numero, casa->unidade);
    } else {
        printf("Parou na casa %d\n", jogadores[jogador_atual].posicao);
    }
}

// Controla o fluxo completo de uma partida.
int executar_partida() {
    tp_player jogadores[4];
    tp_fila fila_turnos;
    tp_pilha pilhas_perguntas[NUM_UNIDADES][NUM_DIFICULDADES];
    tp_carta banco[TOTAL_PERGUNTAS];
    tp_tabuleiro tabuleiro;
    tp_no_est *estatisticas = NULL;
    tp_item id_jogador;
    int num_jogadores, jogador_atual, rodada, pausa;
    int em_jogo = 1;
    int retorno_menu = 1;
    int turnos = 0;
    int dado;
    int delta;
    int acertou;
    int estado = criar_semente_aleatoria();
    tp_casa *casa;

    // Entrada e inicializacao da partida.
    num_jogadores = menu_num_jogadores();

    configurar_jogadores(jogadores, &fila_turnos, num_jogadores);

    if (!criar_tabuleiro(&tabuleiro)) {
        printf("\nErro ao criar tabuleiro. Voltando ao menu.\n");
        return 1;
    }

    // Inicializa estatisticas e pilhas de perguntas.
    inicializar_estatisticas(&estatisticas, tabuleiro.total);
    montar_banco_cartas(banco);
    carregar_perguntas(pilhas_perguntas, banco, TOTAL_PERGUNTAS, &estado);
    exibir_inicio_partida(jogadores, num_jogadores, &tabuleiro);
    exibir_tabuleiro_visual(&tabuleiro, jogadores, num_jogadores);

    // Loop principal de rodadas.
    while (em_jogo) {
        // Seleciona o jogador da vez (FIFO).
        if (!remove_fila(&fila_turnos, &id_jogador)) {
            break;
        }

        jogador_atual = id_jogador - 1;
        insere_fila(&fila_turnos, id_jogador);

        // Calcula a rodada com base no total de turnos.
        turnos++;
        rodada = ((turnos - 1) / num_jogadores) + 1;

        exibir_turno_jogador(jogadores, jogador_atual, rodada);
        printf("Pressione ENTER para rolar o dado...");
        getchar();

        // Rolagem do dado e movimento inicial.
        dado = rolar_dado(&estado);
        printf("Dado sorteado: %d\n", dado);

        jogadores[jogador_atual].posicao = mover_posicao(
            &tabuleiro,
            jogadores[jogador_atual].posicao,
            dado
        );
        registrar_visita(&estatisticas, jogadores[jogador_atual].posicao);
        exibir_posicao_atual(&tabuleiro, jogadores, jogador_atual);

        // Verifica vitoria apos o movimento do dado.
        if (jogadores[jogador_atual].posicao >= tabuleiro.total) {
            exibir_vencedor(jogadores, jogador_atual);
            em_jogo = 0;
            break;
        }

        // Resolve a casa atual (pergunta, bonus ou penalidade).
        casa = obter_casa(&tabuleiro, jogadores[jogador_atual].posicao);

        // Resolve o efeito da casa atual.
        if (casa != NULL && casa->tipo == CASA_PERGUNTA) {
            acertou = 0;
            delta = fazer_pergunta(
                pilhas_perguntas,
                banco,
                TOTAL_PERGUNTAS,
                casa->unidade,
                &estado,
                &acertou
            );
            registrar_resultado(&estatisticas, casa->numero, acertou);
            if (delta != 0) {
                // Move novamente com base no resultado da pergunta.
                jogadores[jogador_atual].posicao = mover_posicao(
                    &tabuleiro,
                    jogadores[jogador_atual].posicao,
                    delta
                );
                registrar_visita(&estatisticas, jogadores[jogador_atual].posicao);
                if (delta > 0) {
                    exibir_avanco_jogador(jogadores, jogador_atual);
                } else {
                    exibir_recuo_jogador(jogadores, jogador_atual);
                }
            }
        } else if (casa != NULL && casa->tipo == CASA_AVANCO) {
            printf("Casa bonus! Avance %d casa(s).\n", casa->efeito);
            jogadores[jogador_atual].posicao = mover_posicao(
                &tabuleiro,
                jogadores[jogador_atual].posicao,
                casa->efeito
            );
            registrar_visita(&estatisticas, jogadores[jogador_atual].posicao);
            exibir_avanco_jogador(jogadores, jogador_atual);
        } else if (casa != NULL && casa->tipo == CASA_RECUO) {
            printf("Casa de penalidade! Retorne %d casa(s).\n", casa->efeito);
            jogadores[jogador_atual].posicao = mover_posicao(
                &tabuleiro,
                jogadores[jogador_atual].posicao,
                -casa->efeito
            );
            registrar_visita(&estatisticas, jogadores[jogador_atual].posicao);
            exibir_recuo_jogador(jogadores, jogador_atual);
        }

        // Mostra o tabuleiro apos aplicar os efeitos da casa.
        exibir_tabuleiro_visual(&tabuleiro, jogadores, num_jogadores);

        // Verifica vitoria apos efeitos das casas.
        if (jogadores[jogador_atual].posicao >= tabuleiro.total) {
            exibir_vencedor(jogadores, jogador_atual);
            em_jogo = 0;
            break;
        }

        // Encerramento do turno e decisao do jogador.
        exibir_placar(jogadores, num_jogadores, &tabuleiro);
        // Abre o menu de pausa apenas se o jogador solicitar (0).
        printf("Pressione ENTER para continuar ou digite 0 e ENTER para pausar: ");
        {
            char entrada[16];
            if (fgets(entrada, sizeof(entrada), stdin) != NULL && entrada[0] == '0') {
                pausa = menu_pause();
            } else {
                pausa = 1;
            }
        }

        if (pausa == 2) {
            em_jogo = 0;
            retorno_menu = 1;
        } else if (pausa == 3) {
            em_jogo = 0;
            retorno_menu = 0;
        }
    }

    // Salva estatisticas e libera recursos.
    if (salvar_relatorio(estatisticas, "relatorio.txt")) {
        printf("\nRelatorio salvo em relatorio.txt\n");
    } else {
        printf("\nNao foi possivel salvar o relatorio.\n");
    }

    liberar_estatisticas(estatisticas);
    destruir_tabuleiro(&tabuleiro);
    return retorno_menu;
}

#endif