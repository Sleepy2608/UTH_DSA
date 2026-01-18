#include "maze.h"
#include "raylib.h"
#include <iostream>

int main() {
    int inputIdx = 0;
    do{
        cout << "Nhap kich thuoc me cung (>= 10): ";
        cin >> inputIdx;
    }while(inputIdx < 10);

    const int screenHeight = 600;
    const int screenWidth = (int)(600.0 * inputIdx / inputIdx);

    InitWindow(screenWidth, screenHeight, "Maze Solver - Raylib");

    Maze maze(inputIdx, inputIdx);
    maze.GenerateMaze();

    SetTargetFPS(60);

    // Toggle flags để kiểm soát hiển thị path
    bool showDFSPath = false;
    bool showBFSPath = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            maze = Maze(mazeWidth, mazeHeight); 
            maze.GenerateMaze();
            // Reset toggle khi tạo maze mới
            showDFSPath = false;
            showBFSPath = false;
        }
        
        // Phím S: Toggle DFS path (màu xanh dương)
        if (IsKeyPressed(KEY_S)) {
            if (!showDFSPath) {
                maze.SolveMaze();
                showDFSPath = true;
            } else {
                maze.ClearPath();
                showDFSPath = false;
            }
        }
        
        // Phím B: Toggle BFS path (màu đỏ)
        if (IsKeyPressed(KEY_B)) {
            if (!showBFSPath) {
                maze.SolveMazeBFS();
                showBFSPath = true;
            } else {
                maze.ClearBFSPath();
                showBFSPath = false;
            }
        }
        
        maze.UpdateVisitAnimation();    // Animation quá trình duyệt
        maze.UpdatePathAnimation();
        maze.UpdateBFSPathAnimation();

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            maze.Draw(screenWidth, screenHeight);
            
            // Vẽ các ô đã duyệt (minh hoạ thuật toán)
            if (showDFSPath || showBFSPath) {
                maze.DrawVisitedCells(screenWidth, screenHeight);
            }
            
            // Chỉ vẽ path khi đang bật
            if (showDFSPath) {
                maze.DrawPath(screenWidth, screenHeight);      // DFS - Xanh dương
            }
            if (showBFSPath) {
                maze.DrawBFSPath(screenWidth, screenHeight);   // BFS - Đỏ
            }
            
            DrawText("Press R to generate new maze", 10, 10, 20, DARKGRAY);
            DrawText("Press S to toggle DFS (Blue)", 10, 35, 20, BLUE);
            DrawText("Press B to toggle BFS (Red)", 10, 60, 20, RED);
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
