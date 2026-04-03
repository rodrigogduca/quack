#ifndef FUNCOES_H
#define FUNCOES_H

/*
 * funcoes.h
 * -----------------------------------------------------------------------------
 * Biblioteca com a logica da partida:
 * - preparacao de jogadores;
 * - controle de turnos com fila;
 * - movimentacao no tabuleiro com dado 1..6;
 * - carta de pergunta ao cair em casa multipla de 3;
 * - condicao de vitoria ao chegar na casa final.
 */

/*
 * Bibliotecas nativas
 * -----------------------------------------------------------------------------
 * stdio.h  : impressao de tabuleiro/status e mensagens.
 */
#include <stdio.h>

/*
 * Bibliotecas do projeto
 * -----------------------------------------------------------------------------
 * dados.h: estruturas tp_player/tp_carta e base de cartas/personagens.
 * estruturas.h: pilha e fila para historico e ordem de turnos.
 * menu.h : funcoes de interface (menus, pausa, leitura validada).
 */
#include "dados.h"
#include "estruturas.h"
#include "menu.h"

/*
 * Todas as funcoes deste arquivo sao static porque o jogo compila apenas
 * a unidade main.c, que inclui este cabecalho com implementacao.
 */

/* Constantes de configuracao da partida e do tabuleiro. */
#define MAX_JOGADORES 4
#define TABULEIRO_LINHAS 5
#define TABULEIRO_COLUNAS 5
#define TABULEIRO_CASAS (TABULEIRO_LINHAS * TABULEIRO_COLUNAS)
#define TOTAL_CARTAS_UNIDADE1 12
#define MIN_DADO 1
#define MAX_DADO 6

/* Estado interno do gerador pseudoaleatorio (xorshift32). */
static unsigned int estado_aleatorio = 2463534242u;

/* Define a semente do gerador interno, evitando semente zero. */
static void definir_semente(unsigned int semente) {
    if (semente == 0u) {
        semente = 2463534242u;
    }
    estado_aleatorio = semente;
}

/* Gera proximo valor pseudoaleatorio sem depender de stdlib/time. */
static unsigned int proximo_aleatorio(void) {
    estado_aleatorio ^= estado_aleatorio << 13;
    estado_aleatorio ^= estado_aleatorio >> 17;
    estado_aleatorio ^= estado_aleatorio << 5;
    return estado_aleatorio;
}

/*
 * exibir_tabuleiro
 * -----------------------------------------------------------------------------
 * Desenha o tabuleiro 5x5 no terminal.
 * Regras de exibicao por casa:
 * - sem jogador: numero da casa (01..25)
 * - 1 jogador  : Jn (ex.: J1)
 * - 2+ jogadores: Mn (ex.: M2)
 */
void exibir_tabuleiro(tp_player jogadores[MAX_JOGADORES], int num_jogadores) {
    printf("================================\n");
    printf("            TABULEIRO           \n");
    printf("================================\n");

    for (int linha = 0; linha < TABULEIRO_LINHAS; linha++) {
        for (int coluna = 0; coluna < TABULEIRO_COLUNAS; coluna++) {
            int casa = (linha * TABULEIRO_COLUNAS) + coluna + 1;
            int qtd_na_casa = 0;
            int ultimo_jogador = -1;
            char celula[8];

            for (int i = 0; i < num_jogadores; i++) {
                if (jogadores[i].posicao == casa) {
                    qtd_na_casa++;
                    ultimo_jogador = i;
                }
            }

            if (qtd_na_casa == 0) {
                snprintf(celula, sizeof(celula), "%02d", casa);
            } else if (qtd_na_casa == 1) {
                celula[0] = 'J';
                celula[1] = (char)('0' + (ultimo_jogador + 1));
                celula[2] = '\0';
            } else {
                celula[0] = 'M';
                celula[1] = (char)('0' + qtd_na_casa);
                celula[2] = '\0';
            }

            printf("| %-3s ", celula);
        }
        printf("|\n");
    }
}

/*
 * exibir_status
 * -----------------------------------------------------------------------------
 * Mostra, para cada jogador:
 * - personagem escolhido;
 * - casa atual;
 * - quantidade de cartas acertadas.
 */
static void exibir_status(tp_player jogadores[MAX_JOGADORES], int acertos[MAX_JOGADORES], int num_jogadores) {
    printf("\n================================\n");
    printf("            STATUS              \n");
    printf("================================\n");

    for (int i = 0; i < num_jogadores; i++) {
        printf("J%d - %-12s | Casa: %2d | Acertos: %d\n",
               i + 1,
               jogadores[i].personagem,
               jogadores[i].posicao,
               acertos[i]);
    }
}

/*
 * dados_aleatorios
 * -----------------------------------------------------------------------------
 * Retorna inteiro pseudoaleatorio no intervalo [min, max].
 */
static int dados_aleatorios(int min, int max) {
    unsigned int faixa = (unsigned int)(max - min + 1);
    return (int)(proximo_aleatorio() % faixa) + min;
}

/*
 * mostrar_carta
 * -----------------------------------------------------------------------------
 * Exibe pergunta, alternativas e metadados da carta atual.
 */
static void mostrar_carta(const tp_carta *carta) {
    printf("\nPergunta #%d (dificuldade %d)\n", carta->id_carta, carta->dificuldade);
    printf("%s", carta->pergunta);
    printf("%s", carta->alternativas);
}

/*
 * executar_partida
 * -----------------------------------------------------------------------------
 * Executa uma partida completa, do setup de jogadores ate a vitoria.
 * Retorno:
 * 0 -> encerrar partida e voltar ao menu principal.
 * 1 -> encerrar totalmente o programa.
 */
static int executar_partida(void) {
    /*
     * Estruturas principais da partida:
     * - personagens_usados: evita selecao repetida.
     * - acertos           : pontuacao por acerto de carta.
     * - historico         : pilha de posicoes por jogador.
     * - fila_turnos       : garante ordem de jogada (FIFO).
     */
    int num_jogadores;
    int personagens_usados[MAX_JOGADORES] = {0, 0, 0, 0};
    int acertos[MAX_JOGADORES] = {0, 0, 0, 0};
    tp_player jogadores[MAX_JOGADORES];
    tp_pilha historico[MAX_JOGADORES];
    tp_fila fila_turnos;
    int indice_carta = 0;

    /*
     * unidade2 e unidade3 ainda nao estao populadas.
     * O cast para void evita warning de "variavel nao usada".
     */
    (void)unidade2;
    (void)unidade3;

    /* Etapa 1: configurar quantidade de jogadores. */
    limpar_terminal();
    num_jogadores = menu_num_jogadores();

    /*
     * Etapa 2: selecao de personagem para cada jogador.
     * O jogador pode voltar para o anterior com opcao 0.
     */
    for (int i = 0; i < num_jogadores; i++) {
        int personagem;

        while (1) {
            personagem = menu_personagem(personagens_usados, i);

            if (personagem == 0) {
                if (i == 0) {
                    printf("Nao existe jogador anterior para voltar.\n");
                    continue;
                }

                i--;
                personagens_usados[jogadores[i].id_player - 1] = 0;
                printf("Selecao do Jogador %d removida.\n", i + 1);
                continue;
            }

            jogadores[i] = player[personagem - 1];
            jogadores[i].id_player = personagem;
            jogadores[i].posicao = 1;
            jogadores[i].moedas = 0;
            jogadores[i].estrelas = 0;
            personagens_usados[personagem - 1] = 1;
            inicializa_pilha(&historico[i]);
            push(&historico[i], 1);
            break;
        }
    }

    /* Etapa 3: inicializa a fila de turnos com os jogadores ativos. */
    inicializa_fila(&fila_turnos);
    for (int i = 0; i < num_jogadores; i++) {
        insere_fila(&fila_turnos, i);
    }

    /*
     * Etapa 4: loop principal da partida.
     * Cada iteracao equivale a um turno de um jogador.
     */
    while (1) {
        int id_jogador;
        int acao_turno;
        int dado;
        int posicao_antiga;
        int nova_posicao;

        limpar_terminal();
        exibir_tabuleiro(jogadores, num_jogadores);
        exibir_status(jogadores, acertos, num_jogadores);

        /* Remove o proximo jogador da fila para executar seu turno. */
        if (!remove_fila(&fila_turnos, &id_jogador)) {
            printf("Erro ao obter o proximo jogador.\n");
            return 0;
        }

        printf("\n================================\n");
        printf("Turno do Jogador %d (%s)\n", id_jogador + 1, jogadores[id_jogador].personagem);
        printf("================================\n");

        /*
         * Antes de rolar o dado, o jogador escolhe:
         * 1) Rolar dado
         * 2) Abrir pause (continuar, voltar ao menu ou encerrar)
         */
        while (1) {
            acao_turno = ler_inteiro_intervalo(
                "\n1. Rolar dado\n2. Abrir pause\nEscolha: ",
                1,
                2);

            if (acao_turno == 1) {
                break;
            }

            {
                int pause = menu_pause();

                if (pause == 1) {
                    continue;
                }

                if (pause == 2) {
                    return 0;
                }

                return 1;
            }
        }

        /* Movimento principal do turno via dado aleatorio 1..6. */
        dado = dados_aleatorios(MIN_DADO, MAX_DADO);
        posicao_antiga = jogadores[id_jogador].posicao;
        nova_posicao = posicao_antiga + dado;

        if (nova_posicao > TABULEIRO_CASAS) {
            nova_posicao = TABULEIRO_CASAS;
        }

        jogadores[id_jogador].posicao = nova_posicao;
        push(&historico[id_jogador], nova_posicao);

        printf("\nDado: %d\n", dado);
        printf("Voce saiu da casa %d e foi para a casa %d.\n", posicao_antiga, nova_posicao);

        /* Vitoria imediata apos movimento do dado. */
        if (jogadores[id_jogador].posicao >= TABULEIRO_CASAS) {
            printf("\n================================\n");
            printf("VENCEDOR: Jogador %d (%s)!\n", id_jogador + 1, jogadores[id_jogador].personagem);
            printf("================================\n");
            aguardar_enter();
            return 0;
        }

        /*
         * Regra da carta:
         * somente quando a posicao final do turno for multipla de 3.
         */
        if (jogadores[id_jogador].posicao % 3 == 0) {
            tp_carta carta = unidade1[indice_carta];
            int turno_concluido = 0;
            int resposta = 0;

            /* Avanca o indice circular de cartas (0..11). */
            indice_carta = (indice_carta + 1) % TOTAL_CARTAS_UNIDADE1;
            printf("\nVoce caiu em uma casa multipla de 3. Carta liberada!\n");

            /* Le resposta da carta, com opcao de abrir pause. */
            while (!turno_concluido) {
                int pause;
                mostrar_carta(&carta);
                resposta = ler_inteiro_intervalo("Resposta (1-5) ou 0 para pausar: ", 0, 5);

                if (resposta == 0) {
                    pause = menu_pause();

                    if (pause == 1) {
                        continue;
                    }

                    if (pause == 2) {
                        return 0;
                    }

                    return 1;
                }

                turno_concluido = 1;
            }

            /* Se acertar a carta, recebe bonus de avancar casas. */
            if (resposta == carta.resposta) {
                int bonus = jogadores[id_jogador].posicao + carta.avanco;

                acertos[id_jogador]++;

                if (bonus > TABULEIRO_CASAS) {
                    bonus = TABULEIRO_CASAS;
                }

                jogadores[id_jogador].posicao = bonus;
                push(&historico[id_jogador], bonus);

                printf("\nAcertou! Voce avancou %d casa(s) de bonus.\n", carta.avanco);
            } else {
                printf("\nResposta incorreta. Sem bonus de carta.\n");
            }

            /* Vitoria apos bonus da carta. */
            if (jogadores[id_jogador].posicao >= TABULEIRO_CASAS) {
                printf("\n================================\n");
                printf("VENCEDOR: Jogador %d (%s)!\n", id_jogador + 1, jogadores[id_jogador].personagem);
                printf("================================\n");
                aguardar_enter();
                return 0;
            }
        } else {
            printf("\nSem carta neste turno (a carta aparece apenas a cada 3 casas).\n");
        }

        /* Reinsere o jogador no fim da fila para manter rotacao de turnos. */
        if (!insere_fila(&fila_turnos, id_jogador)) {
            printf("Erro ao reinserir jogador na fila de turnos.\n");
            return 0;
        }

        /* Pausa de leitura antes de limpar e iniciar o proximo turno. */
        aguardar_enter();
    }
}

#endif