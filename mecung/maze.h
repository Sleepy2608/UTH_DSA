#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>
#include "raylib.h"

using namespace std;

struct Cell {
    int row;
    int col;
    bool visited;
    bool walls[4];

    Cell(int r, int c) : row(r), col(c), visited(false) {
        walls[0] = true;
        walls[1] = true;
        walls[2] = true;
        walls[3] = true;
    }
};

class Maze {
private:
    int rows;
    int cols;
    vector<vector<Cell>> grid;

public:
    Maze(int rows, int cols);
    void GenerateMaze();
    void ResetVisited();
    void Draw(int screenWidth, int screenHeight);
    
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }
    const vector<vector<Cell>>& GetGrid() const { return grid; }
    
private:
    Cell* GetUnvisitedNeighbor(Cell* cell);
    void RemoveWalls(Cell* current, Cell* next);
};

#endif
