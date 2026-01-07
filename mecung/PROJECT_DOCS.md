# Maze Solver Project Documentation

## Project Overview
This project is a C++ Maze Solver using **Raylib** for visualization. It is designed to demonstrate Object-Oriented Programming (OOP) principles, Data Structures and Algorithms (DFS for generation), and game development basics.

## Project Status

### Implemented Features
- [x] **Maze Data Structures**: `Cell` struct and `Maze` class.
- [x] **Maze Generation**: Randomized Depth-First Search (DFS) algorithm (Iterative Backtracker).
    - Ensures a perfect maze (one unique path between any two points).
    - Generates Start (Top-Left) and End (Bottom-Right) openings.
- [x] **Visualization**:
    - Renders maze walls using Raylib.
    - Thickened outer boundaries (5.0f) for better visibility.
    - Standard inner walls (2.0f).
- [x] **Game Loop**:
    - Infinite loop handling updates and drawing.
    - **Control**: Press `R` to regenerate the maze instantly.

### Pending Features (To-Do)
- [ ] **Player Control (Mode 1)**:
    - Allow user to move a character/marker using arrow keys.
    - Collision detection with walls.
    - Victory condition when reaching the exit.
- [ ] **Auto Solve (Mode 2)**:
    - Implement a pathfinding algorithm (BFS, DFS, or A*) to solve the maze.
    - Visualize the solution path.

---

## Code Documentation

### File: `maze.h` & `maze.cpp`

#### Struct `Cell`
Represents a single unit in the maze grid.
- **Variables**:
    - `int row, col`: Coordinates of the cell in the grid.
    - `bool visited`: Flag used during generation to track visited cells.
    - `bool walls[4]`: Array representing the 4 walls of the cell.
        - Index `0`: Top Wall
        - Index `1`: Right Wall
        - Index `2`: Bottom Wall
        - Index `3`: Left Wall
        - `true` means the wall exists, `false` means it is open.

#### Class `Maze`
Manages the grid and operations.
- **Variables**:
    - `int rows`: Number of rows in the maze.
    - `int cols`: Number of columns in the maze.
    - `std::vector<std::vector<Cell>> grid`: 2D container for all `Cell` objects.

- **Methods**:
    - `Maze(int rows, int cols)`: Constructor. Initializes the grid with all walls intact and unvisited cells.
    - `void GenerateMaze()`:
        - Uses a `std::stack` for the **Iterative DFS** algorithm.
        - Starts at (0,0).
        - Randomly visits unvisited neighbors, removing walls between them.
        - Backtracks when no unvisited neighbors exist.
        - Opens `walls[0]` at (0,0) for Entry and `walls[2]` at (`rows`-1, `cols`-1) for Exit.
    - `void Draw(int screenWidth, int screenHeight)`:
        - Calculates `cellWidth` and `cellHeight` based on screen quantity.
        - Iterates through the grid and calls `DrawLineEx` (Raylib) to draw existing walls.
        - Uses thicker lines for maze boundaries.
    - `void ResetVisited()`: Resets the `visited` flag of all cells (useful ensuring pathfinding algorithms run correctly later).
    - `Cell* GetUnvisitedNeighbor(Cell* cell)`:
        - Checks 4 cardinal directions for neighbors within bounds that have not been visited.
        - Returns a random pointer to one of them, or `nullptr` if none exist.
    - `void RemoveWalls(Cell* current, Cell* next)`:
        - Calculates direction between `current` and `next` cells.
        - Sets the corresponding wall booleans to `false` for both cells.

### File: `main.cpp`

#### Function `main()`
Entry point of the program.
- **Constants**:
    - `screenWidth`, `screenHeight`: Window dimensions (set to 800x800).
- **Flow**:
    1.  `InitWindow(...)`: Starts Raylib context.
    2.  `Maze maze(20, 20)`: Creates a 20x20 maze object.
    3.  `maze.GenerateMaze()`: Generates the initial layout.
    4.  `while (!WindowShouldClose())`: Main game loop.
        - **Input**: Checks `IsKeyPressed(KEY_R)` to re-run constructor and generation.
        - **Rendering**:
            - `BeginDrawing()` / `EndDrawing()`.
            - `ClearBackground(RAYWHITE)`.
            - `maze.Draw(...)`.
            - `DrawText(...)` for UI instructions.

## Build Instructions
Run the provided batch script:
```cmd
build.bat
```
This compiles `main.cpp` and `maze.cpp` with Raylib included.
