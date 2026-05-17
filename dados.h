#ifndef DADOS_H
#define DADOS_H

// dados.h
// Modelos do jogo, utilitarios e banco de perguntas.

#include <stdio.h>
#include <stdlib.h>

#include "pilha.h"
#include "fila.h"

typedef struct {
    char pergunta[300]; // Enunciado da pergunta.
    char alternativas[5][150]; // Alternativas numeradas.
    int resposta; // Alternativa correta (1..5).
    int avanco; // Casas a avancar se acertar.
    int dificuldade; // Nivel 1..3.
    int unidade; // Unidade associada (1..3).
    int id_carta; // Identificador interno da carta.
} tp_carta;

typedef struct {
    int posicao; // Casa atual no tabuleiro.
    char nome[50]; // Nome exibido no jogo.
    int id_player; // Identificador de turno (1..4).
} tp_player;

// Rola dado virtual (de 1 a 6) usando o gerador padrao.
int rolar_dado() {
    return (rand() % 6) + 1;
}

// Sorteia dificuldade (de 1 a 3) usando o gerador padrao.
int sortear_dificuldade() {
    return (rand() % 3) + 1;
}

// Copia texto com limite de tamanho (inclui terminador).
void copiar_texto(char destino[], int tamanho, char origem[]) {
    int i = 0;

    // Garante espaco minimo para o terminador.
    if (tamanho <= 0) {
        return;
    }

    // Copia ate o fim da origem ou o limite do destino.
    while (i < tamanho - 1 && origem[i] != '\0') {
        destino[i] = origem[i];
        i++;
    }

    destino[i] = '\0';
}

// Anexa (concatena) um texto ao final de outro, respeitando o limite do buffer.
void anexar_texto(char destino[], int tamanho, char origem[]) {
    int i = 0;
    int j = 0;

    // Encontra o fim atual do destino.
    while (i < tamanho && destino[i] != '\0') {
        i++;
    }

    // Se nao houver espaco para adicionar nada (inclusive terminador), sai.
    if (i >= tamanho - 1) {
        if (tamanho > 0) destino[tamanho - 1] = '\0';
        return;
    }

    // Se origem vazia, nao acrescenta espaco.
    if (origem[0] == '\0') {
        return;
    }

    // Adiciona um espaco separador caso haja texto previo e espaco disponivel.
    if (i > 0 && i < tamanho - 1) {
        destino[i] = ' ';
        i++;
    }

    // Copia a origem respeitando o limite.
    while (i < tamanho - 1 && origem[j] != '\0') {
        destino[i] = origem[j];
        i++;
        j++;
    }

    destino[i] = '\0';
}

// Converte inteiro positivo para string.
void inteiro_para_texto(int valor, char texto[]) {
    char invertido[12];
    int i = 0;
    int j;

    // Caso especial para zero.
    if (valor == 0) {
        texto[0] = '0';
        texto[1] = '\0';
        return;
    }

    // Gera os digitos em ordem inversa.
    while (valor > 0 && i < 11) {
        invertido[i] = (char)('0' + (valor % 10));
        valor /= 10;
        i++;
    }

    // Inverte para formar a string final.
    for (j = 0; j < i; j++) {
        texto[j] = invertido[i - 1 - j];
    }

    texto[i] = '\0';
}

// Monta nome padrao "Jogador N".
void montar_nome_jogador(char nome_saida[], int tamanho, int indice) {
    char prefixo[] = "Jogador ";
    char numero[12];
    int i = 0;
    int j = 0;

    // Garante tamanho valido do buffer.
    if (tamanho <= 0) {
        return;
    }

    // Converte o indice para string.
    inteiro_para_texto(indice, numero);

    while (i < tamanho - 1 && prefixo[i] != '\0') {
        nome_saida[i] = prefixo[i];
        i++;
    }

    while (i < tamanho - 1 && numero[j] != '\0') {
        nome_saida[i] = numero[j];
        i++;
        j++;
    }

    nome_saida[i] = '\0';
}

// Cria uma carta e copia seus textos.
tp_carta criar_carta(
    char perg[],
    char alt1[],
    char alt2[],
    char alt3[],
    char alt4[],
    char alt5[],
    int resp,
    int avanc,
    int dif,
    int unidade,
    int id
) {
    tp_carta c;

    // Copia enunciado e alternativas para o struct.
    copiar_texto(c.pergunta, sizeof(c.pergunta), perg);
    copiar_texto(c.alternativas[0], sizeof(c.alternativas[0]), alt1);
    copiar_texto(c.alternativas[1], sizeof(c.alternativas[1]), alt2);
    copiar_texto(c.alternativas[2], sizeof(c.alternativas[2]), alt3);
    copiar_texto(c.alternativas[3], sizeof(c.alternativas[3]), alt4);
    copiar_texto(c.alternativas[4], sizeof(c.alternativas[4]), alt5);

    // Preenche metadados da carta.
    c.resposta = resp;
    c.avanco = avanc;
    c.dificuldade = dif;
    c.unidade = unidade;
    c.id_carta = id;

    return c;
}

// Preenche o banco fixo de perguntas.
void montar_banco_cartas(tp_carta cartas[]) {
    cartas[0] = criar_carta(
        "Qual operador realiza atribuicao em C?",
        "=",
        "==",
        "!=",
        "<=",
        "++",
        1, 1, 1, 1, 1
    );

    cartas[1] = criar_carta(
        "Qual simbolo inicia um comentario de uma linha em C?",
        "/*",
        "//",
        "#",
        "<!--",
        "--",
        2, 1, 1, 1, 2
    );

    cartas[2] = criar_carta(
        "Qual simbolo termina uma instrucao em C?",
        ";",
        ",",
        ":",
        ".",
        "?",
        1, 1, 1, 1, 3
    );

    cartas[3] = criar_carta(
        "Qual tipo armazena um caractere unico em C?",
        "char",
        "int",
        "float",
        "double",
        "string",
        1, 1, 1, 1, 4
    );

    cartas[4] = criar_carta(
        "Qual e o primeiro indice valido de um vetor int v[10]?",
        "1",
        "0",
        "-1",
        "10",
        "Depende do compilador",
        2, 2, 2, 1, 5
    );

    cartas[5] = criar_carta(
        "Qual o resultado da expressao (7 % 3)?",
        "0",
        "1",
        "2",
        "3",
        "7",
        2, 2, 2, 1, 6
    );

    cartas[6] = criar_carta(
        "Qual operador logico representa 'E' em C?",
        "&&",
        "||",
        "!",
        "&",
        "|",
        1, 2, 2, 1, 7
    );

    cartas[7] = criar_carta(
        "Qual funcao le uma linha inteira com espacos do teclado?",
        "gets",
        "fgets",
        "scanf",
        "printf",
        "puts",
        2, 2, 2, 1, 8
    );

    cartas[8] = criar_carta(
        "O que acontece ao acessar v[10] em int v[10]?",
        "Acessa o ultimo elemento",
        "Acesso fora do limite (erro)",
        "O compilador corrige para v[9]",
        "Retorna sempre 0",
        "O vetor aumenta de tamanho",
        2, 3, 3, 1, 9
    );

    cartas[9] = criar_carta(
        "O que pode ocorrer ao usar printf com variavel nao inicializada?",
        "Erro de compilacao",
        "Valor lixo ou comportamento indefinido",
        "Sempre imprime zero",
        "Sempre imprime -1",
        "O programa termina automaticamente",
        2, 3, 3, 1, 10
    );

    cartas[10] = criar_carta(
        "Qual o resultado de 5 / 2 usando inteiros?",
        "2",
        "2.5",
        "3",
        "1",
        "0",
        1, 3, 3, 1, 11
    );

    cartas[11] = criar_carta(
        "Qual o ultimo indice valido de um vetor int v[10]?",
        "9",
        "10",
        "-1",
        "1",
        "Depende do compilador",
        1, 3, 3, 1, 12
    );

    cartas[12] = criar_carta(
        "Qual operador acessa campo de struct via ponteiro?",
        ".",
        "->",
        "&",
        "*",
        "::",
        2, 1, 1, 2, 13
    );

    cartas[13] = criar_carta(
        "Qual funcao aloca memoria dinamica em C?",
        "malloc",
        "printf",
        "scanf",
        "sizeof",
        "free",
        1, 1, 1, 2, 14
    );

    cartas[14] = criar_carta(
        "Qual operador obtem o endereco de uma variavel?",
        "&",
        "*",
        "->",
        ".",
        "%",
        1, 1, 1, 2, 15
    );

    cartas[15] = criar_carta(
        "Qual operador dereferencia um ponteiro?",
        "*",
        "&",
        "->",
        ".",
        "++",
        1, 1, 1, 2, 16
    );

    cartas[16] = criar_carta(
        "Um ponteiro armazena:",
        "Um valor inteiro fixo",
        "Endereco de memoria",
        "Uma string",
        "Um arquivo aberto",
        "Um vetor de chars",
        2, 2, 2, 2, 17
    );

    cartas[17] = criar_carta(
        "Ao passar um ponteiro para funcao, e possivel:",
        "Alterar o conteudo apontado",
        "Somente ler o valor",
        "Alterar o codigo fonte",
        "Mover o ponteiro para NULL automaticamente",
        "Converter para float",
        1, 2, 2, 2, 18
    );

    cartas[18] = criar_carta(
        "Qual macro representa ponteiro nulo em C?",
        "NULL",
        "NIL",
        "NONE",
        "ZERO",
        "VOID",
        1, 2, 2, 2, 19
    );

    cartas[19] = criar_carta(
        "Qual funcao altera o tamanho de um bloco alocado?",
        "realloc",
        "malloc",
        "calloc",
        "free",
        "sizeof",
        1, 2, 2, 2, 20
    );

    cartas[20] = criar_carta(
        "sizeof(int*) retorna:",
        "Tamanho do int",
        "Tamanho do ponteiro",
        "Sempre 4",
        "Sempre 8",
        "Erro de compilacao",
        2, 3, 3, 2, 21
    );

    cartas[21] = criar_carta(
        "Apos usar malloc, deve-se:",
        "Ignorar o retorno",
        "Verificar NULL e usar free quando nao precisar",
        "Sempre usar scanf",
        "Reiniciar o programa",
        "Declarar outra variavel global",
        2, 3, 3, 2, 22
    );

    cartas[22] = criar_carta(
        "Qual problema ocorre ao esquecer de liberar memoria alocada?",
        "Vazamento de memoria",
        "Erro de sintaxe",
        "Conversao automatica",
        "Overflow de inteiro",
        "Mudanca de tipo",
        1, 3, 3, 2, 23
    );

    cartas[23] = criar_carta(
        "O que pode acontecer ao dereferenciar um ponteiro NULL?",
        "Falha de execucao",
        "Sempre retorna 0",
        "Cria memoria automaticamente",
        "Imprime lixo",
        "Corrige para endereco 1",
        1, 3, 3, 2, 24
    );
}

// Inicializa posicao, nome e id de cada jogador.
void inicializar_jogadores(tp_player jogadores[], int quantidade) {
    int i;

    // Define posicao inicial e nomes padrao.
    for (i = 0; i < quantidade; i++) {
        jogadores[i].posicao = 1;
        montar_nome_jogador(jogadores[i].nome, sizeof(jogadores[i].nome), i + 1);
        jogadores[i].id_player = i + 1;
    }
}

// Preenche a fila de turnos respeitando a ordem de entrada.
void colocar_jogadores_na_fila(tp_player jogadores[], int quantidade, tp_fila *fila_turnos) {
    int i;

    // Fila vazia antes de inserir a ordem dos jogadores.
    inicializa_fila(fila_turnos);

    // Insere IDs em ordem de entrada.
    for (i = 0; i < quantidade; i++) {
        insere_fila(fila_turnos, jogadores[i].id_player);
    }
}

// Embaralha ids com o algoritmo Fisher-Yates.
// 
// Algoritmo Fisher-Yates (também conhecido como Knuth Shuffle):
// 1. Começa do último elemento do array (índice total-1).
// 2. Para cada posição i, seleciona aleatoriamente um índice j entre 0 e i.
// 3. Troca o elemento na posição i com o elemento na posição j.
// 4. Avança para a posição anterior (i-1) e repete até o início.
// 5. Resultado: permutação aleatória com distribuição uniforme.
//
// Exemplo com [0, 1, 2, 3]:
// - i=3: j=rand()%4, troca ids[3] com ids[j]
// - i=2: j=rand()%3, troca ids[2] com ids[j]
// - i=1: j=rand()%2, troca ids[1] com ids[j]
// - i=0: fim (um elemento já está garantido)
void embaralhar_ids(tp_item indice_cartas[], int total) {
    int i, j;
    tp_item aux;

    // Itera do último índice até o segundo (total-1 até 1).
    // Não precisa processar o índice 0 pois só resta um elemento.
    for (i = total - 1; i > 0; i--) {
        // Escolhe aleatoriamente um índice j entre 0 e i (inclusive).
        // rand() % (i + 1) gera um número no intervalo [0, i].
        j = rand() % (i + 1);
        
        // Troca o elemento na posição i com o elemento na posição j.
        // Usa variável auxiliar para não perder dados.
        aux = indice_cartas[i];
        indice_cartas[i] = indice_cartas[j];
        indice_cartas[j] = aux;
    }
}

// Filtra ids de cartas por unidade e dificuldade.
// Filtra índices de cartas por unidade e dificuldade.
int coletar_ids_unidade_dificuldade(
    tp_carta banco[],
    int total,
    int unidade,
    int dificuldade,
    tp_item indice_cartas[]
) {
    int i;
    int quantidade = 0;

    // Varre o banco e coleta os indices que combinam.
    for (i = 0; i < total; i++) {
        if (banco[i].unidade == unidade && banco[i].dificuldade == dificuldade) {
            indice_cartas[quantidade] = i;
            quantidade++;
        }
    }

    return quantidade;
}

// Recarrega a pilha de uma unidade/dificuldade.
void recarregar_pilha_unidade(
    tp_pilha *pilha,
    tp_carta banco[],
    int total,
    int unidade,
    int dificuldade
) {
    tp_item indice_cartas[24];
    int quantidade;
    int i;

    // Garante pilha vazia antes de recarregar.
    inicializa_pilha(pilha);

    quantidade = coletar_ids_unidade_dificuldade(
        banco,
        total,
        unidade,
        dificuldade,
        indice_cartas
    );

    // Se nao houver perguntas, interrompe.
    if (quantidade == 0) {
        return;
    }

    // Embaralha os índices antes de empilhar.
    embaralhar_ids(indice_cartas, quantidade);

    // Empilha todos os índices para sorteio.
    for (i = 0; i < quantidade; i++) {
        push(pilha, indice_cartas[i]);
    }
}

// Carrega pilhas de perguntas para cada unidade/dificuldade.
void carregar_perguntas(
    tp_pilha pilhas[3][3],
    tp_carta banco[],
    int total
) {
    int unidade;
    int dificuldade;

    // Prepara pilhas para todas as unidades e dificuldades.
    for (unidade = 1; unidade <= 3; unidade++) {
        for (dificuldade = 1; dificuldade <= 3; dificuldade++) {
            recarregar_pilha_unidade(
                &pilhas[unidade - 1][dificuldade - 1],
                banco,
                total,
                unidade,
                dificuldade
            );
        }
    }
}

// Retira uma carta da pilha, reembaralhando quando necessario.
int sortear_carta(
    tp_pilha pilhas[3][3],
    tp_carta banco[],
    int total,
    int unidade,
    int dificuldade,
    tp_carta *carta_sorteada
) {
    tp_item id_carta;
    tp_pilha *pilha;

    // Valida unidade e dificuldade informadas.
    if (unidade < 1 || unidade > 3) {
        return 0;
    }

    if (dificuldade < 1 || dificuldade > 3) {
        return 0;
    }

    // Seleciona a pilha correta para sorteio.
    pilha = &pilhas[unidade - 1][dificuldade - 1];

    // Recarrega quando todas as perguntas dessa pilha foram usadas.
    if (pilha_vazia(pilha)) {
        printf(
            "\nPerguntas da unidade %d dificuldade %d esgotadas. Reembaralhando...\n",
            unidade,
            dificuldade
        );
        recarregar_pilha_unidade(pilha, banco, total, unidade, dificuldade);
    }

    // Retira o id da carta do topo.
    if (!pop(pilha, &id_carta)) {
        return 0;
    }

    if (id_carta < 0 || id_carta >= total) {
        return 0;
    }

    // Copia a carta sorteada para retorno.
    *carta_sorteada = banco[id_carta];
    return 1;
}

#endif
