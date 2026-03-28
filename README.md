# QUACK

Trabalho da disciplina Algoritmos e Estrutura de Dados. Tendo como escopo um jogo de tabuleiro, desenvolvido na linguagem de programação C. Utilizando estruturas de Filas e Pilhas.


## Regras do Jogo
O jogo é composto por um tabuleiro de 5x5, onde cada jogador tem uma peça que pode se mover em quatro direções: cima, baixo, esquerda e direita. O objetivo do jogo é chegar ao lado oposto do tabuleiro antes do adversário. Os jogadores podem se mover apenas para casas vazias e não podem passar por cima do adversário. O primeiro jogador a alcançar o lado oposto do tabuleiro vence o jogo.

## Estruturas de Dados Utilizadas

- **Fila**: Utilizada para gerenciar a ordem dos jogadores e suas jogadas. Cada jogador é enfileirado e, após realizar sua jogada, é colocado no final da fila.

- **Pilha**: Utilizada para armazenar as jogadas realizadas por cada jogador, permitindo que eles possam desfazer suas jogadas se necessário. Cada jogador tem sua própria pilha de jogadas, onde cada jogada é empilhada após ser realizada.

- **Tabuleiro**: Representado por uma matriz 5x5, onde cada posição pode estar vazia ou ocupada por um jogador. O tabuleiro é atualizado a cada jogada, refletindo as posições atuais dos jogadores.

- **Jogadores**: Cada jogador é representado por uma estrutura que contém informações como nome, posição atual no tabuleiro e a pilha de jogadas realizadas.

- **Jogada**: Representada por uma estrutura que contém informações sobre a direção do movimento e a posição anterior do jogador, permitindo que as jogadas possam ser desfeitas.

- **Jogo**: Representado por uma estrutura que contém o tabuleiro, a fila de jogadores e as regras do jogo. Esta estrutura é responsável por gerenciar o fluxo do jogo, verificando as condições de vitória e garantindo que as jogadas sejam válidas.

- **Funções**: O jogo inclui funções para inicializar o tabuleiro, adicionar jogadores à fila, realizar jogadas, desfazer jogadas, verificar condições de vitória e exibir o estado atual do jogo. Essas funções são essenciais para garantir a funcionalidade do jogo e a interação entre os jogadores.

- **Interface de Usuário**: O jogo possui uma interface de usuário simples, onde os jogadores podem inserir seus nomes, escolher suas jogadas e visualizar o estado atual do tabuleiro. A interface é projetada para ser intuitiva e fácil de usar, permitindo que os jogadores se concentrem na estratégia do jogo.

- **Validação de Jogadas**: O jogo inclui uma função de validação de jogadas, que verifica se a jogada é válida antes de ser executada. Isso inclui verificar se a posição de destino está dentro dos limites do tabuleiro, se a casa está vazia e se o movimento é permitido de acordo com as regras do jogo.

- **Condições de Vitória**: O jogo verifica as condições de vitória após cada jogada, verificando se um jogador alcançou o lado oposto do tabuleiro. Se um jogador vencer, o jogo é encerrado e o vencedor é anunciado.

- **Desfazer Jogadas**: Os jogadores têm a opção de desfazer suas jogadas, utilizando a pilha de jogadas para retornar à posição anterior. Isso permite que os jogadores corrijam erros ou mudem sua estratégia durante o jogo.

- **Gerenciamento de Jogadores**: O jogo permite que os jogadores sejam adicionados à fila, e a ordem de jogada é gerenciada de forma justa. Após cada jogada, o jogador é colocado no final da fila, garantindo que todos os jogadores tenham a mesma oportunidade de jogar.

- **Atualização do Tabuleiro**: O tabuleiro é atualizado a cada jogada, refletindo as posições atuais dos jogadores. Isso permite que os jogadores visualizem o estado do jogo e planejem suas próximas jogadas de acordo com a posição dos adversários.

- **Estratégia de Jogo**: O jogo incentiva os jogadores a desenvolverem estratégias para alcançar o lado oposto do tabuleiro, considerando as posições dos adversários e as possíveis jogadas. Os jogadores podem tentar bloquear os movimentos dos adversários ou criar caminhos para si mesmos, tornando o jogo mais desafiador e divertido.

- **Multijogador**: O jogo é projetado para ser jogado por dois ou mais jogadores, permitindo que amigos e familiares se divirtam juntos. A interação entre os jogadores é uma parte fundamental do jogo, promovendo a competição saudável e a diversão.

- **Extensibilidade**: O jogo é projetado de forma modular, permitindo que novas funcionalidades sejam adicionadas facilmente no futuro. Isso inclui a possibilidade de adicionar novos tipos de jogadas, diferentes tamanhos de tabuleiro ou até mesmo modos de jogo adicionais, tornando o jogo mais versátil e atraente para uma variedade de jogadores.


## Conclusão
O jogo QUACK é uma implementação divertida e estratégica de um jogo de tabuleiro, utilizando estruturas de dados como filas e pilhas para gerenciar os jogadores e suas jogadas. Com regras simples e uma interface intuitiva, o jogo é acessível para jogadores de todas as idades, promovendo a diversão e a competição saudável entre amigos e familiares. A modularidade do código permite que o jogo seja facilmente expandido no futuro, garantindo que ele continue a ser uma fonte de entretenimento por muito tempo.


## Compilação e Execução
Para compilar o jogo, utilize o seguinte comando no terminal:
```bash
gcc -o quack quack.c
```
Após a compilação, execute o jogo com o comando:
```bash
./quack
```
Certifique-se de ter o ambiente de desenvolvimento C configurado corretamente para compilar e executar o jogo sem problemas. Divirta-se jogando QUACK!