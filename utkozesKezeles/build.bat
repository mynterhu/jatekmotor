@echo off
:: Állítsd be az SDL include és lib elérési útját
set SDL_INC=C:\SDL\include
set SDL_LIB=C:\SDL\lib

:: Inicializálja a Visual Studio build környezetét
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64

:: Fordítás és linkelés
cl /EHsc /std:c++17 main.cpp /I"%SDL_INC%" /link /LIBPATH:"%SDL_LIB%" SDL3.lib SDL3_image.lib