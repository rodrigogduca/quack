# QUACK Boardgame

> Um jogo de tabuleiro educativo em 3D com perguntas e respostas sobre Algoritmos e Estruturas de Dados.

Desenvolvido em **Linguagem C** com a biblioteca grafica **Raylib**.

---

## Sobre o Jogo

Quack e um jogo de perguntas e respostas onde 2 a 4 jogadores competem para chegar primeiro a casa 30 do tabuleiro. Cada jogador escolhe um personagem (pato), rola um dado, avanca pelo tabuleiro em perspectiva 3D e responde perguntas tecnicas sobre programacao em C e estruturas de dados.

O projeto aplica conceitos de **Pilhas**, **Filas Circulares**, **Listas Duplamente Encadeadas** e **Arvores Binarias de Busca**.

---

## Funcionalidades

- Tabuleiro 3D estilo Mario Party com camera ajustavel
- 4 personagens unicos com tokens 3D: Patolino, Pato Donald, Tio Patinhas, Pato
- Mascote pato desenhado com primitivas graficas (2D)
- Sistema de design (design tokens) com tema laranja e modo escuro
- Sistema de componentes reutilizaveis (botoes, cards, badges, toasts, progress bars)
- Animacoes suaves com easing functions (bounce, elastic, sine)
- Particulas e confetes para feedback visual
- Banco de 36 perguntas organizadas por unidade e dificuldade
- Pilhas de perguntas com reembaralhamento Fisher-Yates
- Fila circular (FIFO) para controle de turnos
- Arvore Binaria de Busca para exportacao do historico de respostas para CSV
- Modo de acessibilidade com animacoes reduzidas (Ctrl+R)

---

## Requisitos (Dependencias)

Para compilar o jogo em outra maquina, voce precisa instalar:

1. **GCC** (compilador C)
2. **Raylib** (biblioteca grafica)

### Windows — Instalacao com MSYS2 (recomendado)

O MSYS2 fornece um terminal Linux-like no Windows com gerenciador de pacotes `pacman`.

**Passo a passo:**

1. **Baixe o MSYS2** em https://www.msys2.org/ e instale (padrao: `C:\msys64`)

2. **Abra o terminal UCRT64** (nao o MSYS2 padrão):
   - Iniciar Menu > "UCRT64" ou execute `C:\msys64\ucrt64.exe`

3. **Atualize os pacotes** (rode 2-3 vezes se necessario):
   ```bash
   pacman -Syu
   ```
   Se pedir para fechar o terminal, feche e abra o UCRT64 novamente, depois rode:
   ```bash
   pacman -Su
   ```

4. **Instale o GCC**:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc
   ```

5. **Instale o Raylib**:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-raylib
   ```

6. **Teste a instalacao**:
   ```bash
   gcc --version
   ```
   Deve mostrar algo como `gcc.exe (Rev2, Built by ...) 14.x.x`

---

#### Como abrir o terminal UCRT64

O terminal **UCRT64** e onde voce digita os comandos de compilacao. Para abri-lo:

1. Clique no menu **Iniciar** do Windows
2. Digite **"UCRT64"** na busca
3. Clique no aplicativo **"UCRT64"** (ou "UCRT64.exe")
4. Um terminal com fundo roxo/escuro vai abrir — e la que voce digita os comandos

> **Alternativa:** O atalho tambem fica em `C:\msys64\ucrt64.exe` ou no menu Iniciar > "MSYS2" > "UCRT64".

---

#### Navegar ate a pasta do jogo

No **terminal UCRT64**, digite o comando abaixo (copie e cole, depois pressione Enter):

```bash
cd /c/Users/rodri/Downloads/PROJETOS/Programacao/jogos/quack
```

**Explicacao:** o caminho `/c/Users/...` e a forma do MSYS2 representar a letra `C:\` do Windows.

---

#### Compilar e executar o jogo

Voce tem **3 formas** de compilar:

**Opcao A — build.bat (recomendado, nao precisa do `make`):**

```bash
./build.bat
```

Se aparecer "Compilado com sucesso!", execute o jogo:

```bash
./quack.exe
```

**Opcao B — GCC direto (sem make, sem batch):**

```bash
gcc -o quack.exe main.c interface.c jogador.c tabuleiro.c pilha.c fila.c historico.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm
```

Depois execute:

```bash
./quack.exe
```

**Opcao C — Com Makefile (precisa instalar o `make` primeiro):**

No terminal UCRT64, instale o make:

```bash
pacman -S mingw-w64-ucrt-x86_64-make
```

Depois compile e execute com um unico comando:

```bash
make run
```

**Importante:** Sempre use o terminal **UCRT64** para compilar. O PATH do terminal UCRT64 ja configura automaticamente o GCC e o Raylib.

### Linux (Debian/Ubuntu)

```bash
sudo apt install gcc libraylib-dev
```

### macOS (Homebrew)

```bash
brew install gcc raylib
```

---

## Como Compilar

### Opcao 1: Makefile (recomendado)

O **Makefile** automatiza a compilacao. Voce nao precisa digitar comandos longos.

#### O que e Makefile?

Makefile e um arquivo de configuracao que o programa `make` le para saber como compilar seu projeto. Ele define:
- **CC**: qual compilador usar (`gcc`)
- **CFLAGS**: flags do compilador (`-Wall` = mostra todos os avisos, `-Wextra` = avisos extras, `-O2` = otimizacao)
- **LDFLAGS**: bibliotecas para linkar (`-lraylib -lopengl32 -lgdi32 -lwinmm -lm`)
- **TARGET**: nome do executavel (`quack.exe`)
- **SRCS**: lista dos arquivos .c do projeto
- **Targets** (comandos disponiveis): `all`, `run`, `clean`

#### Comandos do Makefile

Abra o terminal UCRT64, navegue ate a pasta do jogo e use:

```bash
# COMPILAR o jogo (gera quack.exe)
make

# ou (equivalente):
make all

# COMPILAR E EXECUTAR de uma vez
make run

# LIMPAR (apaga o executavel)
make clean
```

O que cada comando faz:
| Comando | Explicacao |
|---------|-----------|
| `make` | Chama o target `all`: compila todos os .c em quack.exe |
| `make run` | Compila (se necessario) e executa `./quack.exe` |
| `make clean` | Apaga o arquivo `quack.exe` |

Se voce editou um arquivo .c, rode `make` novamente que ele recompila so as partes necessarias.

### Opcao 2: build.bat (Windows sem Make)

Se o `make` nao estiver instalado, voce pode usar o script em lotes:

1. Abra o terminal UCRT64
2. Navegue ate a pasta do jogo
3. Digite:
    ```bash
    ./build.bat
    ```
4. Se aparecer "Compilado com sucesso!", execute `quack.exe`

### Opcao 3: Linha de comando manual

```bash
gcc -o quack.exe main.c interface.c jogador.c tabuleiro.c pilha.c fila.c historico.c -lraylib -lopengl32 -lgdi32 -lwinmm -lm
```

---

## Como Executar

```bash
./quack.exe
```

Ou se estiver no Explorer, duplo clique em `quack.exe`.

---

## Como Jogar

1. Abra o jogo e clique em **"NOVO JOGO"**
2. Escolha o numero de jogadores (2 a 4)
3. Cada jogador seleciona seu personagem (Patolino, Donald, Tio Patinhas ou Pato)
4. Na sua vez, clique em **"ROLAR DADO"** para avancar
5. Casas de **pergunta** (laranja no tabuleiro) exibem uma carta com 5 alternativas
6. **Acertar** = avanca casas, **Errar** = volta casas
7. Casas de **bonus** (verde) = avanca 2 casas
8. Casas de **penalidade** (vermelho) = volta 2 casas
9. Primeiro a passar da casa 30 vence!

---

## Controles

| Tecla | Acao |
|-------|------|
| **Setas** | (desabilitado) |
| **Ctrl + R** | Ativa/desativa animacoes reduzidas (modo acessibilidade) |

---

## Personagens

| Personagem | Cor | Token 3D |
|---|---|---|
| **Patolino** | Cinza escuro | Bico laranja |
| **Pato Donald** | Branco | Chapeu de marinheiro azul |
| **Tio Patinhas** | Dourado | Cartola e oculos |
| **Pato** | Marrom | Detalhe verde |

---

## Sistema de Perguntas

- **36 perguntas** no total (12 por unidade)
- **3 unidades** de conteudo: C Basico/Vetores, Structs/Ponteiros/Alocacao, Arvores/Arquivos
- **3 niveis de dificuldade**: Facil (avanca 1), Medio (avanca 2), Dificil (avanca 3)
- A dificuldade e sorteada automaticamente ao cair em uma casa de pergunta
- As perguntas sao organizadas em **9 pilhas** (3 unidades x 3 dificuldades)
- Quando uma pilha acaba, ela e **reembaralhada** (algoritmo Fisher-Yates)
- Respostas sao salvas em `historico_respostas.csv` para analise posterior

---

## Estrutura do Projeto

### Arquivos de codigo (.h e .c)

| Arquivo | Descricao |
|---|---|
| `main.c` | Ponto de entrada do programa |
| `interface.h` / `interface.c` | Interface grafica (telas, painel, mascote, loop principal) |
| `jogador.h` / `jogador.c` | Modelos de dados (jogadores, cartas, banco de perguntas) |
| `tabuleiro.h` / `tabuleiro.c` | Tabuleiro como lista duplamente encadeada |
| `pilha.h` / `pilha.c` | Pilha estatica LIFO (para as cartas) |
| `fila.h` / `fila.c` | Fila circular FIFO (para os turnos) |
| `historico.h` / `historico.c` | Arvore BST para exportacao CSV do historico |
| `tema.h` | Design system (cores, fontes, espacamentos, utilidades) |
| `componentes.h` | Componentes reutilizaveis (botao, card, badge, toast, input, progress bar) |
| `animacao.h` | Animacoes (easing, tween, particulas, confetes) |

### Arquivos de configuracao

| Arquivo | Descricao |
|---|---|
| `Makefile` | Automacao de compilacao (Linux, macOS, MSYS2) |
| `build.bat` | Script de compilacao para Windows |
| `run.bat` | Inicia o jogo com duplo clique |
| `libraylib.dll` | Biblioteca Raylib (necessaria para executar o jogo) |
| `musica.ogg` | Musica de fundo (Futureopolis por Eric Matyas — soundimage.org) |

### Arquivos gerados em tempo de execucao

| Arquivo | Descricao |
|---|---|
| `historico_respostas.csv` | Registro de todas as respostas dos jogadores |
| `perguntas.csv` | Exportacao do banco de perguntas |

---

## Estruturas de Dados Utilizadas

| Estrutura | Arquivo | Uso |
|---|---|---|
| **Pilha** (`tp_pilha`) | `pilha.h/c` | Gerencia as cartas-pergunta (LIFO) |
| **Fila Circular** (`tp_fila`) | `fila.h/c` | Ordem dos turnos dos jogadores (FIFO) |
| **Lista Duplamente Encadeada** (`tp_casa`) | `tabuleiro.h/c` | Tabuleiro com 30 casas encadeadas |
| **Arvore Binaria de Busca** (`tp_no_estatistica`) | `historico.h/c` | Organiza registros de respostas para CSV |

---

## Solucao de Problemas

### "gcc: command not found"

O GCC nao esta instalado ou o PATH nao esta configurado. Instale o MSYS2 e use o terminal UCRT64.

### "fatal error: raylib.h: No such file or directory"

O Raylib nao esta instalado. Rode no UCRT64:
```bash
pacman -S mingw-w64-ucrt-x86_64-raylib
```

### "undefined reference to `__imp_...'" (erros de linkedicao)

Faltam bibliotecas no link. Verifique se os flags `-lraylib -lopengl32 -lgdi32 -lwinmm -lm` estao presentes.

### "32-bit vs 64-bit"

Sempre use o terminal **UCRT64** (nao MINGW32). O MINGW32 e 32-bit e nao funcionara com o Raylib 64-bit.

### "libraylib.dll nao encontrada"

O executavel precisa da `libraylib.dll` na mesma pasta. Copie de `C:\msys64\ucrt64\bin\libraylib.dll` para a pasta do jogo, ou recompile.

---

## Pacote Portatil

Para distribuir o jogo para outro computador **sem MSYS2**:

1. **Compile** o jogo (veja secao "Como Compilar")
2. **Copie a DLL** do Raylib:
   ```bash
   cp /ucrt64/bin/libraylib.dll .
   ```
3. **Verifique** se estes 3 arquivos estao na mesma pasta:
   - `quack.exe` (jogo compilado)
   - `libraylib.dll` (biblioteca Raylib)
   - `run.bat` (atalho para iniciar com duplo clique)
4. **Teste** em outro computador: copie a pasta inteira e execute `run.bat`

O jogo funciona em qualquer Windows 10/11 64-bit sem instalacao adicional.

### "O jogo compila mas nao abre"

Verifique se o `quack.exe` foi gerado na mesma pasta. Execute pelo terminal para ver mensagens de erro:
```bash
./quack.exe
```

---

## Licenca

MIT
