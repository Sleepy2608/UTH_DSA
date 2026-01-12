#include "maze.h"
#include "raylib.h"
#include <iostream>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Maze Solver - Raylib");

    Maze maze(10, 10);
    maze.GenerateMaze();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            maze = Maze(10, 10); 
            maze.GenerateMaze();
        }
        
        // Phím S: Giải bằng DFS (màu xanh dương)
        if (IsKeyPressed(KEY_S)) {
            maze.SolveMaze();
        }
        
        // Phím B: Giải bằng BFS (màu đỏ)
        if (IsKeyPressed(KEY_B)) {
            maze.SolveMazeBFS();
        }
        
        // Phím C: Xóa tất cả đường đi
        if (IsKeyPressed(KEY_C)) {
            maze.ClearPath();
            maze.ClearBFSPath();
        }
        
        maze.UpdatePathAnimation();
        maze.UpdateBFSPathAnimation();

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            maze.Draw(screenWidth, screenHeight);
            maze.DrawPath(screenWidth, screenHeight);      // DFS - Xanh dương
            maze.DrawBFSPath(screenWidth, screenHeight);   // BFS - Đỏ
            
            DrawText("Press R to generate new maze", 10, 10, 20, DARKGRAY);
            DrawText("Press S to solve (DFS - Blue)", 10, 35, 20, BLUE);
            DrawText("Press B to solve (BFS - Red)", 10, 60, 20, RED);
            DrawText("Press C to clear paths", 10, 85, 20, DARKGRAY);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
