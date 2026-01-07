@echo off
g++ main.cpp maze.cpp -o maze_game.exe -IC:/raylib/raylib/src -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm
if %errorlevel% neq 0 (
    echo Compilation Failed!
    pause
    exit /b %errorlevel%
)
echo Compilation Successful! Run maze_game.exe to play.
