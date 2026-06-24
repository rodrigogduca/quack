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
    char alternativas[5][150]; // Vetor com as alternativas numeradas.
    int resposta; // Alternativa correta (1..5).
    int avanco; // Casas a avancar se acertar.
    int dificuldade; // Nivel 1..3.
    int unidade; // Unidade associada (1..3).
    int id_carta; // Identificador interno da carta.
    char tema[50]; // Tema da pergunta
    char subtema[50]; // Subtema da pergunta
} tp_carta;

// Estrutura de dados que representa cada jogador no jogo.
typedef struct {
    int posicao; // Casa atual no tabuleiro.
    char nome[50]; // Nome exibido no jogo.
    int id_player; // Identificador de turno (1..4).
    int historico_casas[100]; // Vetor com o historico das casas percorridas para o ranking final.
    int total_casas_visitadas; // Contador total de casas armazenadas no historico.
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

// Preenche o banco fixo de perguntas a partir de um vetor ja estruturado.
void montar_banco_cartas(tp_carta cartas[36]) {
    int i;

    // Vetor-base de cartas:
    // { pergunta, {alternativas[0..4]}, resposta, avanco, dificuldade, unidade, id, tema, subtema }.
    // A ordem segue o projeto: 3 unidades x 3 dificuldades x 4 cartas por dificuldade.
    tp_carta banco_padrao[36] = {
        {
            "Qual operador realiza atribuicao em C?",
            {"=", "==", "!=", "<=", "++"},
            1, 1, 1, 1, 1, "C Basico", "Operadores"
        },
        {
            "Qual simbolo inicia um comentario de uma linha em C?",
            {"/*", "//", "#", "<!--", "--"},
            2, 1, 1, 1, 2, "C Basico", "Comentarios"
        },
        {
            "Qual simbolo termina uma instrucao em C?",
            {";", ",", ":", ".", "?"},
            1, 1, 1, 1, 3, "C Basico", "Sintaxe"
        },
        {
            "Qual tipo armazena um caractere unico em C?",
            {"char", "int", "float", "double", "string"},
            1, 1, 1, 1, 4, "C Basico", "Tipos de Dados"
        },
        {
            "Qual e o primeiro indice valido de um vetor int v[10]?",
            {"1", "0", "-1", "10", "Depende do compilador"},
            2, 2, 2, 1, 5, "Vetores", "Indices"
        },
        {
            "Qual o resultado da expressao (7 % 3)?",
            {"0", "1", "2", "3", "7"},
            2, 2, 2, 1, 6, "C Basico", "Operadores"
        },
        {
            "Qual operador logico representa 'E' em C?",
            {"&&", "||", "!", "&", "|"},
            1, 2, 2, 1, 7, "C Basico", "Operadores Logicos"
        },
        {
            "Qual funcao le uma linha inteira com espacos do teclado?",
            {"gets", "fgets", "scanf", "printf", "puts"},
            2, 2, 2, 1, 8, "C Basico", "Entrada de Dados"
        },
        {
            "O que acontece ao acessar v[10] em int v[10]?",
            {"Acessa o ultimo elemento", "Acesso fora do limite (erro)", "O compilador corrige para v[9]", "Retorna sempre 0", "O vetor aumenta de tamanho"},
            2, 3, 3, 1, 9, "Vetores", "Limites"
        },
        {
            "O que pode ocorrer ao usar printf com variavel nao inicializada?",
            {"Erro de compilacao", "Valor lixo ou comportamento indefinido", "Sempre imprime zero", "Sempre imprime -1", "O programa termina automaticamente"},
            2, 3, 3, 1, 10, "C Basico", "Variaveis"
        },
        {
            "Qual o resultado de 5 / 2 usando inteiros?",
            {"2", "2.5", "3", "1", "0"},
            1, 3, 3, 1, 11, "C Basico", "Operadores"
        },
        {
            "Qual o ultimo indice valido de um vetor int v[10]?",
            {"9", "10", "-1", "1", "Depende do compilador"},
            1, 3, 3, 1, 12, "Vetores", "Indices"
        },
        {
            "Qual operador acessa campo de struct via ponteiro?",
            {".", "->", "&", "*", "::"},
            2, 1, 1, 2, 13, "Struct", "Ponteiros"
        },
        {
            "Qual funcao aloca memoria dinamica em C?",
            {"malloc", "printf", "scanf", "sizeof", "free"},
            1, 1, 1, 2, 14, "Alocacao Dinamica", "Funcoes"
        },
        {
            "Qual operador obtem o endereco de uma variavel?",
            {"&", "*", "->", ".", "%"},
            1, 1, 1, 2, 15, "Ponteiros", "Operadores"
        },
        {
            "Qual operador dereferencia um ponteiro?",
            {"*", "&", "->", ".", "++"},
            1, 1, 1, 2, 16, "Ponteiros", "Operadores"
        },
        {
            "Um ponteiro armazena:",
            {"Um valor inteiro fixo", "Endereco de memoria", "Uma string", "Um arquivo aberto", "Um vetor de chars"},
            2, 2, 2, 2, 17, "Ponteiros", "Memoria"
        },
        {
            "Ao passar um ponteiro para funcao, e possivel:",
            {"Alterar o conteudo apontado", "Somente ler o valor", "Alterar o codigo fonte", "Mover o ponteiro para NULL automaticamente", "Converter para float"},
            1, 2, 2, 2, 18, "Ponteiros", "Funcoes"
        },
        {
            "Qual macro representa ponteiro nulo em C?",
            {"NULL", "NIL", "NONE", "ZERO", "VOID"},
            1, 2, 2, 2, 19, "Ponteiros", "NULL"
        },
        {
            "Qual funcao altera o tamanho de um bloco alocado?",
            {"realloc", "malloc", "calloc", "free", "sizeof"},
            1, 2, 2, 2, 20, "Alocacao Dinamica", "Funcoes"
        },
        {
            "sizeof(int*) retorna:",
            {"Tamanho do int", "Tamanho do ponteiro", "Sempre 4", "Sempre 8", "Erro de compilacao"},
            2, 3, 3, 2, 21, "Ponteiros", "Memoria"
        },
        {
            "Apos usar malloc, deve-se:",
            {"Ignorar o retorno", "Verificar NULL e usar free quando nao precisar", "Sempre usar scanf", "Reiniciar o programa", "Declarar outra variavel global"},
            2, 3, 3, 2, 22, "Alocacao Dinamica", "Free"
        },
        {
            "Qual problema ocorre ao esquecer de liberar memoria alocada?",
            {"Vazamento de memoria", "Erro de sintaxe", "Conversao automatica", "Overflow de inteiro", "Mudanca de tipo"},
            1, 3, 3, 2, 23, "Alocacao Dinamica", "Perda de memoria"
        },
        {
            "O que pode acontecer ao dereferenciar um ponteiro NULL?",
            {"Falha de execucao", "Sempre retorna 0", "Cria memoria automaticamente", "Imprime lixo", "Corrige para endereco 1"},
            1, 3, 3, 2, 24, "Ponteiros", "NULL"
        },
        {
            "Qual estrutura representa naturalmente relacoes hierarquicas, como pastas dentro de pastas?",
            {"Fila", "Pilha", "Arvore", "Vetor simples", "Variavel escalar"},
            3, 1, 1, 3, 25, "Arvores", "Conceito"
        },
        {
            "Qual e a propriedade fundamental que define uma Arvore Binaria de Busca (BST)?",
            {"Todos os nos da arvore devem possuir exatamente dois nos filhos.", "Os nos da subarvore esquerda possuem valores menores que o no pai, e os nos da subarvore direita possuem valores maiores.", "O caminho da raiz ate qualquer folha deve ter exatamente o mesmo comprimento.", "Os nos da subarvore esquerda sao maiores que o no raiz, e os da direita sao menores.", "A arvore deve estar perfeitamente balanceada em todos os seus niveis."},
            2, 1, 1, 3, 26, "Arvore Binaria", "Busca"
        },
        {
            "Qual e o principal objetivo de se utilizar uma Arvore AVL em vez de uma Arvore Binaria de Busca (BST) convencional?",
            {"Permitir o armazenamento de dados diretamente em arquivos de texto sequenciais.", "Eliminar completamente a necessidade de ponteiros ou referencias na implementacao.", "Garantir que as operacoes de busca, insercao e remocao mantenham complexidade de tempo no pior caso de O(log n) por meio do balanceamento automatico.", "Permitir que cada no tenha mais de dois filhos para economizar memoria RAM.", "Facilitar exclusivamente a ordenacao lexicografica de cadeias de caracteres."},
            3, 1, 1, 3, 27, "Arvore AVL", "Balanceamento"
        },
        {
            "Na manipulacao de arquivos em linguagem C (utilizando a biblioteca stdio.h), qual e a funcao do ponteiro do tipo FILE*?",
            {"Alocar memoria RAM dinamicamente para os novos nos de uma arvore.", "Controlar o fluxo de execucao sincrona e concorrencia do algoritmo.", "Armazenar o conteudo completo do arquivo diretamente na memoria cache do processador.", "Servir como uma referencia/canal para abrir, ler, escrever ou navegar dentro de um arquivo especifico no disco.", "Criptografar automaticamente os dados antes de salva-los no sistema de arquivos."},
            4, 1, 1, 3, 28, "Arquivos", "Manipulacao"
        },
        {
            "A estrutura de dados conhecida como Arvore Digital (Trie) e amplamente utilizada para aplicacoes especificas. Qual e o cenario de uso mais adequado para uma Trie?",
            {"Armazenamento de registros numericos de ponto flutuante com alta precisao.", "Balanceamento estrito de chaves inteiras dispersas em sistemas de paginacao.", "Sistemas de busca por prefixos, mecanismos de autocompletar texto (autocomplete) e dicionarios de strings.", "Gerenciamento de blocos de memoria secundaria em sistemas de arquivos magneticos.", "Ordenacao de vetores numericos de tamanho fixo de forma estavel."},
            3, 2, 2, 3, 29, "Arvore Trie", "Prefixos"
        },
        {
            "Considere uma Arvore Binaria de Busca (BST) inicialmente vazia. Se inserirmos as chaves na seguinte sequencia exata: [10, 5, 15, 3, 7], qual sera o resultado do percurso em-ordem (in-order) dessa arvore?",
            {"[3, 5, 7, 10, 15]", "[10, 5, 3, 7, 15]", "[3, 7, 5, 15, 10]", "[15, 10, 7, 5, 3]", "[10, 15, 5, 7, 3]"},
            1, 2, 2, 3, 30, "Arvore Binaria", "Percurso"
        },
        {
            "Durante a insercao de um elemento em uma Arvore AVL, o balanceamento de um no foi violado apos uma insercao na subarvore direita do filho direito desse no (caso Direita-Direita). Qual rotacao corrige o fator de balanceamento?",
            {"Rotacao Dupla Direita (ou Esquerda-Direita).", "Rotacao Simples a Direita.", "Rotacao Dupla Esquerda (ou Direita-Esquerda).", "Rotacao Simples a Esquerda.", "Nenhuma rotacao, apenas a atualizacao dos fatores de carga."},
            4, 2, 2, 3, 31, "Arvore AVL", "Rotacao"
        },
        {
            "Em um arquivo aberto com fopen no modo \"r\", qual operacao e permitida?",
            {"Somente leitura do conteudo existente.", "Somente escrita no final do arquivo.", "Criar sempre um arquivo novo.", "Apagar automaticamente o conteudo anterior.", "Executar comandos do sistema operacional."},
            1, 2, 2, 3, 32, "Arquivos", "Modos"
        },
        {
            "No contexto de persistencia de dados, qual e a principal vantagem de se implementar indices baseados em estruturas de arvores (em disco) em vez de realizar buscas sequenciais diretamente no arquivo de dados?",
            {"Reducao drastica do numero de acessos a memoria secundaria (disco), que e ordens de grandeza mais lenta que a memoria RAM.", "Eliminacao completa do uso de buffers de entrada e saida (I/O) no sistema operacional.", "Garantia de que o arquivo final indexado ocupe menos espaco fisico no disco rigido que o original.", "Permissao para que multiplos processos escrevam no mesmo byte simultaneamente sem travas de concorrencia.", "Transformacao em tempo de execucao de arquivos binarios em arquivos de texto legiveis."},
            1, 3, 3, 3, 33, "Arquivos", "Persistencia"
        },
        {
            "Considere uma Arvore AVL onde um no X possui fator de balanceamento igual a -2 (calculado como altura(esquerda) - altura(direita)). Sabe-se que o filho a direita de X possui fator de balanceamento igual a +1. Para rebalancear a arvore a partir do no X, qual sequencia de rotacoes deve ser efetuada?",
            {"Uma unica rotacao simples a direita no no X.", "Uma rotacao simples a esquerda no filho de X, seguida por uma rotacao simples a direita no no X.", "Uma rotacao simples a direita no filho de X, seguida por uma rotacao simples a esquerda no no X (Rotacao Dupla Esquerda).", "Duas rotacoes simples consecutivas a esquerda diretamente no no X.", "Uma rotacao dupla a direita diretamente no filho de X."},
            3, 3, 3, 3, 34, "Arvore AVL", "Rotacao"
        },
        {
            "Em relacao a eficiencia de espaco e tempo de uma Arvore Trie padrao, qual alternativa descreve corretamente uma caracteristica complexa dessa estrutura?",
            {"O tempo de busca de uma chave de comprimento L depende diretamente do numero total de chaves (N) armazenadas na arvore, ou seja, O(N * L).", "A remocao de uma palavra em uma Trie exige obrigatoriamente a reconstrucao completa de toda a arvore a partir do no raiz.", "Ela consome menos memoria do que uma arvore binaria para chaves numericas inteiras aleatorias e altamente esparsas.", "O pior caso de espaco pode ser alto: cada no pode reservar ponteiros para todo o alfabeto, mesmo quando poucos prefixos sao compartilhados.", "Os nos de uma Trie armazenam obrigatoriamente chaves inteiras completas em cada nivel para garantir o ordenamento numerico."},
            4, 3, 3, 3, 35, "Arvore Trie", "Complexidade"
        },
        {
            "Qual estrategia ajuda a reduzir acessos ao disco quando se trabalha com arquivos grandes?",
            {"Ler byte por byte sem buffer.", "Usar estruturas de indice e buffers para localizar registros com menos leituras.", "Apagar o arquivo antes de cada consulta.", "Converter todos os dados para char sem criterio.", "Ignorar o valor retornado por fopen."},
            2, 3, 3, 3, 36, "Arquivos", "Desempenho"
        }
    };

    // Copia o vetor padrao para o banco usado pela partida.
    for (i = 0; i < 36; i++) {
        cartas[i] = banco_padrao[i];
    }
}

// Inicializa posicao, nome e id de cada jogador.
void inicializar_jogadores(tp_player jogadores[], int quantidade) {
    int i;

    // Define posicao inicial e nomes padrao.
    for (i = 0; i < quantidade; i++) {
        jogadores[i].posicao = 1;
        montar_nome_jogador(jogadores[i].nome, sizeof(jogadores[i].nome), i + 1);
        jogadores[i].id_player = i + 1;
        jogadores[i].historico_casas[0] = 1; // Registra que ele comeca na casa 1.
        jogadores[i].total_casas_visitadas = 1; // Contador inicia em 1 devido a casa inicial.
    }
}

// Registra a passagem do jogador por uma casa no seu historico.
void registrar_historico_posicao(tp_player *jogador, int posicao) {
    // Previne estouro do buffer de historico.
    if (jogador->total_casas_visitadas < 100) {
        jogador->historico_casas[jogador->total_casas_visitadas] = posicao;
        jogador->total_casas_visitadas++;
    }
}

// Preenche a fila de turnos respeitando a ordem de entrada.
void colocar_jogadores_na_fila(tp_player jogadores[4], int quantidade, tp_fila *fila_turnos) {
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
void embaralhar_ids(tp_item indice_cartas[3], int total_elementos) {
    int indice_atual, indice_aleatorio;
    tp_item valor_auxiliar;

    // Itera do último índice até o segundo (total-1 até 1).
    // Não precisa processar o índice 0 pois só resta um elemento.
    for (indice_atual = total_elementos - 1; indice_atual > 0; indice_atual--) {
        // Escolhe aleatoriamente um índice entre 0 e o indice atual (inclusive).
        indice_aleatorio = rand() % (indice_atual + 1);
        
        // Troca o elemento na posição atual com o elemento na posição aleatoria.
        // Usa variável auxiliar para nao perder os dados na memoria.
        valor_auxiliar = indice_cartas[indice_atual];
        indice_cartas[indice_atual] = indice_cartas[indice_aleatorio];
        indice_cartas[indice_aleatorio] = valor_auxiliar;
    }
}

// Filtra ids de cartas por unidade e dificuldade.
// Filtra índices de cartas por unidade e dificuldade.
int coletar_ids_unidade_dificuldade(
    tp_carta banco[36],
    int total,
    int unidade,
    int dificuldade,
    tp_item indice_cartas[3]
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
    tp_carta banco[36],
    int total,
    int unidade,
    int dificuldade
) {
    tp_item indice_cartas[36];
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
    tp_carta banco[36],
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
    tp_carta banco[36],
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
