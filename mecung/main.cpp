#include "maze.h"
#include "raylib.h"
#include <iostream>
#include <cstdio>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Maze Solver - Raylib (Step-by-Step)");

    Maze maze(10, 10);
    // Không tự động generate - chờ người dùng bấm Enter

    SetTargetFPS(60);

    // Toggle flags để kiểm soát hiển thị path
    bool showDFSPath = false;
    bool showBFSPath = false;

    while (!WindowShouldClose()) {
        // Phím N: Tạo seed mới và reset mê cung
        if (IsKeyPressed(KEY_N)) {
            unsigned int newSeed = (unsigned int)time(nullptr);
            maze.SetSeed(newSeed);
            maze.ResetMaze();
            // Reset toggle khi tạo seed mới
            showDFSPath = false;
            showBFSPath = false;
        }
        
        // Phím ENTER hoặc SPACE: Tiến sang bước tiếp theo
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            int step = maze.GetGenerationStep();
            if (step == 0) {
                // Bước 1: Tạo mê cung bằng DFS
                maze.GenerateMazeStep1();
            } else if (step == 1) {
                // Bước 2: Xoá bớt tường
                maze.GenerateMazeStep2();
            }
            // Nếu đã step 2, không làm gì thêm
        }
        
        // Phím R: Tạo mê cung mới hoàn chỉnh (cả 2 bước) với seed hiện tại
        if (IsKeyPressed(KEY_R)) {
            maze.ResetMaze();
            maze.GenerateMaze();
            // Reset toggle khi tạo maze mới
            showDFSPath = false;
            showBFSPath = false;
        }
        
        // Phím S: Toggle DFS path (màu xanh dương) - chỉ khi đã tạo maze
        if (IsKeyPressed(KEY_S) && maze.GetGenerationStep() >= 1) {
            if (!showDFSPath) {
                maze.SolveMaze();
                showDFSPath = true;
            } else {
                maze.ClearPath();
                showDFSPath = false;
            }
        }
        
        // Phím B: Toggle BFS path (màu đỏ) - chỉ khi đã tạo maze
        if (IsKeyPressed(KEY_B) && maze.GetGenerationStep() >= 1) {
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
            
            // Menu đã bị ẩn - vẫn có thể thao tác bằng phím:
            // ENTER/SPACE: Tiến bước | N: Seed mới | R: Tạo maze đầy đủ
            // S: DFS path | B: BFS path
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
