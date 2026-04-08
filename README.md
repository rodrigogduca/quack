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
| `dados.h` | Cartas, jogadores, pilha de cartas e perguntas |
| `menu.h` | Funções dos menus (principal, personagem, pause) |
| `pilha.h` | Implementação de Pilha (LIFO) |
| `fila.h` | Implementação de Fila (FIFO) |

## 🔧 Estruturas de Dados Utilizadas

### Fila (`tp_fila`)
- Controla a ordem dos turnos dos jogadores
- Implementação circular (FIFO)

### Pilha de Cartas (`tp_pilha_cartas`)
- Armazena as cartas de perguntas por dificuldade
- 3 pilhas separadas: fácil, médio, difícil
- Embaralhamento usando Fisher-Yates

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

## 🦆 Personagens

1. Patolino
2. Pato Donald
3. Tio Patinhas
4. Pato

## 👥 Equipe Quack

Desenvolvido para a disciplina de Algoritmos e Estrutura de Dados.

## 📄 Licença

Este projeto está sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.
