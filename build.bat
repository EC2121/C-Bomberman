@echo off



if exist "bin\bomberman.exe" (
    del "bin\bomberman.exe"
)

clang  -Iapp\include -Iapp\include\SDL_include -o bin\bomberman.exe app\source\*.c -L .\SDL2-2.26.0\lib\x64\ -lSDL2    