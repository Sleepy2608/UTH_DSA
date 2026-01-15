#include "maze.h"
#include <iostream>
#include <algorithm>

Maze::Maze(int r, int c) : rows(r), cols(c), pathAnimIndex(0), bfsPathAnimIndex(0), animating(false), bfsAnimating(false), animDelay(3), animCounter(0), bfsAnimCounter(0), dfsVisitAnimIndex(0), bfsVisitAnimIndex(0), dfsVisitAnimating(false), bfsVisitAnimating(false), visitAnimCounter(0), currentSeed(0), generationStep(0) {
    for (int i = 0; i < rows; ++i) {
        vector<Cell> row;
        for (int j = 0; j < cols; ++j) {
            row.emplace_back(i, j);
        }
        grid.push_back(row);
    }
    // Khởi tạo seed ngẫu nhiên
    currentSeed = (unsigned int)time(nullptr);
    srand(currentSeed);
}

void Maze::SetSeed(unsigned int seed) {
    currentSeed = seed;
    srand(currentSeed);
    generationStep = 0;
}

void Maze::ResetMaze() {
    // Reset tất cả các ô về trạng thái ban đầu
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].visited = false;
            grid[i][j].inPath = false;
            grid[i][j].walls[0] = true;
            grid[i][j].walls[1] = true;
            grid[i][j].walls[2] = true;
            grid[i][j].walls[3] = true;
        }
    }
    // Reset path và animation
    path.clear();
    bfsPath.clear();
    dfsVisitOrder.clear();
    bfsVisitOrder.clear();
    pathAnimIndex = 0;
    bfsPathAnimIndex = 0;
    animating = false;
    bfsAnimating = false;
    dfsVisitAnimIndex = 0;
    bfsVisitAnimIndex = 0;
    dfsVisitAnimating = false;
    bfsVisitAnimating = false;
    generationStep = 0;
}

void Maze::GenerateMaze() {
    // Reset và set seed trước khi tạo
    srand(currentSeed);
    ResetMaze();
    GenerateMazeStep1();
    GenerateMazeStep2();
}

void Maze::GenerateMazeStep1() {
    // Bước 1: Tạo mê cung bằng DFS
    if (generationStep >= 1) return; // Đã thực hiện rồi
    
    srand(currentSeed); // Đảm bảo seed đúng
    
    stack<Cell*> stack;
    Cell* current = &grid[0][0];
    current->visited = true;
    stack.push(current);

    while (!stack.empty()) {
        current = stack.top();
        Cell* neighbor = GetUnvisitedNeighbor(current);

        if (neighbor) {
            neighbor->visited = true;
            stack.push(neighbor);
            RemoveWalls(current, neighbor);
        } else {
            stack.pop();
        }
    }

    // Mở lối vào và lối ra
    grid[0][0].walls[0] = false;
    grid[rows - 1][cols - 1].walls[2] = false;
    
    generationStep = 1;
    ResetVisited(); // Reset visited để chuẩn bị cho solving
}

void Maze::GenerateMazeStep2() {
    // Bước 2: Xoá bớt tường để tạo nhiều đường đi
    if (generationStep < 1) return; // Chưa chạy bước 1
    if (generationStep >= 2) return; // Đã thực hiện rồi
    
    // Tiếp tục sequence ngẫu nhiên từ seed (sau bước 1)
    int extraWallsToRemove = (rows * cols) / 10;
    for (int i = 0; i < extraWallsToRemove; ++i) {
        int r = rand() % rows;
        int c = rand() % cols;
        int wall = rand() % 4;
        
        if (wall == 0 && r > 0) {
            grid[r][c].walls[0] = false;
            grid[r - 1][c].walls[2] = false;
        } else if (wall == 1 && c < cols - 1) {
            grid[r][c].walls[1] = false;
            grid[r][c + 1].walls[3] = false;
        } else if (wall == 2 && r < rows - 1) {
            grid[r][c].walls[2] = false;
            grid[r + 1][c].walls[0] = false;
        } else if (wall == 3 && c > 0) {
            grid[r][c].walls[3] = false;
            grid[r][c - 1].walls[1] = false;
        }
    }
    
    generationStep = 2;
}

void Maze::Draw(int screenWidth, int screenHeight) {
    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = j * cellWidth;
            float y = i * cellHeight;
            if (i == 0 && j == 0) {
                DrawRectangle(x, y, cellWidth, cellHeight, YELLOW);
            } else if (i == rows - 1 && j == cols - 1) {
                DrawRectangle(x, y, cellWidth, cellHeight, GREEN);
            }
            float thick = 2.0f;
            float boundaryThick = 5.0f;
            if (grid[i][j].walls[0]) {
                DrawLineEx({x, y}, {x + cellWidth, y}, (i == 0) ? boundaryThick : thick, BLACK);
            }
            if (grid[i][j].walls[1]) {
                DrawLineEx({x + cellWidth, y}, {x + cellWidth, y + cellHeight}, (j == cols - 1) ? boundaryThick : thick, BLACK);
            }
            if (grid[i][j].walls[2]) {
                DrawLineEx({x, y + cellHeight}, {x + cellWidth, y + cellHeight}, (i == rows - 1) ? boundaryThick : thick, BLACK);
            }
            if (grid[i][j].walls[3]) {
                DrawLineEx({x, y}, {x, y + cellHeight}, (j == 0) ? boundaryThick : thick, BLACK);
            }
        }
    }
}

Cell* Maze::GetUnvisitedNeighbor(Cell* cell) {
    vector<Cell*> neighbors;
    int r = cell->row;
    int c = cell->col;

    if (r > 0 && !grid[r - 1][c].visited) {
        neighbors.push_back(&grid[r - 1][c]);
    }
    if (c < cols - 1 && !grid[r][c + 1].visited) {
        neighbors.push_back(&grid[r][c + 1]);
    }
    if (r < rows - 1 && !grid[r + 1][c].visited) {
        neighbors.push_back(&grid[r + 1][c]);
    }
    if (c > 0 && !grid[r][c - 1].visited) {
        neighbors.push_back(&grid[r][c - 1]);
    }

    if (!neighbors.empty()) {
        int index = rand() % neighbors.size();
        return neighbors[index];
    }

    return nullptr;
}

void Maze::RemoveWalls(Cell* current, Cell* next) {
    int dx = current->col - next->col;
    int dy = current->row - next->row;

    if (dx == 1) {
        current->walls[3] = false;
        next->walls[1] = false;
    } else if (dx == -1) {
        current->walls[1] = false;
        next->walls[3] = false;
    }

    if (dy == 1) {
        current->walls[0] = false;
        next->walls[2] = false;
    } else if (dy == -1) {
        current->walls[2] = false;
        next->walls[0] = false;
    }
}

void Maze::ResetVisited() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].visited = false;
        }
    }
}

bool Maze::DFS(Cell* cell) {
    cell->visited = true;
    dfsVisitOrder.push_back(cell);  // Ghi lại thứ tự duyệt
    if (cell->row == rows - 1 && cell->col == cols - 1) {
        path.push_back(cell);
        return true;
    }
    int r = cell->row;
    int c = cell->col;
    
    if (!cell->walls[0] && r > 0 && !grid[r - 1][c].visited) {
        if (DFS(&grid[r - 1][c])) {
            path.push_back(cell);
            return true;
        }
    }
    if (!cell->walls[1] && c < cols - 1 && !grid[r][c + 1].visited) {
        if (DFS(&grid[r][c + 1])) {
            path.push_back(cell);
            return true;
        }
    }
    if (!cell->walls[2] && r < rows - 1 && !grid[r + 1][c].visited) {
        if (DFS(&grid[r + 1][c])) {
            path.push_back(cell);
            return true;
        }
    }
    if (!cell->walls[3] && c > 0 && !grid[r][c - 1].visited) {
        if (DFS(&grid[r][c - 1])) {
            path.push_back(cell);
            return true;
        }
    }
    return false;
}

void Maze::SolveMaze() {
    ResetVisited();
    path.clear();
    dfsVisitOrder.clear();  // Xóa thứ tự duyệt cũ
    DFS(&grid[0][0]);
    // Bắt đầu animation duyệt trước
    dfsVisitAnimIndex = 0;
    visitAnimCounter = 0;
    dfsVisitAnimating = true;
    // Animation đường đi sẽ bắt đầu sau khi duyệt xong
    pathAnimIndex = 0;
    animCounter = 0;
    animating = false;  // Chưa animate đường đi, đợi duyệt xong
}

void Maze::DrawPath(int screenWidth, int screenHeight) {
    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;
    float offset = -3.0f; // DFS dịch sang trái/trên
    
    int limit = pathAnimIndex < (int)path.size() ? pathAnimIndex : path.size();
    
    for (int i = 0; i < limit - 1; ++i) {
        Cell* current = path[path.size() - 1 - i];
        Cell* next = path[path.size() - 2 - i];
        
        float x1 = current->col * cellWidth + cellWidth / 2 + offset;
        float y1 = current->row * cellHeight + cellHeight / 2 + offset;
        float x2 = next->col * cellWidth + cellWidth / 2 + offset;
        float y2 = next->row * cellHeight + cellHeight / 2 + offset;
        
        DrawLineEx({x1, y1}, {x2, y2}, 4.0f, BLUE); // DFS vẽ màu xanh dương
    }
    
    if (limit > 0) {
        Cell* lastCell = path[path.size() - limit];
        float x = lastCell->col * cellWidth + cellWidth / 2 + offset;
        float y = lastCell->row * cellHeight + cellHeight / 2 + offset;
        DrawCircle(x, y, 6.0f, DARKBLUE);
    }
}

void Maze::UpdatePathAnimation() {
    if (animating && pathAnimIndex < (int)path.size()) {
        animCounter++;
        if (animCounter >= animDelay) {
            pathAnimIndex++;
            animCounter = 0;
        }
    } else {
        animating = false;
    }
}

void Maze::ClearPath() {
    path.clear();
    dfsVisitOrder.clear();
    pathAnimIndex = 0;
    dfsVisitAnimIndex = 0;
    animating = false;
    dfsVisitAnimating = false;
    ResetVisited();
}

// ==================== BFS Implementation ====================

bool Maze::BFSsolve(Cell* start) {
    ResetVisited();
    // Reset inPath nếu trước đó đã vẽ
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].inPath = false;
        }
    }
    
    queue<Cell*> q;
    map<Cell*, Cell*> parent;
    Cell* end = &grid[rows - 1][cols - 1]; // Xác định ô cuối cùng của mê cung
    
    q.push(start);
    start->visited = true;
    parent[start] = nullptr;
    bfsVisitOrder.push_back(start);  // Ghi lại ô đầu tiên
    
    while (!q.empty()) {
        Cell* current = q.front(); // Lấy phần tử đầu tiên trong hàng đợi
        q.pop(); // Loại bỏ phần tử đầu tiên khỏi hàng đợi
        
        // Khi đã tới đích, truy vết lại đường đi
        if (current == end) {
            Cell* cur = end;
            bfsPath.clear();
            while (cur != nullptr) {
                cur->inPath = true; // Đánh dấu vào inPath
                bfsPath.push_back(cur);
                cur = parent[cur];
            }
            return true;
        }
        
        int r = current->row;
        int c = current->col;
        
        // Lên
        if (!current->walls[0] && r > 0) {
            Cell* next = &grid[r - 1][c];
            if (!next->visited) {
                next->visited = true;
                parent[next] = current;
                q.push(next);
                bfsVisitOrder.push_back(next);  // Ghi lại thứ tự duyệt
            }
        }
        
        // Phải
        if (!current->walls[1] && c < cols - 1) {
            Cell* next = &grid[r][c + 1];
            if (!next->visited) {
                next->visited = true;
                parent[next] = current;
                q.push(next);
                bfsVisitOrder.push_back(next);  // Ghi lại thứ tự duyệt
            }
        }
        
        // Xuống
        if (!current->walls[2] && r < rows - 1) {
            Cell* next = &grid[r + 1][c];
            if (!next->visited) {
                next->visited = true;
                parent[next] = current;
                q.push(next);
                bfsVisitOrder.push_back(next);  // Ghi lại thứ tự duyệt
            }
        }
        
        // Trái
        if (!current->walls[3] && c > 0) {
            Cell* next = &grid[r][c - 1];
            if (!next->visited) {
                next->visited = true;
                parent[next] = current;
                q.push(next);
                bfsVisitOrder.push_back(next);  // Ghi lại thứ tự duyệt
            }
        }
    }
    return false;
}

void Maze::SolveMazeBFS() {
    ResetVisited();
    bfsPath.clear();
    bfsVisitOrder.clear();  // Xóa thứ tự duyệt cũ
    BFSsolve(&grid[0][0]);
    // Bắt đầu animation duyệt trước
    bfsVisitAnimIndex = 0;
    visitAnimCounter = 0;
    bfsVisitAnimating = true;
    // Animation đường đi sẽ bắt đầu sau khi duyệt xong
    bfsPathAnimIndex = 0;
    bfsAnimCounter = 0;
    bfsAnimating = false;  // Chưa animate đường đi, đợi duyệt xong
}

void Maze::DrawBFSPath(int screenWidth, int screenHeight) {
    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;
    float offset = 3.0f; // BFS dịch sang phải/dưới
    
    int limit = bfsPathAnimIndex < (int)bfsPath.size() ? bfsPathAnimIndex : bfsPath.size();
    
    // BFS path là từ end -> start, vẽ từ cuối về đầu
    for (int i = 0; i < limit - 1; ++i) {
        Cell* current = bfsPath[bfsPath.size() - 1 - i];
        Cell* next = bfsPath[bfsPath.size() - 2 - i];
        
        float x1 = current->col * cellWidth + cellWidth / 2 + offset;
        float y1 = current->row * cellHeight + cellHeight / 2 + offset;
        float x2 = next->col * cellWidth + cellWidth / 2 + offset;
        float y2 = next->row * cellHeight + cellHeight / 2 + offset;
        
        DrawLineEx({x1, y1}, {x2, y2}, 4.0f, RED); // BFS vẽ màu đỏ
    }
    
    if (limit > 0) {
        Cell* lastCell = bfsPath[bfsPath.size() - limit];
        float x = lastCell->col * cellWidth + cellWidth / 2 + offset;
        float y = lastCell->row * cellHeight + cellHeight / 2 + offset;
        DrawCircle(x, y, 6.0f, MAROON);
    }
}

void Maze::UpdateBFSPathAnimation() {
    if (bfsAnimating && bfsPathAnimIndex < (int)bfsPath.size()) {
        bfsAnimCounter++;
        if (bfsAnimCounter >= animDelay) {
            bfsPathAnimIndex++;
            bfsAnimCounter = 0;
        }
    } else {
        bfsAnimating = false;
    }
}

void Maze::ClearBFSPath() {
    bfsPath.clear();
    bfsVisitOrder.clear();
    bfsPathAnimIndex = 0;
    bfsVisitAnimIndex = 0;
    bfsAnimating = false;
    bfsVisitAnimating = false;
    // Reset inPath
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].inPath = false;
        }
    }
    ResetVisited();
}

// ==================== Visit Animation ====================

void Maze::DrawVisitedCells(int screenWidth, int screenHeight) {
    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;
    
    // Vẽ các ô đã duyệt DFS (màu vàng nhạt)
    int dfsLimit = dfsVisitAnimIndex < (int)dfsVisitOrder.size() ? dfsVisitAnimIndex : dfsVisitOrder.size();
    for (int i = 0; i < dfsLimit; ++i) {
        Cell* cell = dfsVisitOrder[i];
        // Không vẽ đè lên ô bắt đầu và ô kết thúc
        if ((cell->row == 0 && cell->col == 0) || (cell->row == rows - 1 && cell->col == cols - 1)) continue;
        float x = cell->col * cellWidth;
        float y = cell->row * cellHeight;
        DrawRectangle(x + 2, y + 2, cellWidth - 4, cellHeight - 4, ORANGE);  // DFS dùng màu cam
    }
    
    // Vẽ các ô đã duyệt BFS (màu tím nhạt)
    int bfsLimit = bfsVisitAnimIndex < (int)bfsVisitOrder.size() ? bfsVisitAnimIndex : bfsVisitOrder.size();
    for (int i = 0; i < bfsLimit; ++i) {
        Cell* cell = bfsVisitOrder[i];
        // Không vẽ đè lên ô bắt đầu và ô kết thúc
        if ((cell->row == 0 && cell->col == 0) || (cell->row == rows - 1 && cell->col == cols - 1)) continue;
        float x = cell->col * cellWidth;
        float y = cell->row * cellHeight;
        DrawRectangle(x + 2, y + 2, cellWidth - 4, cellHeight - 4, PINK);  // BFS dùng màu hồng
    }
}

void Maze::UpdateVisitAnimation() {
    // Update DFS visit animation
    if (dfsVisitAnimating && dfsVisitAnimIndex < (int)dfsVisitOrder.size()) {
        visitAnimCounter++;
        if (visitAnimCounter >= animDelay) {
            dfsVisitAnimIndex++;
            visitAnimCounter = 0;
        }
    } else if (dfsVisitAnimating) {
        // Duyệt xong, bắt đầu vẽ đường đi
        dfsVisitAnimating = false;
        animating = true;
    }
    
    // Update BFS visit animation
    if (bfsVisitAnimating && bfsVisitAnimIndex < (int)bfsVisitOrder.size()) {
        visitAnimCounter++;
        if (visitAnimCounter >= animDelay) {
            bfsVisitAnimIndex++;
            visitAnimCounter = 0;
        }
    } else if (bfsVisitAnimating) {
        // Duyệt xong, bắt đầu vẽ đường đi
        bfsVisitAnimating = false;
        bfsAnimating = true;
    }
}
