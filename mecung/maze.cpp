#include "maze.h"
#include <iostream>
#include <algorithm>

Maze::Maze(int r, int c) : rows(r), cols(c), pathAnimIndex(0), bfsPathAnimIndex(0), animating(false), bfsAnimating(false), animDelay(3), animCounter(0), bfsAnimCounter(0) {
    for (int i = 0; i < rows; ++i) {
        vector<Cell> row;
        for (int j = 0; j < cols; ++j) {
            row.emplace_back(i, j);
        }
        grid.push_back(row);
    }
    srand(time(nullptr));
}

void Maze::GenerateMaze() {
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

    grid[0][0].walls[0] = false;
    grid[rows - 1][cols - 1].walls[2] = false;
    
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
    DFS(&grid[0][0]);
    pathAnimIndex = 0;
    animCounter = 0;
    animating = true;
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
    pathAnimIndex = 0;
    animating = false;
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
            }
        }
        
        // Phải
        if (!current->walls[1] && c < cols - 1) {
            Cell* next = &grid[r][c + 1];
            if (!next->visited) {
                next->visited = true;
                parent[next] = current;
                q.push(next);
            }
        }
        
        // Xuống
        if (!current->walls[2] && r < rows - 1) {
            Cell* next = &grid[r + 1][c];
            if (!next->visited) {
                next->visited = true;
                parent[next] = current;
                q.push(next);
            }
        }
        
        // Trái
        if (!current->walls[3] && c > 0) {
            Cell* next = &grid[r][c - 1];
            if (!next->visited) {
                next->visited = true;
                parent[next] = current;
                q.push(next);
            }
        }
    }
    return false;
}

void Maze::SolveMazeBFS() {
    ResetVisited();
    bfsPath.clear();
    BFSsolve(&grid[0][0]);
    bfsPathAnimIndex = 0;
    bfsAnimCounter = 0;
    bfsAnimating = true;
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
    bfsPathAnimIndex = 0;
    bfsAnimating = false;
    // Reset inPath
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j].inPath = false;
        }
    }
    ResetVisited();
}
