#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>

//Infromações cartas
typedef struct {
    char pergunta[500];
    char alternativas [500];
    int resposta;
    int avanco;
    int dificuldade;
    int id_carta;
} tp_carta;

//Informações Player
typedef struct {
    int posicao;
    char nome[50];
    int id_player;
} tp_player;




//Definição dos players (Posição inicial, Nome e ID)
static tp_player player[4] = {
    {1, "Player 1", 1},
    {1, "Player 2", 2},
    {1, "Player 3", 3},
    {1, "Player 4", 4}
};


//Definição das cartas (Pergunta, Alternativas, Resposta, Avanço, dificuldade 1-3, ID)
static tp_carta unidade1[12] = {
{
    //Pergunta 1:
    "Sobre a estrutura struct em C, é correto afirmar que:\n",
    //Alternativas:
    "1) Permite armazenar apenas dados do mesmo tipo. \n"
    "2) É uma estrutura de dados homogênea. \n"
    "3) Permite agrupar variáveis de tipos diferentes sob um mesmo nome. \n" 
    "4) Não pode conter vetores em seu interior. \n"
    "5) É declarada obrigatoriamente dentro da função. \n",
    //Resposta:
    3,
    //Avanço:
    1,
    //Dificuldade:
    1,
    //ID:
    1 
},
{
    //Pergunta 2:
    "Qual a principal característica de um vetor (array) na linguagem C?\n",
    //Alternativas:
    "1) Seus elementos são espalhados aleatoriamente na memória. \n"
    "2) O tamanho pode ser alterado dinamicamente durante a execução sem comandos especiais. \n" 
    "3) É uma estrutura heterogênea. \n" 
    "4) Ocupa um bloco de memória contíguo e possui tamanho fixo após declarado. \n"
    "5) O primeiro elemento de um vetor v[10] é acessado pelo índice v[1]. \n",
    //Resposta:
    4,
    //Avanço:
    1,
    //Dificuldade:
    1,
    //ID:
    2
},
{
    //Pergunta 3:
    "Qual estrutura de repetição garante que o bloco de código seja executado pelo menos uma vez?\n",
    //Alternativas:
    "1) while \n" 
    "2) for \n" 
    "3) switch case \n" 
    "4) if \n"
    "5) do while \n",
    //Resposta:
    5,
    //Avanço:
    1,
    //Dificuldade:
    1,
    //ID:
    3
},
{
    //Pergunta 4:
    "O que acontece se tentarmos acessar o índice 10 de um vetor declarado como int vet[10]?\n",
    //Alternativas:
    "1) O programa acessa o último elemento do vetor. \n" 
    "2) O programa acessa uma posição de memória fora do limite (lixo de memória ou erro). \n" 
    "3) O compilador corrige automaticamente para o índice 9. \n"
    "4) O valor retornado será sempre 0. \n"
    "5) O vetor aumenta de tamanho automaticamente. \n",
    //Resposta:
    2,
    //Avanço:
    1,
    //Dificuldade:
    1,
    //ID:
    4
},
{
    //Pergunta 5:
    "Na estrutura de dados do tipo FILA (Queue), qual a regra de inserção e remoção?\n",
    //Alternativas:
    "1) LIFO (Last In, First Out) - O último a entrar é o primeiro a sair. \n" 
    "2) FIFO (First In, First Out) - O primeiro a entrar é o primeiro a sair. \n" 
    "3) O usuário escolhe qualquer elemento para remover no meio da estrutura. \n" 
    "4) A remoção acontece no fim e a inserção no início. \n"
    "5) Não existe ordem específica para saída. \n",
    //Resposta:
    2,
    //Avanço:
    2,
    //Dificuldade:
    2,
    //ID:
    5
},
{
    //Pergunta 6:
    "Na estrutura de dados do tipo PILHA (Stack), o termo \"POP\" refere-se a:\n",
    //Alternativas:
    "1) Inserir um elemento no topo. \n" 
    "2) Limpar todos os elementos da memória. \n" 
    "3) Remover o elemento que está no topo. \n" 
    "4) Consultar o elemento da base sem removê-lo. \n"
    "5) Inverter a ordem dos elementos. \n",
    //Resposta:
    3,
    //Avanço:
    2,
    //Dificuldade:
    2,
    //ID:
    6
},
{
    //Pergunta 7:
    "Considere o operador %% (módulo). Qual o resultado da expressão (15 %% 4)?\n",
    //Alternativas:
    "1) 3.75 \n" 
    "2) 3 \n" 
    "3) 1 \n" 
    "4) 0 \n"
    "5) 4 \n",
    //Resposta:
    2,
    //Avanço:
    2,
    //Dificuldade:
    2,
    //ID:
    7
},
{
    //Pergunta 8:
    "Se uma Pilha contendo os números [10, 20, 30] (sendo 30 o topo) sofrer uma operação de PUSH(40) e logo em seguida um POP(), qual será o elemento no topo?\n",
    //Alternativas:
    "1) 40 \n" 
    "2) 10 \n" 
    "3) 20 \n" 
    "4) 30 \n"
    "5) A pilha ficará vazia. \n",
    //Resposta:
    4,
    //Avanço:
    2,
    //Dificuldade:
    2,
    //ID:
    8
},
{
    //Pergunta 9:
    "Em uma Fila implementada sobre um vetor de 5 posições, após inserir 5 elementos, qual a condição do ponteiro/índice de \"Fim\"?\n",
    //Alternativas:
    "1) Aponta para a posição 0. \n" 
    "2) Aponta para a posição 4. \n" 
    "3) Aponta para a posição 5 (fora do vetor). \n" 
    "4) Retorna ao valor nulo automaticamente. \n"
    "5) É resetado para o valor do ponteiro de \"Início\". \n",
    //Resposta:
    2,
    //Avanço:
    3,
    //Dificuldade:
    3,
    //ID:
    9
},
{
    //Pergunta 10:
    "Qual a principal diferença lógica entre uma Pilha e uma Fila?\n",
    //Alternativas:
    "1) A Pilha usa struct e a Fila não. \n" 
    "2) A Fila é estática e a Pilha é sempre dinâmica. \n" 
    "3) O ponto de saída: na Pilha sai o mais recente; na Fila sai o mais antigo. \n" 
    "4) Na Pilha os dados são ordenados, na Fila não. \n"
    "5) Não há diferença, são apenas nomes diferentes para vetores. \n",
    //Resposta:
    3,
    //Avanço:
    3,
    //Dificuldade:
    3,
    //ID:
    10
},
{
    //Pergunta 11:
    "Se utilizarmos um for(i = 0; i < n; i++) para buscar um elemento em um vetor desordenado, qual o pior cenário de busca?\n",
    //Alternativas:
    "1) O elemento estar na primeira posição. \n" 
    "2) O elemento estar no meio do vetor. \n" 
    "3) O elemento não existir ou estar na última posição (índice n-1). \n" 
    "4) O vetor estar vazio. \n"
    "5) O compilador encontrar um erro de sintaxe. \n",
    //Resposta:
    3,
    //Avanço:
    3,
    //Dificuldade:
    3,
    //ID:
    11
},
{
    //Pergunta 12:
    "Ao passar uma struct para uma função em C, o que acontece por padrão? \n",
    //Alternativas:
    "1) A função altera diretamente os dados da struct original. \n" 
    "2) Uma cópia de toda a struct é enviada para a função (passagem por valor). \n" 
    "3) Apenas o primeiro campo da struct é enviado. \n" 
    "4) O programa trava, pois structs não podem ser passadas para funções. \n"
    "5) A struct é convertida automaticamente em um vetor de inteiros. \n",
    //Resposta:
    2,
    //Avanço:
    3,
    //Dificuldade:
    3,
    //ID:
    12
}
};

static tp_carta unidade2[12];
static tp_carta unidade3[12];



#endif
