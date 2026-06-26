// jogador.c
// Implementacao dos modelos de dados: banco de 36 perguntas, jogadores e
// sistema de sorteio de cartas por unidade/dificuldade com pilhas.

#include "jogador.h"

// Simula o rolamento de um dado de 6 faces: retorna valor entre 1 e 6
int rolar_dado(void) {
    return (rand() % 6) + 1;
}

// Retorna 1 (Facil), 2 (Medio) ou 3 (Dificil) aleatoriamente
int sortear_dificuldade(void) {
    return (rand() % 3) + 1;
}

// Copia a string origem para destino respeitando o limite 'tamanho'.
// Garante terminacao com '\0' mesmo se a origem for maior que o destino.
void copiar_texto(char destino[], int tamanho, char origem[]) {
    int i = 0;
    if (tamanho <= 0) return;
    while (i < tamanho - 1 && origem[i] != '\0') {
        destino[i] = origem[i];
        i++;
    }
    destino[i] = '\0';
}

// Converte um inteiro nao-negativo para sua representacao em string.
// Exemplo: 42 -> "42", 0 -> "0".
void inteiro_para_texto(int valor, char texto[]) {
    char invertido[12];
    int i = 0;
    int j;
    if (valor == 0) {
        texto[0] = '0';
        texto[1] = '\0';
        return;
    }
    // Converte digito por digito (armazenando invertido)
    while (valor > 0 && i < 11) {
        invertido[i] = (char)('0' + (valor % 10));
        valor /= 10;
        i++;
    }
    // Inverte a string para a ordem correta
    for (j = 0; j < i; j++) {
        texto[j] = invertido[i - 1 - j];
    }
    texto[i] = '\0';
}

// Monta o nome padrao "Jogador N" concatenando o prefixo com o numero.
// Exemplo: indice=1 -> "Jogador 1".
void montar_nome_jogador(char nome_saida[], int tamanho, int indice) {
    char prefixo[] = "Jogador ";
    char numero[12];
    int i = 0;
    int j = 0;
    if (tamanho <= 0) return;
    inteiro_para_texto(indice, numero);
    // Copia o prefixo "Jogador "
    while (i < tamanho - 1 && prefixo[i] != '\0') {
        nome_saida[i] = prefixo[i];
        i++;
    }
    // Concatena o numero
    while (i < tamanho - 1 && numero[j] != '\0') {
        nome_saida[i] = numero[j];
        i++;
        j++;
    }
    nome_saida[i] = '\0';
}

// Preenche o vetor de 36 cartas com o banco de perguntas pre-definido.
// As perguntas cobrem 3 unidades de conteudo:
//   Unidade 1: C Basico, Vetores (12 perguntas)
//   Unidade 2: Structs, Ponteiros, Alocacao Dinamica (12 perguntas)
//   Unidade 3: Arvores, Arquivos (12 perguntas)
// Cada pergunta possui: enunciado, 5 alternativas, resposta correta, tema e subtema.
void montar_banco_cartas(tp_carta cartas[36]) {
    int i;
    tp_carta banco_padrao[36] = {
        {"Qual operador realiza atribuição em C?", {"=", "==", "!=", "<=", "++"}, 1, 1, 1, 1, 1, "C Básico", "Operadores"},
        {"Qual símbolo inicia um comentário de uma linha em C?", {"/*", "//", "#", "<!--", "--"}, 2, 1, 1, 1, 2, "C Básico", "Comentários"},
        {"Qual símbolo termina uma instrução em C?", {";", ",", ":", ".", "?"}, 1, 1, 1, 1, 3, "C Básico", "Sintaxe"},
        {"Qual tipo armazena um caractere único em C?", {"char", "int", "float", "double", "string"}, 1, 1, 1, 1, 4, "C Básico", "Tipos de Dados"},
        {"Qual é o primeiro índice válido de um vetor int v[10]?", {"1", "0", "-1", "10", "Depende do compilador"}, 2, 2, 2, 1, 5, "Vetores", "Índices"},
        {"Qual o resultado da expressão (7 %% 3)?", {"0", "1", "2", "3", "7"}, 2, 2, 2, 1, 6, "C Básico", "Operadores"},
        {"Qual operador lógico representa 'E' em C?", {"&&", "||", "!", "&", "|"}, 1, 2, 2, 1, 7, "C Básico", "Operadores Lógicos"},
        {"Qual função lê uma linha inteira com espaços do teclado?", {"gets", "fgets", "scanf", "printf", "puts"}, 2, 2, 2, 1, 8, "C Básico", "Entrada de Dados"},
        {"O que acontece ao acessar v[10] em int v[10]?", {"Acessa o último elemento", "Acesso fora do limite (erro)", "O compilador corrige para v[9]", "Retorna sempre 0", "O vetor aumenta de tamanho"}, 2, 3, 3, 1, 9, "Vetores", "Limites"},
        {"O que pode ocorrer ao usar printf com variável não inicializada?", {"Erro de compilação", "Valor lixo ou comportamento indefinido", "Sempre imprime zero", "Sempre imprime -1", "O programa termina automaticamente"}, 2, 3, 3, 1, 10, "C Básico", "Variáveis"},
        {"Qual o resultado de 5 / 2 usando inteiros?", {"2", "2.5", "3", "1", "0"}, 1, 3, 3, 1, 11, "C Básico", "Operadores"},
        {"Qual o último índice válido de um vetor int v[10]?", {"9", "10", "-1", "1", "Depende do compilador"}, 1, 3, 3, 1, 12, "Vetores", "Índices"},
        {"Qual operador acessa campo de struct via ponteiro?", {".", "->", "&", "*", "::"}, 2, 1, 1, 2, 13, "Struct", "Ponteiros"},
        {"Qual função aloca memória dinâmica em C?", {"malloc", "printf", "scanf", "sizeof", "free"}, 1, 1, 1, 2, 14, "Alocação Dinâmica", "Funções"},
        {"Qual operador obtém o endereço de uma variável?", {"&", "*", "->", ".", "%"}, 1, 1, 1, 2, 15, "Ponteiros", "Operadores"},
        {"Qual operador dereferencia um ponteiro?", {"*", "&", "->", ".", "++"}, 1, 1, 1, 2, 16, "Ponteiros", "Operadores"},
        {"Um ponteiro armazena:", {"Um valor inteiro fixo", "Endereço de memória", "Uma string", "Um arquivo aberto", "Um vetor de chars"}, 2, 2, 2, 2, 17, "Ponteiros", "Memória"},
        {"Ao passar um ponteiro para função, é possível:", {"Alterar o conteúdo apontado", "Somente ler o valor", "Alterar o código fonte", "Mover o ponteiro para NULL automaticamente", "Converter para float"}, 1, 2, 2, 2, 18, "Ponteiros", "Funções"},
        {"Qual macro representa ponteiro nulo em C?", {"NULL", "NIL", "NONE", "ZERO", "VOID"}, 1, 2, 2, 2, 19, "Ponteiros", "NULL"},
        {"Qual função altera o tamanho de um bloco alocado?", {"realloc", "malloc", "calloc", "free", "sizeof"}, 1, 2, 2, 2, 20, "Alocação Dinâmica", "Funções"},
        {"sizeof(int*) retorna:", {"Tamanho do int", "Tamanho do ponteiro", "Sempre 4", "Sempre 8", "Erro de compilação"}, 2, 3, 3, 2, 21, "Ponteiros", "Memória"},
        {"Após usar malloc, deve-se:", {"Ignorar o retorno", "Verificar NULL e usar free quando não precisar", "Sempre usar scanf", "Reiniciar o programa", "Declarar outra variável global"}, 2, 3, 3, 2, 22, "Alocação Dinâmica", "Free"},
        {"Qual problema ocorre ao esquecer de liberar memória alocada?", {"Vazamento de memória", "Erro de sintaxe", "Conversão automática", "Overflow de inteiro", "Mudança de tipo"}, 1, 3, 3, 2, 23, "Alocação Dinâmica", "Perda de memória"},
        {"O que pode acontecer ao dereferenciar um ponteiro NULL?", {"Falha de execução", "Sempre retorna 0", "Cria memória automaticamente", "Imprime lixo", "Corrige para endereço 1"}, 1, 3, 3, 2, 24, "Ponteiros", "NULL"},
        {"Qual estrutura representa naturalmente relações hierárquicas, como pastas dentro de pastas?", {"Fila", "Pilha", "Árvore", "Vetor simples", "Variável escalar"}, 3, 1, 1, 3, 25, "Árvores", "Conceito"},
        {"Qual é a propriedade fundamental que define uma Árvore Binária de Busca (BST)?", {"Todos os nós da árvore devem\npossuir exatamente dois nós filhos.", "Os nós da subárvore esquerda possuem valores menores que o nó pai,\ne os nós da subárvore direita possuem valores maiores.", "O caminho da raiz até qualquer folha\ndeve ter exatamente o mesmo comprimento.", "Os nós da subárvore esquerda são maiores\nque o nó raiz, e os da direita são menores.", "A árvore deve estar perfeitamente\nbalanceada em todos os seus níveis."}, 2, 1, 1, 3, 26, "Árvore Binária", "Busca"},
        {"Qual é o principal objetivo de se utilizar uma Árvore AVL em vez de uma Árvore Binária de Busca (BST) convencional?", {"Permitir o armazenamento de dados\ndiretamente em arquivos de texto sequenciais.", "Eliminar completamente a necessidade de\nponteiros ou referências na implementação.", "Garantir que as operações de busca, inserção e remoção\nmantenham complexidade de tempo no pior caso de O(log n)\npor meio do balanceamento automático.", "Permitir que cada nó tenha mais de dois\nfilhos para economizar memória RAM.", "Facilitar exclusivamente a ordenação\nlexicográfica de cadeias de caracteres."}, 3, 1, 1, 3, 27, "Árvore AVL", "Balanceamento"},
        {"Na manipulação de arquivos em linguagem C (utilizando a biblioteca stdio.h), qual é a função do ponteiro do tipo FILE*?", {"Alocar memória RAM dinamicamente\npara os novos nós de uma árvore.", "Controlar o fluxo de execução\nsíncrona e concorrência do algoritmo.", "Armazenar o conteúdo completo do arquivo\ndiretamente na memória cache do processador.", "Servir como uma referência/canal para abrir, ler,\nescrever ou navegar dentro de um arquivo específico no disco.", "Criptografar automaticamente os dados\nantes de salvá-los no sistema de arquivos."}, 4, 1, 1, 3, 28, "Arquivos", "Manipulação"},
        {"A estrutura de dados conhecida como Árvore Digital (Trie) é amplamente utilizada para aplicações específicas. Qual é o cenário de uso mais adequado para uma Trie?", {"Armazenamento de registros numéricos\nde ponto flutuante com alta precisão.", "Balanceamento estrito de chaves inteiras\ndispersas em sistemas de paginação.", "Sistemas de busca por prefixos, mecanismos\nde autocompletar texto (autocomplete) e dicionários de strings.", "Gerenciamento de blocos de memória secundária\nem sistemas de arquivos magnéticos.", "Ordenação de vetores numéricos de\ntamanho fixo de forma estável."}, 3, 2, 2, 3, 29, "Árvore Trie", "Prefixos"},
        {"Considere uma Árvore Binária de Busca (BST) inicialmente vazia. Se inserirmos as chaves na seguinte sequência exata: [10, 5, 15, 3, 7], qual será o resultado do percurso em-ordem (in-order) dessa árvore?", {"[3, 5, 7, 10, 15]", "[10, 5, 3, 7, 15]", "[3, 7, 5, 15, 10]", "[15, 10, 7, 5, 3]", "[10, 15, 5, 7, 3]"}, 1, 2, 2, 3, 30, "Árvore Binária", "Percurso"},
        {"Durante a inserção de um elemento em uma Árvore AVL, o balanceamento de um nó foi violado após uma inserção na subárvore direita do filho direito desse nó (caso Direita-Direita). Qual rotação corrige o fator de balanceamento?", {"Rotação Dupla Direita (ou Esquerda-Direita).", "Rotação Simples à Direita.", "Rotação Dupla Esquerda (ou Direita-Esquerda).", "Rotação Simples à Esquerda.", "Nenhuma rotação, apenas a atualização\ndos fatores de carga."}, 4, 2, 2, 3, 31, "Árvore AVL", "Rotação"},
        {"Em um arquivo aberto com fopen no modo \"r\", qual operação é permitida?", {"Somente leitura do conteúdo existente.", "Somente escrita no final do arquivo.", "Criar sempre um arquivo novo.", "Apagar automaticamente o conteúdo anterior.", "Executar comandos do sistema operacional."}, 1, 2, 2, 3, 32, "Arquivos", "Modos"},
        {"No contexto de persistência de dados, qual é a principal vantagem de se implementar índices baseados em estruturas de árvores (em disco) em vez de realizar buscas sequenciais diretamente no arquivo de dados?", {"Redução drástica do número de acessos\nà memória secundária (disco), que é ordens\nde grandeza mais lenta que a memória RAM.", "Eliminação completa do uso de buffers\nde entrada e saída (I/O) no sistema operacional.", "Garantia de que o arquivo final indexado ocupe\nmenos espaço físico no disco rígido que o original.", "Permissão para que múltiplos processos escrevam\nno mesmo byte simultaneamente sem travas de concorrência.", "Transformação em tempo de execução de arquivos\nbinários em arquivos de texto legíveis."}, 1, 3, 3, 3, 33, "Arquivos", "Persistência"},
        {"Considere uma Árvore AVL onde um nó X possui fator de balanceamento igual a -2 (calculado como altura(esquerda) - altura(direita)). Sabe-se que o filho à direita de X possui fator de balanceamento igual a +1. Para rebalancear a árvore a partir do nó X, qual sequência de rotações deve ser efetuada?", {"Uma única rotação simples à direita no nó X.", "Uma rotação simples à esquerda no filho de X,\nseguida por uma rotação simples à direita no nó X.", "Uma rotação simples à direita no filho de X,\nseguida por uma rotação simples à esquerda\nno nó X (Rotação Dupla Esquerda).", "Duas rotações simples consecutivas\nà esquerda diretamente no nó X.", "Uma rotação dupla à direita diretamente no filho de X."}, 3, 3, 3, 3, 34, "Árvore AVL", "Rotação"},
        {"Em relação à eficiência de espaço e tempo de uma Árvore Trie padrão, qual alternativa descreve corretamente uma característica complexa dessa estrutura?", {"O tempo de busca de uma chave de comprimento L\ndepende diretamente do número total de chaves (N)\narmazenadas na árvore, ou seja, O(N * L).", "A remoção de uma palavra em uma Trie exige\nobrigatoriamente a reconstrução completa de\ntoda a árvore a partir do nó raiz.", "Ela consome menos memória do que uma árvore binária\npara chaves numéricas inteiras aleatórias e altamente esparsas.", "O pior caso de espaço pode ser alto:\ncada nó pode reservar ponteiros para todo o alfabeto,\nmesmo quando poucos prefixos são compartilhados.", "Os nós de uma Trie armazenam obrigatoriamente chaves inteiras\ncompletas em cada nível para garantir o ordenamento numérico."}, 4, 3, 3, 3, 35, "Árvore Trie", "Complexidade"},
        {"Qual estratégia ajuda a reduzir acessos ao disco quando se trabalha com arquivos grandes?", {"Ler byte por byte sem buffer.", "Usar estruturas de índice e buffers\npara localizar registros com menos leituras.", "Apagar o arquivo antes de cada consulta.", "Converter todos os dados para char sem critério.", "Ignorar o valor retornado por fopen."}, 2, 3, 3, 3, 36, "Arquivos", "Desempenho"}
    };
    // Copia o banco padrao para o vetor fornecido
    for (i = 0; i < 36; i++) {
        cartas[i] = banco_padrao[i];
    }
}

// Inicializa todos os jogadores na casa 1, com nomes padrao e historico vazio
void inicializar_jogadores(tp_player jogadores[], int quantidade) {
    int i;
    for (i = 0; i < quantidade; i++) {
        jogadores[i].posicao = 1;
        montar_nome_jogador(jogadores[i].nome, sizeof(jogadores[i].nome), i + 1);
        jogadores[i].id_player = i + 1;
        jogadores[i].historico_casas[0] = 1;  // Primeira visita: casa 1
        jogadores[i].total_casas_visitadas = 1;
    }
}

// Adiciona a posicao atual ao historico do jogador (limite de 100 registros)
void registrar_historico_posicao(tp_player *jogador, int posicao) {
    if (jogador->total_casas_visitadas < 100) {
        jogador->historico_casas[jogador->total_casas_visitadas] = posicao;
        jogador->total_casas_visitadas++;
    }
}

// Insere os IDs dos jogadores na fila circular de turnos (ordem sequencial)
void colocar_jogadores_na_fila(tp_player jogadores[4], int quantidade, tp_fila *fila_turnos) {
    int i;
    inicializa_fila(fila_turnos);
    for (i = 0; i < quantidade; i++) {
        insere_fila(fila_turnos, jogadores[i].id_player);
    }
}

// Algoritmo Fisher-Yates (Knuth shuffle) para embaralhar um vetor de IDs
void embaralhar_ids(tp_item indice_cartas[3], int total_elementos) {
    int indice_atual, indice_aleatorio;
    tp_item valor_auxiliar;
    for (indice_atual = total_elementos - 1; indice_atual > 0; indice_atual--) {
        indice_aleatorio = rand() % (indice_atual + 1);
        valor_auxiliar = indice_cartas[indice_atual];
        indice_cartas[indice_atual] = indice_cartas[indice_aleatorio];
        indice_cartas[indice_aleatorio] = valor_auxiliar;
    }
}

// Percorre o banco de cartas e coleta os indices das que correspondem
// a unidade e dificuldade especificadas. Retorna a quantidade encontrada.
int coletar_ids_unidade_dificuldade(tp_carta banco[36], int total, int unidade, int dificuldade, tp_item indice_cartas[3]) {
    int i;
    int quantidade = 0;
    for (i = 0; i < total; i++) {
        if (banco[i].unidade == unidade && banco[i].dificuldade == dificuldade) {
            indice_cartas[quantidade] = i;
            quantidade++;
        }
    }
    return quantidade;
}

// Recarrega uma pilha com as cartas de determinada unidade e dificuldade.
// As cartas sao coletadas, embaralhadas (Fisher-Yates) e inseridas na pilha.
void recarregar_pilha_unidade(tp_pilha *pilha, tp_carta banco[36], int total, int unidade, int dificuldade) {
    tp_item indice_cartas[36];
    int quantidade;
    int i;
    inicializa_pilha(pilha);
    quantidade = coletar_ids_unidade_dificuldade(banco, total, unidade, dificuldade, indice_cartas);
    if (quantidade == 0) return;
    embaralhar_ids(indice_cartas, quantidade);
    for (i = 0; i < quantidade; i++) {
        push(pilha, indice_cartas[i]);
    }
}

// Carrega as 9 pilhas (3 unidades x 3 dificuldades) com as cartas do banco.
// Cada pilha contera os indices das cartas de uma combinacao unidade+dificuldade.
void carregar_perguntas(tp_pilha pilhas[3][3], tp_carta banco[36], int total) {
    int unidade;
    int dificuldade;
    for (unidade = 1; unidade <= 3; unidade++) {
        for (dificuldade = 1; dificuldade <= 3; dificuldade++) {
            recarregar_pilha_unidade(&pilhas[unidade - 1][dificuldade - 1], banco, total, unidade, dificuldade);
        }
    }
}

// Sorteia uma carta da pilha correspondente a unidade x dificuldade.
// Se a pilha estiver vazia, recarrega automaticamente (reembaralha).
// Retorna 1 se conseguiu sortear, 0 em caso de erro.
int sortear_carta(tp_pilha pilhas[3][3], tp_carta banco[36], int total, int unidade, int dificuldade, tp_carta *carta_sorteada) {
    tp_item id_carta;
    tp_pilha *pilha;
    if (unidade < 1 || unidade > 3) return 0;
    if (dificuldade < 1 || dificuldade > 3) return 0;
    pilha = &pilhas[unidade - 1][dificuldade - 1];
    // Recarrega automaticamente se a pilha estiver vazia (ciclo de perguntas)
    if (pilha_vazia(pilha)) {
        recarregar_pilha_unidade(pilha, banco, total, unidade, dificuldade);
    }
    if (!pop(pilha, &id_carta)) return 0;
    if (id_carta < 0 || id_carta >= total) return 0;
    *carta_sorteada = banco[id_carta];
    return 1;
}
