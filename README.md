# QUACK QUIZ

Projeto academico da disciplina de Algoritmos e Estruturas de Dados.
O jogo foi desenvolvido em C e usa Pilha e Fila para controlar historico e ordem de turnos.

## Visao Geral

No QUACK QUIZ, cada jogador escolhe um personagem e disputa uma corrida em tabuleiro 5x5.
No turno, o jogador pode abrir o pause ou rolar o dado.
Quando cai em casa multipla de 3, responde uma carta de pergunta.
Se acertar, recebe bonus de avancar casas.
Vence quem chegar primeiro na casa final (25).

## Estrutura Atual do Projeto

- `main.c`: ponto de entrada, loop do menu principal e encerramento da aplicacao.
- `menu.h`: interface textual (menus, pausa e leitura validada de inteiros).
- `funcoes.h`: logica principal da partida (tabuleiro, turnos, dado, cartas e vitoria).
- `dados.h`: estruturas e dados fixos (personagens e cartas).
- `estruturas.h`: implementacoes de Pilha e Fila em um unico arquivo.

## Bibliotecas Utilizadas

- `<stdio.h>`: entrada e saida no terminal (`printf`, `fgets`, `sscanf`, `fflush`).

Observacao:
O projeto foi ajustado para nao depender de `<stdlib.h>`, `<stdbool.h>` e `<time.h>`.
O dado usa um gerador pseudoaleatorio interno (xorshift32), definido em `funcoes.h`.

## Papel de Cada Arquivo

- `main.c`
  - Inicializa a semente do gerador interno.
  - Exibe o menu principal em loop.
  - Encaminha para instrucoes, partida ou encerramento.

- `menu.h`
  - Mostra menus do jogo (principal, instrucoes, pausa, jogadores e personagem).
  - Valida entradas numericas em intervalo seguro.
  - Centraliza funcoes de UX textual (limpar tela e aguardar ENTER).

- `funcoes.h`
  - Configura jogadores, pilha de historico e fila de turnos.
  - Controla cada turno: dado, movimento, carta e bonus.
  - Verifica condicoes de vitoria e retornos para menu/encerramento.

- `dados.h`
  - Define os tipos `tp_player` e `tp_carta`.
  - Armazena personagens e baralho da unidade 1.
  - Reserva as unidades 2 e 3 para expansao futura.

- `estruturas.h`
  - Implementa Pilha estatica (`push`, `pop`, `top`, etc.).
  - Implementa Fila circular estatica (`insere_fila`, `remove_fila`, etc.).
  - Compartilha o mesmo tipo base (`tp_item`) e mesma capacidade (`MAX`).

## Fluxo de Execucao

1. `main` mostra o menu principal.
2. Ao iniciar jogo, `executar_partida` prepara jogadores e fila de turnos.
3. Em cada turno:
   - remove o jogador da fila;
   - permite rolar dado ou abrir pause;
   - move o jogador no tabuleiro;
   - aplica carta se cair em multiplo de 3;
   - reinsere o jogador no fim da fila.
4. Ao chegar na casa 25, o jogo declara vencedor e retorna ao menu principal.

## Regras Implementadas

- Jogadores: minimo 2 e maximo 4.
- Dado: valores de 1 a 6.
- Carta: aparece apenas em casas multiplas de 3.
- Bonus: aplicado somente em acerto da carta.
- Pause: disponivel antes de rolar dado e durante resposta da carta.
- Vitoria: ao atingir a casa final (25).

## Compilacao e Execucao

### Windows (PowerShell)

```powershell
gcc -Wall -Wextra -pedantic -std=c11 -o quack main.c
.\quack
```

### Linux/macOS

```bash
gcc -Wall -Wextra -pedantic -std=c11 -o quack main.c
./quack
```

## Observacoes

- Nao compile arquivos `.h` diretamente.
- Comando correto para gerar executavel: `gcc -o quack main.c`.
- O codigo usa comentarios explicativos por modulo e por funcao para facilitar manutencao.
