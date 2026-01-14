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
    
    // Thứ tự duyệt các ô để minh hoạ thuật toán
    vector<Cell*> dfsVisitOrder;   // Thứ tự duyệt DFS
    vector<Cell*> bfsVisitOrder;   // Thứ tự duyệt BFS
    
    // Animation cho đường đi cuối cùng
    int pathAnimIndex;
    int bfsPathAnimIndex;
    bool animating;
    bool bfsAnimating;
    int animDelay;
    int animCounter;
    int bfsAnimCounter;
    
    // Animation cho quá trình duyệt (minh hoạ thuật toán)
    int dfsVisitAnimIndex;         // Index animation duyệt DFS
    int bfsVisitAnimIndex;         // Index animation duyệt BFS
    bool dfsVisitAnimating;        // Đang animate quá trình duyệt DFS
    bool bfsVisitAnimating;        // Đang animate quá trình duyệt BFS
    int visitAnimCounter;

public:
    Maze(int rows, int cols);
    void GenerateMaze();
    void ResetVisited();
    void Draw(int screenWidth, int screenHeight);
    void SolveMaze();           // Giải bằng DFS
    void SolveMazeBFS();         // Giải bằng BFS
    void DrawPath(int screenWidth, int screenHeight);    // Vẽ đường DFS (màu xanh dương)
    void DrawBFSPath(int screenWidth, int screenHeight); // Vẽ đường BFS (màu đỏ)
    void DrawVisitedCells(int screenWidth, int screenHeight);  // Vẽ các ô đã duyệt (màu vàng)
    void ClearPath();
    void ClearBFSPath();
    void UpdatePathAnimation();
    void UpdateBFSPathAnimation();
    void UpdateVisitAnimation();   // Cập nhật animation quá trình duyệt
    bool IsAnimating() const { return animating; }
    bool IsBFSAnimating() const { return bfsAnimating; }
    bool IsVisitAnimating() const { return dfsVisitAnimating || bfsVisitAnimating; }
    bool IsDFSVisitComplete() const { return !dfsVisitAnimating && dfsVisitAnimIndex >= (int)dfsVisitOrder.size(); }
    bool IsBFSVisitComplete() const { return !bfsVisitAnimating && bfsVisitAnimIndex >= (int)bfsVisitOrder.size(); }
    
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
