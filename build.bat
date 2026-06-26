@echo off
REM build.bat - Script de compilacao para Windows sem Make
REM
REM Uso: duplo clique ou execute no terminal: build.bat
REM
REM Compila todos os arquivos .c com a biblioteca Raylib.
REM OBS: O executavel precisa do arquivo libraylib.dll na mesma pasta para rodar.
REM      Copie de C:\msys64\ucrt64\bin\libraylib.dll para a pasta do jogo.
REM
REM Bibliotecas linkedas:
REM   -lraylib     : Raylib (graficos, audio, input)
REM   -lopengl32   : OpenGL (renderizacao 3D)
REM   -lgdi32      : GDI (janela, eventos do Windows)
REM   -lwinmm      : Windows Multimedia (som, timer)
REM   -lm          : Math (funcoes matematicas como sin, cos, sqrt)

gcc -o quack.exe ^
    main.c ^
    interface.c ^
    jogador.c ^
    tabuleiro.c ^
    pilha.c ^
    fila.c ^
    historico.c ^
    -lraylib -lopengl32 -lgdi32 -lwinmm -lm

REM Verifica se a compilacao foi bem-sucedida (errorlevel = 0)
if %errorlevel% equ 0 (
    echo Compilado com sucesso! Execute quack.exe
) else (
    echo Erro na compilacao.
)
