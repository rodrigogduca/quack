# 🦆 QUACK QUIZ

Jogo de tabuleiro educativo desenvolvido em C para a disciplina de **Algoritmos e Estrutura de Dados**.

## 📖 Sobre o Jogo

Quack Quiz é um jogo de perguntas e respostas onde os jogadores competem para chegar primeiro à casa 30 do tabuleiro. Cada jogador escolhe um personagem de pato, rola um dado e enfrenta casas especiais e perguntas sobre programação em C e estruturas de dados.

## 🎮 Como Jogar

1. Escolha o número de jogadores (2-4)
2. Cada jogador seleciona seu personagem
3. Na sua vez, role o dado (1-6) e avance no tabuleiro
4. Se cair em casa de pergunta, responda e receba avancos ou penalidades
5. Casas especiais podem avancar ou recuar sua posicao
6. Primeiro a chegar na casa 30 vence!

## 🏃 Executando

### Compilar
```bash
gcc -o quack main.c
```

### Executar
```bash
./quack
```

## 📁 Estrutura do Projeto

| Arquivo | Descrição |
|---------|-----------|
| `main.c` | Loop principal do jogo |
| `config.h` | Constantes do jogo (tabuleiro, perguntas, penalidades) |
| `dados.h` | Estruturas de dados, banco de perguntas por unidade e embaralhamento |
| `menu.h` | Menus do jogo e fluxo completo da partida |
| `tabuleiro.h` | Tabuleiro dinamico e casas especiais |
| `estatisticas.h` | Arvore de estatisticas e relatorio em disco |
| `pilha.h` | Implementacao de Pilha (LIFO) |
| `fila.h` | Implementacao de Fila (FIFO) |
| `listase.h` | Implementacao da Lista Simplesmente Encadeada |

## 🔧 Estruturas de Dados Utilizadas

### Fila (`tp_fila`)
- Controla a ordem dos turnos dos jogadores
- Implementação circular (FIFO)
- Jogadores são colocados na fila pela função `colocar_jogadores_na_fila` (em `dados.h`)

### Pilhas de Perguntas (`tp_pilha`)
- Organizadas por unidade e dificuldade
- Usa embaralhamento estilo Fisher-Yates para evitar repeticao imediata
- Quando uma pilha esgota, o conjunto e reembaralhado

### Tabuleiro Dinamico (`tp_tabuleiro`)
- Lista duplamente encadeada com 30 casas (10 por unidade)
- Permite navegacao para frente e para tras

### Estatisticas (`tp_no_est`)
- Arvore binaria de busca para registrar visitas, acertos e erros por casa

### Estruturas Personalizadas
- `tp_player`: Dados do jogador (posição, nome, ID)
- `tp_carta`: Dados das perguntas (pergunta, alternativas em matriz, resposta, avanço, dificuldade)

## 🎯 Sistema de Perguntas

- **18 perguntas** (6 por unidade) com 3 niveis de dificuldade
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

## 🦆 Personagens

1. Patolino
2. Pato Donald
3. Tio Patinhas
4. Pato

## 📊 Registro de Estatisticas

- Cada casa registra visitas, acertos e erros
- O relatorio e salvo em `relatorio.txt` ao final da partida

## 👥 Equipe Quack

Desenvolvido para a disciplina de Algoritmos e Estrutura de Dados.

## 📄 Licença

Este projeto está sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.
