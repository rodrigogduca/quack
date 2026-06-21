#ifndef MENU_H
#define MENU_H

// menu.h
// Interface de menus e fluxo principal da partida.

#include <stdio.h>
#include <stdlib.h>

#include "fila.h"
#include "dados.h"
#include "tabuleiro.h"
#include "estatisticas.h"

// Cores ANSI para a interface de terminal
#define RESET   "\033[0m"  // Reseta todas as formatacoes (cor e estilo) para o padrao do terminal.
#define BOLD    "\033[1m"  // Aplica negrito/brilho ao texto.
#define RED     "\033[31m" // Altera a cor do texto para vermelho.
#define GREEN   "\033[32m" // Altera a cor do texto para verde.
#define YELLOW  "\033[33m" // Altera a cor do texto para amarelo.
#define BLUE    "\033[34m" // Altera a cor do texto para azul.
#define MAGENTA "\033[35m" // Altera a cor do texto para magenta/roxo.
#define CYAN    "\033[36m" // Altera a cor do texto para ciano/azul claro.

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

// Desenha uma linha horizontal usada nos blocos da interface.
void interface_linha(char caractere, int tamanho) {
    int i;

    for (i = 0; i < tamanho; i++) {
        printf("%c", caractere);
    }
    printf("\n");
}

// Exibe um cabecalho padronizado para as telas principais.
void interface_cabecalho(char titulo[]) {
    printf(CYAN BOLD "\n");
    interface_linha('=', 58);
    printf("| %-54s |\n", titulo);
    interface_linha('=', 58);
    printf(RESET);
}

// Exibe uma opcao de menu no mesmo formato em todas as telas.
void interface_opcao(int numero, char texto[]) {
    printf("  [%d] %s\n", numero, texto);
}

// Exibe uma mensagem de aviso/erro com destaque visual.
void interface_aviso(char mensagem[]) {
    printf(YELLOW BOLD "\n%s\n" RESET, mensagem);
}

// Exibe uma pausa padronizada antes de voltar para outro menu.
void interface_aguardar_enter(char mensagem[]) {
    printf("\n%s", mensagem);
    getchar();
}

// Lê e exibe o conteudo do historico de respostas (CSV).
void exibir_estatisticas_menu() {
    FILE *arquivo = fopen("historico_respostas.csv", "r");
    char linha[512];

    limpar_tela();
    interface_cabecalho("HISTORICO DE RESPOSTAS (CSV)");

    if (arquivo == NULL) {
        interface_aviso("Nenhum historico encontrado ainda. Jogue uma partida primeiro!");
    } else {
        // Imprime cada linha do arquivo formatado.
        while (fgets(linha, sizeof(linha), arquivo) != NULL) {
            printf("%s", linha);
        }
        fclose(arquivo);
    }
    interface_linha('=', 58);
    interface_aguardar_enter("Pressione ENTER para voltar ao menu...");
}

// Exibe o menu principal e retorna a opcao escolhida pelo usuario.
int menu_principal() {
    int opcao;
    int leitura;

    while (1) {
        limpar_tela();
        interface_cabecalho("QUACK BOARDGAME");
        printf(YELLOW "              __                            \n");
        printf("           __(o )>  " CYAN "QUACK!" YELLOW "                  \n");
        printf("           \\ <_. )                         \n");
            printf("            `---'                           \n" RESET);
        interface_linha('-', 58);
        interface_opcao(1, "Novo jogo");
        interface_opcao(2, "Ver historico de respostas");
        printf("  [0] Sair\n");
        interface_linha('-', 58);
        printf("Digite a opcao (1, 2 ou 0): ");
        leitura = scanf("%d", &opcao);
        limpar_entrada();

        if (leitura == 1) {
            if (opcao == 1 || opcao == 0) {
                break;
            } else if (opcao == 2) {
                exibir_estatisticas_menu();
                continue; // Volta pro topo do laco pra manter o menu aberto
            }
        }

        interface_aviso("Opcao invalida! Digite 1, 2 ou 0.");
        interface_aguardar_enter("Pressione ENTER para tentar novamente...");
    }

    return opcao;
}

// Exibe a mensagem de encerramento do jogo.
void exibir_mensagem_saida() {
    interface_cabecalho("FIM DE JOGO");
    printf("Ate mais! Obrigado por jogar QUACK QUIZ!\n");
}

// Define o numero de jogadores da partida atual.
int menu_num_jogadores() {
    int quantidade;
    int leitura;

    // Repete ate receber um numero valido.
    while (1) {
        limpar_tela();
        interface_cabecalho("NUMERO DE JOGADORES");
        printf("Escolha entre 2 e 4 jogadores: ");
        leitura = scanf("%d", &quantidade);
        limpar_entrada();

        if (leitura == 1 && quantidade >= 2 && quantidade <= 4) {
            break;
        }

        interface_aviso("Opcao invalida! Digite um numero entre 2 e 4.");
        interface_aguardar_enter("Pressione ENTER para tentar novamente...");
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
    int i;
    char nomes_personagens[4][50] = {
        "Patolino",
        "Pato Donald",
        "Tio Patinhas",
        "Pato"
    };

    // Mantem o menu ate o jogador escolher uma opcao valida.
    while (1) {
        limpar_tela();
        interface_cabecalho("SELECAO DE PERSONAGEM");
        printf("Jogador %d, escolha seu personagem:\n\n", jogador_num);
        // Exibe os personagens a partir do vetor de nomes padrao.
        for (i = 1; i <= 4; i++) {
            printf(
                "  [%d] %-14s%s\n",
                i,
                nomes_personagens[i - 1],
                personagem_ja_escolhido(i, personagens_escolhidos, total_escolhidos) ? " (indisponivel)" : ""
            );
        }
        interface_linha('-', 58);
        printf("Digite sua opcao: ");
        leitura = scanf("%d", &personagem);
        limpar_entrada();

        if (leitura == 1 && personagem >= 1 && personagem <= 4
            && !personagem_ja_escolhido(personagem, personagens_escolhidos, total_escolhidos)) {
            break;
        }

        if (leitura == 1 && personagem >= 1 && personagem <= 4) {
            interface_aviso("Personagem indisponivel! Escolha outro.");
        } else {
            interface_aviso("Resposta invalida! Digite um numero entre 1 e 4.");
        }
        interface_aguardar_enter("Pressione ENTER para tentar novamente...");
    }

    return personagem;
}

// Converte o id do personagem para o nome exibido no jogo.
void nome_personagem(int id, char nome_saida[]) {
    char nomes_personagens[4][50] = {
        "Patolino",
        "Pato Donald",
        "Tio Patinhas",
        "Pato"
    };

    if (id >= 1 && id <= 4) {
        copiar_texto(nome_saida, 50, nomes_personagens[id - 1]);
    } else {
        copiar_texto(nome_saida, 50, "Desconhecido");
    }
}

// Menu de pausa ao fim do turno.
int menu_pause() {
    int opcao;

    limpar_tela();
    interface_cabecalho("PAUSE");
    interface_opcao(1, "Continuar");
    interface_opcao(2, "Sair para o menu principal");
    interface_opcao(3, "Sair do jogo");
    interface_linha('-', 58);
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
void exibir_placar(tp_player jogadores[], int numero_jogadores, tp_tabuleiro *tabuleiro) {
    int i;
    tp_casa *casa;

    printf("\n--- PLACAR ---\n");
    // Lista cada jogador e a unidade correspondente.
    for (i = 0; i < numero_jogadores; i++) {
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
void configurar_jogadores(tp_player jogadores[], tp_fila *fila_turnos, int numero_jogadores) {
    int i, personagem;
    int personagens_escolhidos[4];
    char nome_personagem_escolhido[50];

    printf("\n================================\n");
    printf("   SELECIONANDO PERSONAGENS     \n");
    printf("================================\n");

    // Inicializa dados basicos dos jogadores.
    inicializar_jogadores(jogadores, numero_jogadores);

    // Inicializa o controle de personagens escolhidos.
    for (i = 0; i < 4; i++) {
        personagens_escolhidos[i] = 0;
    }

    // Cada jogador escolhe o personagem.
    for (i = 0; i < numero_jogadores; i++) {
        personagem = menu_personagem(i + 1, personagens_escolhidos, i);
        nome_personagem(personagem, nome_personagem_escolhido);
        // Define o nome do jogador como o nome do personagem escolhido (ex: "Patolino").
        copiar_texto(jogadores[i].nome, sizeof(jogadores[i].nome), nome_personagem_escolhido);
        printf("\n%s selecionou!\n", jogadores[i].nome);
        personagens_escolhidos[i] = personagem;
    }

    // Mantem a ordem de jogada na fila.
    colocar_jogadores_na_fila(jogadores, numero_jogadores, fila_turnos);
}

// Exibe resumo inicial da partida e configuracoes do tabuleiro.
void exibir_inicio_partida(tp_player jogadores[], int numero_jogadores, tp_tabuleiro *tabuleiro) {
    int i;

    printf("\n================================\n");
    printf("        JOGO INICIADO!          \n");
    printf("================================\n");
    printf("Jogadores participando:\n");
    // Mostra a posicao inicial de cada jogador.
    for (i = 0; i < numero_jogadores; i++) {
        printf("  - %s (Casa %d)\n", jogadores[i].nome, jogadores[i].posicao);
    }
    // Exibe configuracoes basicas do tabuleiro e perguntas.
    printf("\nTabuleiro: %d casas (%d por unidade)\n", tabuleiro->total, 10);
    printf("Perguntas carregadas: %d (unidades 1, 2 e 3)\n", 36);
}

// Exibe o tabuleiro em formato visual com jogadores e casas especiais.
void exibir_tabuleiro_visual(tp_tabuleiro *tabuleiro, tp_player jogadores[], int numero_jogadores) {
    int casa_num;
    int inicio_linha;
    int fim_linha;
    int indice;
    tp_casa *casa;

    if (tabuleiro == NULL) {
        return;
    }

    printf("\nTABULEIRO\n");
    printf("Legenda: P=Pergunta A=Avanco R=Recuo .=Normal | Jogadores 1-4\n");

    // Gera a exibicao a cada bloco de casas por unidade para caber adequadamente no terminal.
    for (inicio_linha = 1; inicio_linha <= tabuleiro->total; inicio_linha += 10) {
        fim_linha = inicio_linha + 10 - 1;
        if (fim_linha > tabuleiro->total) {
            fim_linha = tabuleiro->total;
        }

        // Linha 1: numeros das casas com colunas.
        printf("\nUNIDADE %d\n", unidade_por_posicao(inicio_linha));
        printf("+-----+");
        for (casa_num = inicio_linha; casa_num <= fim_linha; casa_num++) {
            printf("-----+");
        }
        printf("\n");

        printf("|%s|", "CASA");
        for (casa_num = inicio_linha; casa_num <= fim_linha; casa_num++) {
            printf(" %02d  |", casa_num);
        }
        printf("\n");

        // Linha 2: tipo de casa (P=pergunta, A=avanco, R=recuo, .=normal).
        printf("|%s|", "TIPO");
        for (casa_num = inicio_linha; casa_num <= fim_linha; casa_num++) {
            casa = obter_casa(tabuleiro, casa_num);
            if (casa != NULL) {
                if (casa->tipo == 1) {
                    printf("  " YELLOW "P" RESET "  |");
                } else if (casa->tipo == 2) {
                    printf("  " GREEN "A" RESET "  |");
                } else if (casa->tipo == 3) {
                    printf("  " RED "R" RESET "  |");
                } else {
                    printf("  .  |");
                }
            } else {
                printf("  .  |");
            }
        }
        printf("\n");

        // Linha 3: jogadores presentes em cada casa.
        printf("|%s|", "JOGS");
        for (casa_num = inicio_linha; casa_num <= fim_linha; casa_num++) {
            char ocupacao[5];
            int indice_ocupacao = 0;
            int indice_limpeza;

            // Inicializa o vetor de ocupacao com pontos e terminador de string.
            for (indice_limpeza = 0; indice_limpeza < 4; indice_limpeza++) {
                ocupacao[indice_limpeza] = '.';
            }
            ocupacao[4] = '\0';

            // Verifica quais jogadores estao presentes na casa_num.
            for (indice = 0; indice < numero_jogadores; indice++) {
                if (jogadores[indice].posicao == casa_num && indice_ocupacao < 4) {
                    ocupacao[indice_ocupacao] = (char)('1' + indice);
                    indice_ocupacao++;
                }
            }

            printf("%-5s|", ocupacao);
        }
        printf("|\n");

        printf("+-----+");
        for (casa_num = inicio_linha; casa_num <= fim_linha; casa_num++) {
            printf("-----+");
        }
        printf("\n");
    }
}

// Exibe cabecalho visual da rodada e dados do jogador da vez.
void exibir_turno_jogador(tp_player jogadores[], int indice_jogador, int numero_rodada) {
    // Limpa o terminal para destacar o turno atual.
    limpar_tela();
    // Cabecalho visual da rodada.
    printf(BLUE BOLD "\n========================================\n");
    printf("           RODADA %d                     \n", numero_rodada);
    printf("========================================\n" RESET);
    printf("Vez de: " CYAN BOLD "%s\n" RESET, jogadores[indice_jogador].nome);
    printf("Posicao atual: Casa %d\n", jogadores[indice_jogador].posicao);
    printf(BLUE "----------------------------------------\n" RESET);
}

// Exibe mensagem de avanco apos resposta correta.
void exibir_avanco_jogador(tp_player jogadores[], int indice_jogador) {
    // Mensagem resumida de avancos.
    printf(
        "\n%s avancou para a casa %d!\n",
        jogadores[indice_jogador].nome,
        jogadores[indice_jogador].posicao
    );
}

// Exibe mensagem de recuo quando o jogador sofre penalidade.
void exibir_recuo_jogador(tp_player jogadores[], int indice_jogador) {
    // Mensagem resumida de recuos.
    printf(
        "\n%s recuou para a casa %d!\n",
        jogadores[indice_jogador].nome,
        jogadores[indice_jogador].posicao
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
    for (i = 0; i < 5; i++) {
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
    tp_pilha pilhas[3][3],
    tp_carta banco[36],
    int total,
    int unidade,
    int *indicador_acerto,
    char nome_jogador[]
) {
    tp_carta carta;
    int resposta;
    int nivel_dificuldade;

    // Inicializa o sinalizador de acerto.
    if (indicador_acerto != NULL) {
        *indicador_acerto = 0;
    }

    // Sorteia o nivel de dificuldade dentro da unidade atual.
    nivel_dificuldade = sortear_dificuldade();
    printf("\nDificuldade sorteada: %d\n", nivel_dificuldade);

    if (!sortear_carta(pilhas, banco, total, unidade, nivel_dificuldade, &carta)) {
        return 0;
    }

    // Exibe a carta e captura a resposta.
    menu_carta_sorteada(carta);
    resposta = menu_resposta_carta();

    printf("\nRESPOSTA...\n");

    // Salva o historico da resposta (formato CSV do PDF)
    int resposta_correta = (resposta == carta.resposta);
    salvar_historico_resposta(
        "AED-2026.1-T", 
        nome_jogador, 
        carta.id_carta, 
        carta.unidade, 
        carta.tema, 
        carta.subtema, 
        nivel_dificuldade, 
        resposta, 
        carta.resposta, 
        resposta_correta
    );

    // Avalia a resposta e calcula a movimentacao.
    if (resposta_correta) {
        if (indicador_acerto != NULL) {
            *indicador_acerto = 1;
        }
        printf(GREEN BOLD "\n*** CORRETA! ***\n" RESET);
        printf("Voce avanca %d casa(s)!\n", carta.avanco);
        return carta.avanco;
    }

    printf(RED BOLD "\n*** ERRADA! ***\n" RESET);
    printf(
        "A resposta correta era: %d) %s\n",
        carta.resposta,
        carta.alternativas[carta.resposta - 1]
    );
    printf("Penalidade: retorna %d casa(s).\n", 1);

    return -1;
}

// Exibe tela de vitoria e ranking final com historico de casas.
void exibir_vencedor(tp_player jogadores[], int numero_jogadores, int indice_jogador) {
    int i, j;
    tp_player copia_jogadores[4];

    printf(YELLOW BOLD "\n========================================\n");
    printf("        VENCEDOR!!!                      \n");
    printf("========================================\n");
    printf("%s VENCEU O JOGO!\n", jogadores[indice_jogador].nome);
    printf("========================================\n");
    printf("========================================\n\n" RESET);

    // Cria uma copia para ordenar sem alterar o array original.
    for (i = 0; i < numero_jogadores; i++) {
        copia_jogadores[i] = jogadores[i];
    }

    // Ordena os jogadores pela posicao final (Bubble sort simples para ate 4 elementos).
    for (i = 0; i < numero_jogadores - 1; i++) {
        for (j = 0; j < numero_jogadores - i - 1; j++) {
            if (copia_jogadores[j].posicao < copia_jogadores[j + 1].posicao) {
                tp_player temp = copia_jogadores[j];
                copia_jogadores[j] = copia_jogadores[j + 1];
                copia_jogadores[j + 1] = temp;
            }
        }
    }

    printf("================================================================================\n");
    printf("                                RANKING FINAL                                   \n");
    printf("================================================================================\n");
    
    for (i = 0; i < numero_jogadores; i++) {
        printf("%do Lugar: %s (Casa %d)\n", i + 1, copia_jogadores[i].nome, copia_jogadores[i].posicao);
        printf("Caminho percorrido: ");
        for (j = 0; j < copia_jogadores[i].total_casas_visitadas; j++) {
            printf("%d", copia_jogadores[i].historico_casas[j]);
            if (j < copia_jogadores[i].total_casas_visitadas - 1) {
                printf(" -> ");
            }
        }
        printf("\n--------------------------------------------------------------------------------\n");
    }

    printf("\nPressione ENTER para voltar ao menu principal...");
    getchar();
}

// Exibe a posicao atual do jogador e a unidade correspondente.
void exibir_posicao_atual(tp_tabuleiro *tabuleiro, tp_player jogadores[], int indice_jogador) {
    // Identifica a casa para mostrar unidade e numero.
    tp_casa *casa = obter_casa(tabuleiro, jogadores[indice_jogador].posicao);

    if (casa != NULL) {
        printf("Parou na casa %d (Unidade %d)\n", casa->numero, casa->unidade);
    } else {
        printf("Parou na casa %d\n", jogadores[indice_jogador].posicao);
    }
}

// Controla o fluxo completo de uma partida.
int executar_partida() {
    tp_player jogadores[4];
    tp_fila fila_turnos;
    tp_pilha pilhas_perguntas[3][3];
    tp_carta banco[36];
    tp_tabuleiro tabuleiro;
    tp_item id_jogador;
    int numero_jogadores, indice_jogador_atual, numero_rodada, pausa;
    int partida_em_andamento = 1;
    int retorno_menu = 1;
    int turnos = 0;
    int valor_dado;
    int diferenca_casas;
    int indicador_acerto;
    tp_casa *casa;

    // Entrada e inicializacao da partida.
    numero_jogadores = menu_num_jogadores();

    // Prepara os patos! (Nomes e estruturas)
    configurar_jogadores(jogadores, &fila_turnos, numero_jogadores);

    if (!criar_tabuleiro(&tabuleiro)) {
        printf("\nErro ao criar tabuleiro. Voltando ao menu.\n");
        return 1;
    }

    // Inicializa pilhas de perguntas embaralhadas.
    montar_banco_cartas(banco);
    exportar_perguntas_csv(banco, 36); // Salva as perguntas em csv
    carregar_perguntas(pilhas_perguntas, banco, 36);
    exibir_inicio_partida(jogadores, numero_jogadores, &tabuleiro);
    exibir_tabuleiro_visual(&tabuleiro, jogadores, numero_jogadores);

    // Loop principal de rodadas.
    while (partida_em_andamento) {
        // Seleciona o jogador da vez (FIFO).
        if (!remove_fila(&fila_turnos, &id_jogador)) {
            break;
        }

        // Coloca o jogador de volta na fila para a proxima rodada
        indice_jogador_atual = id_jogador - 1;
        insere_fila(&fila_turnos, id_jogador);

        // Calcula a rodada com base no total de turnos.
        turnos++;
        numero_rodada = ((turnos - 1) / numero_jogadores) + 1;

        exibir_turno_jogador(jogadores, indice_jogador_atual, numero_rodada);
        printf("Pressione ENTER para rolar o dado...");
        getchar();

        // Rolagem do dado e movimento inicial.
        valor_dado = rolar_dado();
        printf("Dado sorteado: %d\n", valor_dado);

        jogadores[indice_jogador_atual].posicao = mover_posicao(
            &tabuleiro,
            jogadores[indice_jogador_atual].posicao,
            valor_dado
        );
        registrar_historico_posicao(&jogadores[indice_jogador_atual], jogadores[indice_jogador_atual].posicao);
        exibir_posicao_atual(&tabuleiro, jogadores, indice_jogador_atual);

        // Verifica vitoria apos o movimento do dado.
        if (jogadores[indice_jogador_atual].posicao >= tabuleiro.total) {
            exibir_vencedor(jogadores, numero_jogadores, indice_jogador_atual);
            partida_em_andamento = 0;
            break;
        }

        // Captura a casa atual para resolver efeitos especificos (pergunta, bonus ou penalidade).
        casa = obter_casa(&tabuleiro, jogadores[indice_jogador_atual].posicao);

        // Resolve o efeito da casa atual.
        if (casa != NULL && casa->tipo == 1) {
            indicador_acerto = 0;
            
            diferenca_casas = fazer_pergunta(
                pilhas_perguntas,
                banco,
                36,
                casa->unidade,
                &indicador_acerto,
                jogadores[indice_jogador_atual].nome
            );

            if (diferenca_casas != 0) {
                // Move novamente com base no resultado da pergunta.
                jogadores[indice_jogador_atual].posicao = mover_posicao(
                    &tabuleiro,
                    jogadores[indice_jogador_atual].posicao,
                    diferenca_casas
                );
                registrar_historico_posicao(&jogadores[indice_jogador_atual], jogadores[indice_jogador_atual].posicao);
                if (diferenca_casas > 0) {
                    exibir_avanco_jogador(jogadores, indice_jogador_atual);
                } else {
                    exibir_recuo_jogador(jogadores, indice_jogador_atual);
                }
            }
        } else if (casa != NULL && casa->tipo == 2) {
            printf("Casa bonus! Avance %d casa(s).\n", casa->efeito);
            jogadores[indice_jogador_atual].posicao = mover_posicao(
                &tabuleiro,
                jogadores[indice_jogador_atual].posicao,
                casa->efeito
            );
            registrar_historico_posicao(&jogadores[indice_jogador_atual], jogadores[indice_jogador_atual].posicao);
            exibir_avanco_jogador(jogadores, indice_jogador_atual);
        } else if (casa != NULL && casa->tipo == 3) {
            printf("Casa de penalidade! Retorne %d casa(s).\n", casa->efeito);
            jogadores[indice_jogador_atual].posicao = mover_posicao(
                &tabuleiro,
                jogadores[indice_jogador_atual].posicao,
                -casa->efeito
            );
            registrar_historico_posicao(&jogadores[indice_jogador_atual], jogadores[indice_jogador_atual].posicao);
            exibir_recuo_jogador(jogadores, indice_jogador_atual);
        }

        // Mostra o tabuleiro apos aplicar os efeitos da casa.
        exibir_tabuleiro_visual(&tabuleiro, jogadores, numero_jogadores);

        // Verifica vitoria apos efeitos das casas.
        if (jogadores[indice_jogador_atual].posicao >= tabuleiro.total) {
            exibir_vencedor(jogadores, numero_jogadores, indice_jogador_atual);
            partida_em_andamento = 0;
            break;
        }

        // Encerramento do turno e decisao do jogador.
        exibir_placar(jogadores, numero_jogadores, &tabuleiro);
        // Abre o menu de pausa apenas se o jogador solicitar (0).
        printf("Pressione ENTER para continuar ou digite 0 e para pausar o jogo: ");
        {
            char entrada[16];
            if (fgets(entrada, sizeof(entrada), stdin) != NULL && entrada[0] == '0') {
                pausa = menu_pause();
            } else {
                pausa = 1;
            }
        }

        if (pausa == 2) {
            partida_em_andamento = 0;
            retorno_menu = 1;
        } else if (pausa == 3) {
            partida_em_andamento = 0;
            retorno_menu = 0;
        }
    }


    destruir_tabuleiro(&tabuleiro);
    return retorno_menu;
}

#endif
