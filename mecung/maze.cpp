#include "maze.h"
#include <iostream>
#include <algorithm>

Maze::Maze(int r, int c) : rows(r), cols(c) {
    for (int i = 0; i < rows; ++i) {
        std::vector<Cell> row;
        for (int j = 0; j < cols; ++j) {
            row.emplace_back(i, j);
        }
        grid.push_back(row);
    }
    std::srand(std::time(nullptr));
}

void Maze::GenerateMaze() {
    std::stack<Cell*> stack;
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
}

void Maze::Draw(int screenWidth, int screenHeight) {
    float cellWidth = (float)screenWidth / cols;
    float cellHeight = (float)screenHeight / rows;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = j * cellWidth;
            float y = i * cellHeight;
            
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
    std::vector<Cell*> neighbors;
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
        int index = std::rand() % neighbors.size();
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
