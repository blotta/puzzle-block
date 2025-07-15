@echo off
for /f "delims=" %%i in ('pkg-config --cflags --libs sdl2') do set DEPFLAGS=%%i

@REM echo %DEPFLAGS%


@echo on

@REM CPP
@REM Debug
g++ -g -o build/game src/main.cpp %DEPFLAGS%

@REM Release
@REM g++ -O2 -o build/game src/main.cpp %DEPFLAGS%


@REM g++ -O2 -o build/game src/main.cpp %DEPFLAGS%
