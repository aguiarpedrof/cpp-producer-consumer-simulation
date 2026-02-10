@echo off

echo ========================================
echo  Compilando Sistema de Frutas
echo ========================================

g++ -std=c++11 -Wall -Wextra -pthread -O2 -o sistema_frutas.exe ^
    main.cpp ^
    Configuracao.cpp ^
    Celeiro.cpp ^
    Deposito.cpp ^
    Caminhao.cpp ^
    Fazendeiro.cpp ^
    Transportador.cpp ^
    Cliente.cpp

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  Compilacao bem-sucedida!
    echo  Execute com: sistema_frutas.exe
    echo ========================================
) else (
    echo.
    echo ========================================
    echo  ERRO na compilacao!
    echo  Verifique se o MinGW esta instalado.
    echo ========================================
)

pause
