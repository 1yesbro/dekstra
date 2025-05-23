#include "raylib.h"
#include "maze.h"
#include "dekstra.h"
#include <iostream>
const int CELL_SIZE = 30;
const int WIDTH = 30;
const int HEIGHT = 30;
const int SCREEN_WIDTH = WIDTH * CELL_SIZE;
const int SCREEN_HEIGHT = HEIGHT * CELL_SIZE;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze with Dijkstra's Algorithm");
    SetTargetFPS(60);

    MazeGenerator generator(WIDTH, HEIGHT);
    generator.generate();
    auto maze = generator.getMaze();
    auto start = generator.getStartPoint();
    auto end = generator.getEndPoint();

    // Debug output for start and end points
    std::cout << "Start point: (" << start.first << "," << start.second << ")" << std::endl;
    std::cout << "End point: (" << end.first << "," << end.second << ")" << std::endl;

    // Validate start and end points
    if (start.first < 0 || start.first >= WIDTH || start.second < 0 || start.second >= HEIGHT) {
        std::cerr << "Error: Invalid start point coordinates" << std::endl;
        return 1;
    }

    if (end.first < 0 || end.first >= WIDTH || end.second < 0 || end.second >= HEIGHT) {
        std::cerr << "Error: Invalid end point coordinates" << std::endl;
        return 1;
    }

    // Debug output for maze cells at start and end positions
    std::cout << "Maze at start: " << maze[start.second][start.first] << std::endl;
    std::cout << "Maze at end: " << maze[end.second][end.first] << std::endl;

    std::pair<int, int> currentPos = start; // Current player position

    dekstra solver(maze);
    auto path = solver.findShortestPath(start, end);
    
    // Check if a valid path was found
    if (path.size() == 0) {
        std::cerr << "Error: No valid path found between start and end points" << std::endl;
    } else {
        std::cout << "Valid path found with " << path.size() << " steps" << std::endl;
    }
    bool showSteps = false;
    bool isFinished = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_G)) {
            showSteps = !showSteps;
            if (showSteps) {
                solver.reset();
                solver.findShortestPath(start, end);
                isFinished = false;
            }
        }

        // Handle spacebar press to move player
        if (IsKeyPressed(KEY_SPACE)) {
            // Validate end point before finding path
            if (end.first < 0 || end.first >= WIDTH || end.second < 0 || end.second >= HEIGHT) {
                std::cout << "Cannot move: end point is invalid" << std::endl;
            } else {
                // Find path from current position to end
                std::cout << "Finding path from (" << currentPos.first << "," << currentPos.second 
                          << ") to (" << end.first << "," << end.second << ")" << std::endl;
                
                auto path = solver.findShortestPath(currentPos, end);
                
                // Move to next position if there is a valid path
                if (path.size() > 1) { // Ensure path has at least current position and next position
                    currentPos = path[1]; // Move to the first step (index 1, since index 0 is current position)
                    
                    // Print debug info
                    std::cout << "Moving to: (" << currentPos.first << "," << currentPos.second 
                              << "), Path size: " << path.size() << std::endl;
                    
                    // Check if reached the end
                    if (currentPos.first == end.first && currentPos.second == end.second) {
                        std::cout << "Reached the end!" << std::endl;
                    }
                } else {
                    std::cout << "No valid path found or already at destination." << std::endl;
                }
            }
            
            // Reset visualization if needed
            if (showSteps) {
                solver.reset();
                solver.findShortestPath(currentPos, end);
                isFinished = false;
            } else {
                // Force recalculation of path for visualization
                solver.reset();
                solver.findShortestPath(currentPos, end);
                isFinished = true;
            }
        }

        if (showSteps && !isFinished) {
            isFinished = !solver.step();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw maze
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                Color cellColor;
                // Properly visualize different cell types
                switch (maze[y][x]) {
                    case '+': // Wall
                        cellColor = BLACK;
                        break;
                    case 'I': // Start point
                        cellColor = Color{220, 255, 220, 255}; // Light green
                        break;
                    case 'O': // End point
                        cellColor = Color{255, 220, 220, 255}; // Light red
                        break;
                    case '-': // Path
                    default:
                        cellColor = WHITE;
                        break;
                }

                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, cellColor);
                DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
            }
        }

        // Draw visited nodes from start
        if (showSteps) {
            auto visited = solver.getVisited();
            for (int y = 0; y < HEIGHT; ++y) {
                for (int x = 0; x < WIDTH; ++x) {
                    if (visited[y][x]) {
                        DrawRectangle(
                            x * CELL_SIZE + CELL_SIZE/4,
                            y * CELL_SIZE + CELL_SIZE/4,
                            CELL_SIZE/2,
                            CELL_SIZE/2,
                            Color{0, 255, 0, 128} // Semi-transparent green
                        );
                    }
                }
            }

            // Draw current nodes
            auto current = solver.getCurrent();
            DrawCircle(
                current.first * CELL_SIZE + CELL_SIZE/2,
                current.second * CELL_SIZE + CELL_SIZE/2,
                CELL_SIZE/3,
                BLUE
            );

            auto currentFromEnd = solver.getCurrentFromEnd();
            DrawCircle(
                currentFromEnd.first * CELL_SIZE + CELL_SIZE/2,
                currentFromEnd.second * CELL_SIZE + CELL_SIZE/2,
                CELL_SIZE/3,
                Color{255, 165, 0, 255} // Orange
            );
        }

        // Draw start and end points with more visible colors
        DrawCircle(
            start.first * CELL_SIZE + CELL_SIZE/2,
            start.second * CELL_SIZE + CELL_SIZE/2,
            CELL_SIZE/3,
            GREEN
        );
        // Draw "S" at start
        DrawText("S", start.first * CELL_SIZE + CELL_SIZE/3, start.second * CELL_SIZE + CELL_SIZE/3, CELL_SIZE/2, BLACK);
        
        DrawCircle(
            end.first * CELL_SIZE + CELL_SIZE/2,
            end.second * CELL_SIZE + CELL_SIZE/2,
            CELL_SIZE/3,
            RED
        );
        // Draw "E" at end
        DrawText("E", end.first * CELL_SIZE + CELL_SIZE/3, end.second * CELL_SIZE + CELL_SIZE/3, CELL_SIZE/2, BLACK);

        // Draw current player position
        // Draw current player position with more visibility
        DrawCircle(
            currentPos.first * CELL_SIZE + CELL_SIZE/2,
            currentPos.second * CELL_SIZE + CELL_SIZE/2,
            CELL_SIZE/2.5, // Slightly larger
            YELLOW
        );
        // Draw "P" at player position
        DrawText("P", currentPos.first * CELL_SIZE + CELL_SIZE/3, currentPos.second * CELL_SIZE + CELL_SIZE/3, CELL_SIZE/2, BLACK);

        // Draw path if finished
        // Draw path if finished or not in step mode
        // Draw path if finished or not in step mode
        if ((isFinished || !showSteps) && 
            end.first >= 0 && end.first < WIDTH && end.second >= 0 && end.second < HEIGHT) {
            // Use current position instead of start for path
            auto path = solver.findShortestPath(currentPos, end);
            // Draw path as a connected line
            if (path.size() > 1) {
                for (size_t i = 0; i < path.size() - 1; i++) {
                    DrawLine(
                        path[i].first * CELL_SIZE + CELL_SIZE/2,
                        path[i].second * CELL_SIZE + CELL_SIZE/2,
                        path[i+1].first * CELL_SIZE + CELL_SIZE/2,
                        path[i+1].second * CELL_SIZE + CELL_SIZE/2,
                        BLUE
                    );
                }
                
                // Also draw circles for each point in the path
                for (const auto& point : path) {
                    if (point != currentPos && point != end) {
                        DrawCircle(
                            point.first * CELL_SIZE + CELL_SIZE/2,
                            point.second * CELL_SIZE + CELL_SIZE/2,
                            CELL_SIZE/4,
                            BLUE
                        );
                    }
                }
            }
        }

        // Draw instructions
        DrawText("Press 'G' to toggle step-by-step visualization", 10, 10, 20, DARKGRAY);
        DrawText("Press SPACE to move one step toward exit", 10, 40, 20, DARKGRAY);
        
        // Draw legend
        DrawText("S: Start", 10, 70, 20, GREEN);
        DrawText("E: End", 10, 100, 20, RED);
        DrawText("P: Player", 10, 130, 20, YELLOW);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
