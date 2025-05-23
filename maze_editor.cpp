#include "raylib.h"

#define CELL_SIZE 30
#define GRID_WIDTH 20
#define GRID_HEIGHT 15

int main() {
    InitWindow(CELL_SIZE * GRID_WIDTH, CELL_SIZE * GRID_HEIGHT, "Maze Editor");
    SetTargetFPS(60);

    int maze[GRID_HEIGHT][GRID_WIDTH] = {{0}};

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            int gridX = mousePos.x / CELL_SIZE;
            int gridY = mousePos.y / CELL_SIZE;

            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                maze[gridY][gridX] = 1;
            }
        }
        else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            int gridX = mousePos.x / CELL_SIZE;
            int gridY = mousePos.y / CELL_SIZE;

            if (gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT) {
                maze[gridY][gridX] = 0;
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (maze[y][x] == 1) {
                        DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
                    }
                    DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
                }
            }

            DrawText("Left click: add wall", 10, 10, 20, DARKGRAY);
            DrawText("Right click: remove wall", 10, 30, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
