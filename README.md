# 🦆 QUACK QUIZ

Jogo de tabuleiro educativo desenvolvido em C para a disciplina de **Algoritmos e Estrutura de Dados**.

## 📖 Sobre o Jogo

Quack Quiz é um jogo de perguntas e respostas onde os jogadores competem para chegar primeiro à casa 20 do tabuleiro. Cada jogador escolhe um personagem de pato e responde perguntas sobre programação em C e estruturas de dados.

## 🎮 Como Jogar

1. Escolha o número de jogadores (2-4)
2. Cada jogador seleciona seu personagem
3. Na sua vez, uma pergunta é sorteada aleatoriamente
4. Acertou? Avança no tabuleiro! Errou? Fica na mesma casa
5. Primeiro a chegar na casa 20 vence!

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
| `dados.h` | Estruturas de dados, banco de cartas, embaralhamento e sorteio de carta |
| `menu.h` | Menus do jogo (principal, personagem, pause, carta sorteada) e fluxo da partida |
| `pilha.h` | Implementação de Pilha (LIFO) |
| `fila.h` | Implementação de Fila (FIFO) |
| `listase.h` | Implementação da Lista Simplesmente Encadeada |

## 🔧 Estruturas de Dados Utilizadas

### Fila (`tp_fila`)
- Controla a ordem dos turnos dos jogadores
- Implementação circular (FIFO)
- Jogadores são colocados na fila pela função `colocar_jogadores_na_fila` (em `dados.h`)

### Pilha de Cartas (`tp_pilha_cartas`)
- Armazena os IDs das cartas disponíveis para sorteio
- Usa embaralhamento estilo Fisher-Yates para evitar repetição imediata
- Quando a pilha acaba, o jogo recarrega e embaralha novamente

### Estruturas Personalizadas
- `tp_player`: Dados do jogador (posição, nome, ID)
- `tp_carta`: Dados das perguntas (pergunta, alternativas em matriz, resposta, avanço, dificuldade)

## 🎯 Sistema de Perguntas

- **12 perguntas** divididas em 3 níveis de dificuldade
- **Dificuldade 1**: Avança 1 casa (4 perguntas)
- **Dificuldade 2**: Avança 2 casas (4 perguntas)
- **Dificuldade 3**: Avança 3 casas (4 perguntas)
- Perguntas não se repetem até todas serem usadas
- Alternativas armazenadas em matriz `char[5][150]`

### Organização da Carta Sorteada
- `dados.h`: sorteia e devolve a carta com `sortear_carta`
- `menu.h`: exibe a carta e coleta a resposta com `menu_carta_sorteada` e `menu_resposta_carta`
- `menu.h`: valida acerto/erro e retorno de avanço com `fazer_pergunta`

## 🦆 Personagens

1. Patolino
2. Pato Donald
3. Tio Patinhas
4. Pato

## 👥 Equipe Quack

Desenvolvido para a disciplina de Algoritmos e Estrutura de Dados.

## 📄 Licença

Este projeto está sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.
