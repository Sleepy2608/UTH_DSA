#include "maze.h"
#include "raylib.h"
#include <iostream>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Maze Solver - Raylib");

    Maze maze(20, 20);
    maze.GenerateMaze();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            maze = Maze(20, 20); 
            maze.GenerateMaze();
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            maze.Draw(screenWidth, screenHeight);
            
            DrawText("Press R to generate new maze", 10, 10, 20, DARKGRAY);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
