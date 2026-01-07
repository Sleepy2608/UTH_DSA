# Tài liệu Dự án Maze Solver (Giải Mê Cung)

## Tổng quan Dự án
Dự án này là chương trình Giải Mê Cung bằng C++ sử dụng **Raylib** để hiển thị đồ họa. Dự án được thiết kế để minh họa các nguyên tắc Lập trình Hướng đối tượng (OOP), Cấu trúc Dữ liệu và Giải thuật (DFS để sinh mê cung), và cơ bản về phát triển game.

## Trạng thái Dự án

### Các tính năng đã hoàn thành
- [x] **Cấu trúc Dữ liệu Mê cung**: Struct `Cell` và Class `Maze`.
- [x] **Sinh Mê cung**: Thuật toán Tìm kiếm theo chiều sâu (DFS) ngẫu nhiên (Iterative Backtracker).
    - Đảm bảo tạo ra mê cung hoàn hảo (có duy nhất một đường đi giữa hai điểm bất kỳ).
    - Tạo lối vào (Góc trên trái) và lối ra (Góc dưới phải).
- [x] **Hiển thị**:
    - Vẽ các bức tường mê cung bằng Raylib.
    - Viền ngoài được làm dày (5.0f) để dễ nhìn hơn.
    - Tường bên trong có độ dày chuẩn (2.0f).
    - **Màu sắc**: Ô Bắt đầu màu Vàng, ô Đích màu Xanh lá.
- [x] **Vòng lặp Game**:
    - Vòng lặp vô hạn xử lý cập nhật và vẽ.
    - **Điều khiển**: Nhấn phím `R` để tạo lại mê cung mới ngay lập tức.

### Các tính năng đang chờ (Cần làm)
- [ ] **Điều khiển của người chơi (Chế độ 1)**:
    - Cho phép người dùng di chuyển nhân vật/điểm đánh dấu bằng các phím mũi tên.
    - Phát hiện va chạm với tường (không đi xuyên tường).
    - Điều kiện chiến thắng khi đến đích.
- [ ] **Tự động giải (Chế độ 2)**:
    - Cài đặt thuật toán tìm đường (BFS, DFS, hoặc A*) để giải mê cung.
    - Hiển thị đường đi giải pháp trên màn hình.

---

## Tài liệu Mã nguồn

### File: `maze.h` & `maze.cpp`

#### Struct `Cell`
Đại diện cho một ô đơn vị trong lưới mê cung.
- **Biến thành viên**:
    - `int row, col`: Tọa độ của ô trong lưới.
    - `bool visited`: Cờ đánh dấu để theo dõi các ô đã được thăm trong quá trình sinh mê cung.
    - `bool walls[4]`: Mảng đại diện cho 4 bức tường của ô.
        - Chỉ số `0`: Tường Trên (Top)
        - Chỉ số `1`: Tường Phải (Right)
        - Chỉ số `2`: Tường Dưới (Bottom)
        - Chỉ số `3`: Tường Trái (Left)
        - `true` nghĩa là có tường, `false` nghĩa là tường đã mở.

#### Class `Maze`
Quản lý lưới và các hoạt động của mê cung.
- **Biến thành viên**:
    - `int rows`: Số hàng của mê cung.
    - `int cols`: Số cột của mê cung.
    - `std::vector<std::vector<Cell>> grid`: Container 2D chứa tất cả các đối tượng `Cell`.

- **Phương thức**:
    - `Maze(int rows, int cols)`: Hàm khởi tạo (Constructor). Khởi tạo lưới với tất cả các bức tường đều đóng và chưa được thăm.
    - `void GenerateMaze()`:
        - Sử dụng `std::stack` cho thuật toán **DFS Lặp (Iterative DFS)**.
        - Bắt đầu tại (0,0).
        - Ngẫu nhiên thăm các ô lân cận chưa được thăm, loại bỏ tường giữa chúng.
        - Quay lui (Backtrack) khi không còn ô lân cận nào chưa thăm.
        - Mở `walls[0]` tại (0,0) làm Lối vào và `walls[2]` tại (`rows`-1, `cols`-1) làm Lối ra.
    - `void Draw(int screenWidth, int screenHeight)`:
        - Tính toán `cellWidth` (chiều rộng ô) và `cellHeight` (chiều cao ô) dựa trên kích thước màn hình.
        - Duyệt qua lưới và gọi `DrawLineEx` (Raylib) để vẽ các bức tường đang tồn tại.
        - Vẽ ô Bắt đầu màu Vàng và ô Đích màu Xanh lá.
        - Sử dụng nét vẽ dày hơn cho biên giới hạn của mê cung.
    - `void ResetVisited()`: Đặt lại cờ `visited` của tất cả các ô về `false` (hữu ích để đảm bảo thuật toán tìm đường chạy đúng sau này).
    - `Cell* GetUnvisitedNeighbor(Cell* cell)`:
        - Kiểm tra 4 hướng chính xem có ô lân cận nào nằm trong giới hạn và chưa được thăm không.
        - Trả về con trỏ ngẫu nhiên đến một trong các ô đó, hoặc `nullptr` nếu không có.
    - `void RemoveWalls(Cell* current, Cell* next)`:
        - Tính toán hướng đi giữa ô `current` (hiện tại) và ô `next` (tiếp theo).
        - Đặt biến tường tương ứng thành `false` cho cả hai ô.

### File: `main.cpp`

#### Hàm `main()`
Điểm bắt đầu của chương trình.
- **Hằng số**:
    - `screenWidth`, `screenHeight`: Kích thước cửa sổ (đặt là 800x800).
- **Luồng xử lý**:
    1.  `InitWindow(...)`: Khởi tạo ngữ cảnh Raylib.
    2.  `Maze maze(20, 20)`: Tạo một đối tượng mê cung kích thước 20x20.
    3.  `maze.GenerateMaze()`: Sinh cấu trúc mê cung ban đầu.
    4.  `while (!WindowShouldClose())`: Vòng lặp game chính.
        - **Input**: Kiểm tra `IsKeyPressed(KEY_R)` để chạy lại hàm khởi tạo và sinh lại mê cung.
        - **Rendering (Vẽ)**:
            - `BeginDrawing()` / `EndDrawing()`.
            - `ClearBackground(RAYWHITE)`.
            - `maze.Draw(...)`.
            - `DrawText(...)` để hiển thị hướng dẫn.

## Hướng dẫn Build (Biên dịch)
Chạy script batch đã được cung cấp:
```cmd
build.bat
```
Script này sẽ biên dịch `main.cpp` và `maze.cpp` có liên kết với thư viện Raylib.
