# QUACK Boardgame

Um jogo de tabuleiro educativo em 3D com perguntas e respostas sobre Algoritmos e Estruturas de Dados. Desenvolvido em **Linguagem C** com **Raylib**.

## Sobre o Jogo

Quack e um jogo de perguntas e respostas onde 2 a 4 jogadores competem para chegar primeiro a casa 30 do tabuleiro. Cada jogador escolhe um personagem (pato), rola um dado, avanca pelo tabuleiro em perspectiva 3D e responde perguntas tecnicas sobre programacao em C e estruturas de dados.

O projeto aplica conceitos de Pilhas, Filas e Listas Duplamente Encadeadas.

## Funcionalidades

- Tabuleiro 3D estilo Mario Party com camera ajustavel
- 4 personagens unicos com visuais 3D: Patolino, Pato Donald, Tio Patinhas, Pato
- Mascote pato desenhado com primitivas graficas
- Tema laranja e preto
- Banco de 36 perguntas organizadas por unidade e dificuldade
- Pilhas de perguntas com reembaralhamento Fisher-Yates
- Fila circular (FIFO) para controle de turnos
- Exportacao do historico de respostas para CSV

## Personagens

| Personagem | Visual 3D |
|---|---|
| **Patolino** | Preto com bico laranja |
| **Pato Donald** | Branco com chapeu de marinheiro azul |
| **Tio Patinhas** | Dourado com cartola e oculos |
| **Pato** | Marrom com detalhe verde |

## Como Jogar

1. Escolha o numero de jogadores (2 a 4)
2. Cada jogador seleciona seu personagem
3. Na sua vez, clique em "ROLAR DADO" para avancar
4. Casas de pergunta exibem uma carta com 5 alternativas
5. Acertar = avanca, errar = recua
6. Primeiro a passar da casa 30 vence

### Controles da Camera 3D

- **Setas**: movem a camera no plano
- **W/S**: zoom in/out

## Compilar e Executar

### Requisitos

- GCC (MinGW/MSYS2)
- Raylib 5.5+ (`pacman -S mingw-w64-ucrt-x86_64-raylib`)

### Compilar

```bash
gcc -o quack.exe main.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm
```

Ou execute `build_gui.bat`.

### Executar

```bash
quack.exe
```

## Estrutura do Projeto

| Arquivo | Descricao |
|---|---|
| `main.c` | Ponto de entrada |
| `interface.h` | Interface grafica completa (Raylib) |
| `dados.h` | Modelos de jogadores, cartas e banco de perguntas |
| `tabuleiro.h` | Lista duplamente encadeada das 30 casas |
| `pilha.h` | Pilha estatica (LIFO) para as cartas |
| `fila.h` | Fila circular (FIFO) para turnos |
| `estatisticas.h` | Exportacao do historico para CSV |

## Estruturas de Dados

- **Fila (`tp_fila`)**: controla a ordem dos turnos (FIFO)
- **Pilha (`tp_pilha`)**: gerencia as perguntas por unidade/dificuldade (LIFO)
- **Tabuleiro (`tp_tabuleiro`)**: lista duplamente encadeada com 30 casas
- **Arvore (`tp_arvore_estatisticas`)**: organiza registros de respostas para exportacao CSV

## Sistema de Perguntas

- 36 perguntas (12 por unidade, 3 niveis de dificuldade)
- Facil: avanca 1 casa
- Medio: avanca 2 casas
- Dificil: avanca 3 casas
- A dificuldade e sorteada ao cair em uma casa de pergunta
- Perguntas nao se repetem ate a pilha esgotar

## Licenca

MIT
