# Makefile - Sistema de compilacao do QUACK Boardgame
# 
# Uso:
#   make        -> compila o jogo (gerando quack.exe)
#   make run    -> compila (se necessario) e executa o jogo
#   make clean  -> remove o executavel
#
# Pre-requisitos:
#   - GCC (MinGW) instalado
#   - Raylib instalado (via MSYS2: pacman -S mingw-w64-ucrt-x86_64-raylib)
#
# Variaveis:
#   CC      = compilador C (gcc)
#   CFLAGS  = flags de compilacao (-Wall = todos warnings, -Wextra = extras, -O2 = otimizacao)
#   LDFLAGS = bibliotecas para linkedicao (raylib, opengl32, gdi32, winmm, math)
#   TARGET  = nome do executavel gerado
#   SRCS    = lista de todos os arquivos .c do projeto
#
# OBS: O executavel precisa do arquivo libraylib.dll na mesma pasta para rodar.
#      Copie de C:\msys64\ucrt64\bin\libraylib.dll para a pasta do jogo.

CC := gcc
CFLAGS := -Wall -Wextra -O2
LDFLAGS := -lraylib -lopengl32 -lgdi32 -lwinmm -lm
TARGET := quack.exe
SRCS := main.c interface.c jogador.c tabuleiro.c pilha.c fila.c historico.c

# Indica que 'all', 'clean' e 'run' sao alvos (targets) virtuais (nao sao arquivos)
.PHONY: all clean run

# Alvo padrao: compila o jogo
all: $(TARGET)

# Regra de compilacao: gera o executavel a partir dos fontes
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Alvo 'run': compila (se necessario) e executa o jogo
run: $(TARGET)
	./$(TARGET)

# Alvo 'clean': remove o executavel
clean:
	rm -f $(TARGET)
