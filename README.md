# 🦆 Quack Quiz - Boardgame

Um jogo de tabuleiro interativo e educativo desenvolvido completamente em **Linguagem C** para a disciplina de **Algoritmos e Estruturas de Dados**. O projeto aplica conceitos fundamentais de Estruturas de Dados (Pilhas, Filas e Listas Duplamente Encadeadas) aliando entretenimento e aprendizado!

## 📖 Sobre o Jogo

O jogo possui uma interface rica em terminal (TUI), que utiliza cores ANSI e suporte UTF-8 para uma melhor experiência visual.

Quack Quiz é um jogo de perguntas e respostas onde os jogadores competem para chegar primeiro à casa final do tabuleiro (Casa 30). Cada jogador escolhe um personagem de pato, rola um dado e enfrenta casas especiais e perguntas técnicas sobre programação em C e estruturas de dados.

## 🚀 Funcionalidades

- **Tabuleiro Dinâmico:** Gerado sob demanda utilizando uma Lista Duplamente Encadeada, contendo casas normais, de bônus (avanço), de penalidade (recuo) e casas de perguntas.
- **Perguntas e Níveis:** As cartas de perguntas estão separadas por unidades e dificuldades utilizando um conjunto de Matrizes de **Pilhas** para as cartas, reembaralhando com o algoritmo de *Fisher-Yates* quando necessário.
- **Turnos Justos:** A ordem em que os jogadores jogam é rigorosamente controlada por uma **Fila Circular** estática (FIFO).
- **Exportação e Análise de Dados:** Integração com I/O de arquivos para salvar o histórico completo das respostas das partidas no formato `.csv`.

## 🎮 Como Jogar

1. Escolha o número de jogadores (2 a 4).
2. Cada jogador seleciona seu personagem:
   - Patolino
   - Pato Donald
   - Tio Patinhas
   - Pato
3. Na sua vez, role o dado (1 a 6) e avance no tabuleiro.
4. Se cair em uma **Casa de Pergunta (Amarela)**, responda a uma pergunta técnica. Se acertar, recebe um avanço extra. Se errar, sofre uma penalidade e recua.
5. **Casas de Bônus (Verdes)** avançam sua posição e **Casas de Penalidade (Vermelhas)** recuam sua posição instantaneamente.
6. O primeiro jogador a ultrapassar a casa 30 vence o jogo!

*Atenção: Vários jogadores podem ocupar a mesma casa simultaneamente (não há empurrões!).*

## 🛠️ Como Compilar e Executar

O jogo não possui dependências complexas. Tudo que você precisa é um compilador C (como o GCC/MinGW) e um terminal padrão.

Abra o terminal na pasta raiz do projeto e compile executando:
```bash
gcc -o quack main.c
```

### Executar
```bash
./quack
```

## 📂 Estrutura de Arquivos do Projeto
---
|---|---|
| `main.c` | Ponto de entrada para rodar o jogo pelo Terminal. |
| `menu.h` | Controla o fluxo da partida, interações do terminal, ASCII Art e cores. |
| `dados.h` | Modelos de jogadores e cartas, bem como o banco estático de perguntas. |
| `tabuleiro.h` | Lógica da lista duplamente encadeada que forma a trilha de casas. |
| `pilha.h` | Biblioteca de Pilha estática (LIFO) para as cartas. |
| `fila.h` | Biblioteca de Fila Circular (FIFO) para turnos dos jogadores. |
| `estatisticas.h` | Biblioteca para salvar e exportar dados para `historico_respostas.csv`. |

## 🔧 Estruturas de Dados Utilizadas
---

### Fila (`tp_fila`)
- Controla a ordem dos turnos dos jogadores
- Implementação circular (FIFO)
- Jogadores são colocados na fila pela função `colocar_jogadores_na_fila` (em `dados.h`)
## 🛠️ Como Compilar e Rodar o Jogo

### Pilhas de Perguntas (`tp_pilha`)
- Organizadas por unidade e dificuldade
- Usa embaralhamento estilo Fisher-Yates para evitar repeticao imediata
- Quando uma pilha esgota, o conjunto e reembaralhado
O jogo não possui dependências complexas. Tudo que você precisa é um compilador C (como o GCC/MinGW) e um terminal padrão.

### Tabuleiro Dinamico (`tp_tabuleiro`)
- Lista duplamente encadeada com 30 casas (10 por unidade)
- Permite navegacao para frente e para tras
Pelo terminal na pasta raiz do projeto, execute:
```bash
gcc main.c -o quack_terminal.exe
```
E então inicie o jogo:
```bash
.\quack_terminal.exe
```

### Estruturas Personalizadas
- `tp_player`: Dados do jogador (posição, nome, ID)
- `tp_carta`: Dados das perguntas (pergunta, alternativas em matriz, resposta, avanço, dificuldade)

## 🎯 Sistema de Perguntas

- **36 perguntas** (12 por unidade, distribuidas em 3 niveis de dificuldade) com banco preenchido em vetor de cartas
- **Facil**: avanca 1 casa
- **Media**: avanca 2 casas
- **Dificil**: avanca 3 casas
- A dificuldade e sorteada quando o jogador cai em uma casa de pergunta
- Cada unidade possui suas pilhas independentes por dificuldade
- Perguntas nao se repetem ate a pilha esgotar

### Organização da Carta Sorteada
- `dados.h`: sorteia e devolve a carta com `sortear_carta`
- `menu.h`: exibe a carta e coleta a resposta com `menu_carta_sorteada` e `menu_resposta_carta`
- `menu.h`: valida acerto/erro e retorno de avanço com `fazer_pergunta`
---

## 🦆 Personagens
## 🎲 Regras Básicas do Quack Quiz

1. Patolino
2. Pato Donald
3. Tio Patinhas
4. Pato
- **Objetivo:** Ser o primeiro jogador a passar da última casa do tabuleiro.
- **Movimento:** Os dados são rolados com valores pseudoaleatórios de 1 a 6.
- **Casas de Pergunta (Amarelas):** O jogador deverá responder a uma pergunta técnica sobre algoritmos e estruturas de dados. Se acertar, receberá um avanço extra de casas. Se errar, sofre penalidade e recua.
- **Casas Bônus (Verdes):** Avança instantaneamente algumas casas à frente.
- **Casas de Penalidade (Vermelhas):** Recua instantaneamente o jogador pelo tabuleiro.
- **Empates / Acúmulo de Jogadores:** Múltiplos jogadores podem ocupar a mesma casa.

## 👥 Equipe Quack
---

Desenvolvido para a disciplina de Algoritmos e Estrutura de Dados.
## 📋 Registros Pedagógicos (CSV)
Sempre que uma partida é encerrada ou perguntas são respondidas, o jogo manipula dois arquivos na sua raiz:
- `perguntas.csv`: Exporta um catálogo de todas as perguntas disponíveis no banco de cartas para fácil leitura.
- `historico_respostas.csv`: Anexa permanentemente o ID da carta, resposta escolhida, nome do jogador, unidade e se ele acertou ou errou. Serve como métrica analítica do desempenho dos jogadores!

## 📄 Licença
---

Este projeto está sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.
### Créditos e Autoria
*Desenvolvido como projeto educacional de Estruturas de Dados e Algoritmos.*
