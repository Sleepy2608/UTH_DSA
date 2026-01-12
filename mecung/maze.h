#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <cstdlib>
#include <ctime>
#include "raylib.h"

using namespace std;

struct Cell {
    int row;
    int col;
    bool visited;
    bool walls[4];
    bool inPath; // Dùng cho BFS

    Cell(int r, int c) : row(r), col(c), visited(false), inPath(false) {
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
    vector<Cell*> path;         // Đường đi DFS
    vector<Cell*> bfsPath;       // Đường đi BFS
    int pathAnimIndex;
    int bfsPathAnimIndex;        // Animation index cho BFS
    bool animating;
    bool bfsAnimating;           // Animation flag cho BFS
    int animDelay;
    int animCounter;
    int bfsAnimCounter;          // Animation counter cho BFS

public:
    Maze(int rows, int cols);
    void GenerateMaze();
    void ResetVisited();
    void Draw(int screenWidth, int screenHeight);
    void SolveMaze();           // Giải bằng DFS
    void SolveMazeBFS();         // Giải bằng BFS
    void DrawPath(int screenWidth, int screenHeight);    // Vẽ đường DFS (màu xanh dương)
    void DrawBFSPath(int screenWidth, int screenHeight); // Vẽ đường BFS (màu đỏ)
    void ClearPath();
    void ClearBFSPath();
    void UpdatePathAnimation();
    void UpdateBFSPathAnimation();
    bool IsAnimating() const { return animating; }
    bool IsBFSAnimating() const { return bfsAnimating; }
    
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }
    const vector<vector<Cell>>& GetGrid() const { return grid; }
    
private:
    Cell* GetUnvisitedNeighbor(Cell* cell);
    void RemoveWalls(Cell* current, Cell* next);
    bool DFS(Cell* cell);
    bool BFSsolve(Cell* start);
};

#endif
