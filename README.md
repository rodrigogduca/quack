# QUACK QUIZ

Projeto academico da disciplina de Algoritmos e Estruturas de Dados.
O jogo foi desenvolvido em C e usa Fila e Pilha para controlar o fluxo de turnos e historico de posicoes.

## Visao Geral

O jogador escolhe personagem, entra em uma partida com 2 a 4 participantes e avanca em um tabuleiro 5x5.
Em cada turno:
1. O jogador pode abrir o pause ou rolar o dado.
2. Ao rolar, avanca de 1 a 6 casas.
3. Se cair em casa multipla de 3, responde uma carta.
4. Se acertar a carta, recebe bonus de avanco.
5. Vence quem chegar primeiro na casa 25.

## Estrutura do Projeto

- `main.c`: ponto de entrada do programa e loop do menu principal.
- `menu.h`: interface textual (menus, pause, leitura validada de inteiros).
- `funcoes.h`: logica da partida (turnos, dado, cartas, vitoria).
- `dados.h`: structs e dados estaticos (jogadores e cartas).
- `fila.h`: implementacao da fila circular (ordem de jogadas).
- `pilha.h`: implementacao da pilha (historico de posicoes).

## Bibliotecas Nativas Utilizadas

- `<stdio.h>`
	- Entrada e saida no terminal (`printf`, `fgets`, `sscanf`, `fflush`).
- `<stdlib.h>`
	- Funcoes utilitarias e aleatoriedade (`rand`, `srand`).
- `<stdbool.h>`
	- Tipo booleano (`bool`, `true`, `false`).
- `<time.h>`
	- Semente de aleatoriedade com horario atual (`time(NULL)`).

## Bibliotecas do Projeto e Responsabilidades

- `menu.h`
	- Limpa terminal, mostra menus e valida entrada do usuario com intervalo minimo/maximo.
	- Centraliza menus: principal, instrucoes, pausa, quantidade de jogadores e personagens.

- `funcoes.h`
	- Inicializa a partida, prepara jogadores e controla o loop de turnos.
	- Rola dado de 1 a 6 e aplica regra de carta em casas multiplas de 3.
	- Verifica condicao de vitoria apos movimento e apos bonus de carta.

- `dados.h`
	- Define `tp_carta` e `tp_player`.
	- Mantem vetor base de personagens (`player`) e cartas da unidade 1 (`unidade1`).

- `fila.h`
	- Implementa fila circular estatica (`inicializa_fila`, `insere_fila`, `remove_fila`).
	- Usada para garantir ordem justa de turnos.

- `pilha.h`
	- Implementa pilha estatica (`push`, `pop`, `top`).
	- Usada para manter historico de posicoes dos jogadores.

## Fluxo Geral da Partida

1. `main` exibe menu principal.
2. Ao iniciar jogo, `executar_partida` configura jogadores e fila de turnos.
3. Para cada turno:
	 - remove jogador da fila;
	 - mostra opcao de rolar dado ou abrir pause;
	 - move jogador com dado aleatorio;
	 - se casa final for multipla de 3, aplica carta;
	 - reinsere jogador no fim da fila.
4. Ao atingir casa 25, encerra partida e retorna ao menu principal.

## Regras Implementadas

- Quantidade de jogadores: 2 a 4.
- Dado: valor aleatorio entre 1 e 6.
- Carta: aparece apenas em casas multiplas de 3.
- Bonus: acerto de carta avanca casas extras.
- Pause durante turno e durante resposta da carta.
- Vitoria ao chegar na casa final (25).

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

## Observacoes Importantes

- Nao compile arquivos `.h` diretamente.
	- Exemplo incorreto: `gcc funcoes.h -o funcoes`
	- Exemplo correto: `gcc -o quack main.c`
- As unidades 2 e 3 de cartas estao reservadas para expansao futura.

## Padrao de Codigo Aplicado

- Comentarios explicativos em todos os arquivos principais.
- Organizacao por secoes (cabecalho, includes, constantes, funcoes).
- Nomes e mensagens padronizados em portugues sem acentos para evitar problema de codificacao no terminal.