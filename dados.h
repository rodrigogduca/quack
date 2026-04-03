#ifndef DADOS_H
#define DADOS_H

/*
 * dados.h
 * -----------------------------------------------------------------------------
 * Biblioteca de dados estaticos do jogo.
 * Este arquivo contem:
 * 1) Estruturas base (carta e jogador).
 * 2) Personagens disponiveis.
 * 3) Cartas da unidade 1.
 *
 * Observacao sobre codificacao:
 * Os textos estao em ASCII para evitar caracteres quebrados no terminal.
*/

/*
 * tp_carta
 * -----------------------------------------------------------------------------
 * pergunta    : enunciado da pergunta.
 * alternativas: bloco de alternativas ja formatado para impressao.
 * resposta    : alternativa correta (1 a 5).
 * avanco      : casas de bonus ao acertar.
 * dificuldade : nivel da carta (1 facil, 2 medio, 3 dificil).
 * id_carta    : identificador numerico unico da carta.
 */
typedef struct {
    char pergunta[500];
    char alternativas[500];
    int resposta;
    int avanco;
    int dificuldade;
    int id_carta;
} tp_carta;

/*
 * tp_player
 * -----------------------------------------------------------------------------
 * posicao    : casa atual no tabuleiro (1 a 25).
 * personagem : nome do personagem escolhido.
 * moedas     : campo reservado para futuras mecanicas.
 * estrelas   : campo reservado para futuras mecanicas.
 * id_player  : identificador do personagem no vetor base.
 */
typedef struct {
    int posicao;
    char personagem[20];
    int moedas;
    int estrelas;
    int id_player;
} tp_player;

/*
 * player
 * -----------------------------------------------------------------------------
 * Base de personagens disponiveis para selecao no inicio da partida.
 * Todos iniciam na posicao 0, com moedas/estrelas zeradas.
 */
static tp_player player[4] = {
    {0, "Pato Donald", 0, 0, 1},
    {0, "Tio Patinhas", 0, 0, 2},
    {0, "Patolino", 0, 0, 3},
    {0, "Pato", 0, 0, 4}
};

/*
 * unidade1
 * -----------------------------------------------------------------------------
 * Baralho principal atualmente ativo no jogo.
 * Estrutura de cada item:
 * {pergunta, alternativas, resposta, avanco, dificuldade, id}
 */
static tp_carta unidade1[12] = {
{
    // Pergunta 1
    "Sobre a estrutura struct em C, e correto afirmar que:\n",
    // Alternativas
    "1) Permite armazenar apenas dados do mesmo tipo. \n"
    "2) E uma estrutura de dados homogenea. \n"
    "3) Permite agrupar variaveis de tipos diferentes sob um mesmo nome. \n" 
    "4) Nao pode conter vetores em seu interior. \n"
    "5) E declarada obrigatoriamente dentro da funcao. \n",
    // Resposta
    3,
    // Avanco
    1,
    // Dificuldade
    1,
    // ID
    1 
},
{
    // Pergunta 2
    "Qual a principal caracteristica de um vetor (array) na linguagem C?\n",
    // Alternativas
    "1) Seus elementos sao espalhados aleatoriamente na memoria. \n"
    "2) O tamanho pode ser alterado dinamicamente durante a execucao sem comandos especiais. \n" 
    "3) E uma estrutura heterogenea. \n" 
    "4) Ocupa um bloco de memoria contiguo e possui tamanho fixo apos declarado. \n"
    "5) O primeiro elemento de um vetor v[10] e acessado pelo indice v[1]. \n",
    // Resposta
    4,
    // Avanco
    1,
    // Dificuldade
    1,
    // ID
    2
},
{
    // Pergunta 3
    "Qual estrutura de repeticao garante que o bloco de codigo seja executado pelo menos uma vez?\n",
    // Alternativas
    "1) while \n" 
    "2) for \n" 
    "3) switch case \n" 
    "4) if \n"
    "5) do while \n",
    // Resposta
    5,
    // Avanco
    1,
    // Dificuldade
    1,
    // ID
    3
},
{
    // Pergunta 4
    "O que acontece se tentarmos acessar o indice 10 de um vetor declarado como int vet[10]?\n",
    // Alternativas
    "1) O programa acessa o ultimo elemento do vetor. \n" 
    "2) O programa acessa uma posicao de memoria fora do limite (lixo de memoria ou erro). \n" 
    "3) O compilador corrige automaticamente para o indice 9. \n"
    "4) O valor retornado sera sempre 0. \n"
    "5) O vetor aumenta de tamanho automaticamente. \n",
    // Resposta
    2,
    // Avanco
    1,
    // Dificuldade
    1,
    // ID
    4
},
{
    // Pergunta 5
    "Na estrutura de dados do tipo FILA (Queue), qual a regra de insercao e remocao?\n",
    // Alternativas
    "1) LIFO (Last In, First Out) - O ultimo a entrar e o primeiro a sair. \n" 
    "2) FIFO (First In, First Out) - O primeiro a entrar e o primeiro a sair. \n" 
    "3) O usuario escolhe qualquer elemento para remover no meio da estrutura. \n" 
    "4) A remocao acontece no fim e a insercao no inicio. \n"
    "5) Nao existe ordem especifica para saida. \n",
    // Resposta
    2,
    // Avanco
    2,
    // Dificuldade
    2,
    // ID
    5
},
{
    // Pergunta 6
    "Na estrutura de dados do tipo PILHA (Stack), o termo \"POP\" refere-se a:\n",
    // Alternativas
    "1) Inserir um elemento no topo. \n" 
    "2) Limpar todos os elementos da memoria. \n" 
    "3) Remover o elemento que esta no topo. \n" 
    "4) Consultar o elemento da base sem remove-lo. \n"
    "5) Inverter a ordem dos elementos. \n",
    // Resposta
    3,
    // Avanco
    2,
    // Dificuldade
    2,
    // ID
    6
},
{
    // Pergunta 7
    "Considere o operador % (modulo). Qual o resultado da expressao (15 % 4)?\n",
    // Alternativas
    "1) 3.75 \n" 
    "2) 3 \n" 
    "3) 1 \n" 
    "4) 0 \n"
    "5) 4 \n",
    // Resposta
    2,
    // Avanco
    2,
    // Dificuldade
    2,
    // ID
    7
},
{
    // Pergunta 8
    "Se uma Pilha contendo os numeros [10, 20, 30] (sendo 30 o topo) sofrer uma operacao de PUSH(40) e logo em seguida um POP(), qual sera o elemento no topo?\n",
    // Alternativas
    "1) 40 \n" 
    "2) 10 \n" 
    "3) 20 \n" 
    "4) 30 \n"
    "5) A pilha ficara vazia. \n",
    // Resposta
    4,
    // Avanco
    2,
    // Dificuldade
    2,
    // ID
    8
},
{
    // Pergunta 9
    "Em uma Fila implementada sobre um vetor de 5 posicoes, apos inserir 5 elementos, qual a condicao do ponteiro/indice de \"Fim\"?\n",
    // Alternativas
    "1) Aponta para a posicao 0. \n" 
    "2) Aponta para a posicao 4. \n" 
    "3) Aponta para a posicao 5 (fora do vetor). \n" 
    "4) Retorna ao valor nulo automaticamente. \n"
    "5) E resetado para o valor do ponteiro de \"Inicio\". \n",
    // Resposta
    2,
    // Avanco
    3,
    // Dificuldade
    3,
    // ID
    9
},
{
    // Pergunta 10
    "Qual a principal diferenca logica entre uma Pilha e uma Fila?\n",
    // Alternativas
    "1) A Pilha usa struct e a Fila nao. \n" 
    "2) A Fila e estatica e a Pilha e sempre dinamica. \n" 
    "3) O ponto de saida: na Pilha sai o mais recente; na Fila sai o mais antigo. \n" 
    "4) Na Pilha os dados sao ordenados, na Fila nao. \n"
    "5) Nao ha diferenca, sao apenas nomes diferentes para vetores. \n",
    // Resposta
    3,
    // Avanco
    3,
    // Dificuldade
    3,
    // ID
    10
},
{
    // Pergunta 11
    "Se utilizarmos um for(i = 0; i < n; i++) para buscar um elemento em um vetor desordenado, qual o pior cenario de busca?\n",
    // Alternativas
    "1) O elemento estar na primeira posicao. \n" 
    "2) O elemento estar no meio do vetor. \n" 
    "3) O elemento nao existir ou estar na ultima posicao (indice n-1). \n" 
    "4) O vetor estar vazio. \n"
    "5) O compilador encontrar um erro de sintaxe. \n",
    // Resposta
    3,
    // Avanco
    3,
    // Dificuldade
    3,
    // ID
    11
},
{
    // Pergunta 12
    "Ao passar uma struct para uma funcao em C, o que acontece por padrao? \n",
    // Alternativas
    "1) A funcao altera diretamente os dados da struct original. \n" 
    "2) Uma copia de toda a struct e enviada para a funcao (passagem por valor). \n" 
    "3) Apenas o primeiro campo da struct e enviado. \n" 
    "4) O programa trava, pois structs nao podem ser passadas para funcoes. \n"
    "5) A struct e convertida automaticamente em um vetor de inteiros. \n",
    // Resposta
    2,
    // Avanco
    3,
    // Dificuldade
    3,
    // ID
    12
}
};

/*
 * unidade2 e unidade3
 * -----------------------------------------------------------------------------
 * Estruturas reservadas para expansao futura do banco de cartas.
 */
static tp_carta unidade2[12];
/*
 * Criar as perguntas, alternativas, respostas, avancos, dificuldades e IDs
 * para a unidade 2 seguindo o mesmo formato da unidade 1.
 */
   
static tp_carta unidade3[12];
/*
 * Criar as perguntas, alternativas, respostas, avancos, dificuldades e IDs
 * para a unidade 3 seguindo o mesmo formato da unidade 1.
 */


#endif
