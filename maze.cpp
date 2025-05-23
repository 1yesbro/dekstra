// maze.cpp
#include "maze.h"
#include <cstdlib>
#include <ctime>
#include <stack>
#include <utility>
#include <iostream>
MazeGenerator::MazeGenerator(int width, int height)
    : width(width), height(height), maze(height, MyVector<char>(width, '+')),
      start({-1, -1}), end({-1, -1}) {
    std::srand(std::time(0));
}

void MazeGenerator::generate() {
    std::cout << "Generating maze of size " << width << "x" << height << std::endl;
    
    // Initialize maze with walls
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze[y][x] = '+';
        }
    }
    
    // Start carving from a random point
    int startX = 1 + std::rand() % (width - 2);
    int startY = 1 + std::rand() % (height - 2);
    std::cout << "Starting maze generation from (" << startX << "," << startY << ")" << std::endl;
    
    carvePath(startX, startY);
    setStartEndPoints();
    
    // Verify maze after generation
    int pathCount = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (maze[y][x] == '-') {
                pathCount++;
            }
        }
    }
    std::cout << "Maze generation complete. Path cells: " << pathCount << std::endl;
}

void MazeGenerator::carvePath(int x, int y) {
    maze[y][x] = '-';

    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    int order[] = {0, 1, 2, 3};
    for (int i = 0; i < 4; ++i) {
        int j = std::rand() % 4;
        std::swap(order[i], order[j]);
    }

    for (int i = 0; i < 4; ++i) {
        int dir = order[i];
        int nx = x + dx[dir] * 2;
        int ny = y + dy[dir] * 2;

        if (isValid(nx, ny) && maze[ny][nx] == '+') {
            maze[y + dy[dir]][x + dx[dir]] = '-';
            carvePath(nx, ny);
        }
    }
}

void MazeGenerator::setStartEndPoints() {
    std::cout << "Setting start and end points for maze of size " << width << "x" << height << std::endl;
    
    // Try to set start point at top edge first
    bool startFound = false;
    for (int x = 1; x < width - 1; ++x) {
        if (maze[1][x] == '-') {
            start = {x, 0};
            maze[0][x] = 'I';
            std::cout << "Start point set at top edge: (" << start.first << "," << start.second << ")" << std::endl;
            startFound = true;
            break;
        }
    }
    
    // If no start point found at top edge, try other rows
    if (!startFound) {
        std::cout << "No suitable start point found at top edge, trying other rows..." << std::endl;
        
        // Try left edge
        for (int y = 1; y < height - 1; ++y) {
            if (maze[y][1] == '-') {
                start = {0, y};
                maze[y][0] = 'I';
                std::cout << "Start point set at left edge: (" << start.first << "," << start.second << ")" << std::endl;
                startFound = true;
                break;
            }
        }
        
        // If still not found, look for any path cell
        if (!startFound) {
            for (int y = 1; y < height - 1; ++y) {
                for (int x = 1; x < width - 1; ++x) {
                    if (maze[y][x] == '-') {
                        // Create an entrance adjacent to this path
                        if (x > 1) {
                            start = {x-1, y};
                            maze[y][x-1] = 'I';
                        } else {
                            start = {x+1, y};
                            maze[y][x+1] = 'I';
                        }
                        std::cout << "Start point set at internal position: (" << start.first << "," << start.second << ")" << std::endl;
                        startFound = true;
                        break;
                    }
                }
                if (startFound) break;
            }
        }
    }
    
    // Verify that start point was set
    if (start.first == -1 || start.second == -1) {
        std::cerr << "ERROR: Failed to set start point!" << std::endl;
        // Force a start point as last resort
        start = {1, 1};
        maze[1][1] = 'I';
        std::cout << "Forced start point at (1,1)" << std::endl;
    }

    // Try to set end point at bottom edge first
    bool endFound = false;
    
    // Try from right to left at bottom edge
    for (int x = width - 2; x >= 1; --x) {
        if (maze[height-2][x] == '-') {
            end = {x, height-1};
            maze[height-1][x] = 'O';
            std::cout << "End point set at bottom edge: (" << end.first << "," << end.second << ")" << std::endl;
            endFound = true;
            break;
        }
    }
    
    // If not found, try from left to right at bottom edge
    if (!endFound) {
        for (int x = 1; x < width - 1; ++x) {
            if (maze[height-2][x] == '-') {
                end = {x, height-1};
                maze[height-1][x] = 'O';
                std::cout << "End point set at bottom edge (left-to-right): (" << end.first << "," << end.second << ")" << std::endl;
                endFound = true;
                break;
            }
        }
    }
    
    // If still not found, try right edge
    if (!endFound) {
        std::cout << "No suitable end point found at bottom edge, trying right edge..." << std::endl;
        for (int y = height - 2; y >= 1; --y) {
            if (maze[y][width-2] == '-') {
                end = {width-1, y};
                maze[y][width-1] = 'O';
                std::cout << "End point set at right edge: (" << end.first << "," << end.second << ")" << std::endl;
                endFound = true;
                break;
            }
        }
    }
    
    // If still not found, pick any path cell far from start
    if (!endFound) {
        std::cout << "No suitable end point found at edges, looking for any distant path cell..." << std::endl;
        
        // Find a path cell that's far from start
        int maxDistance = 0;
        std::pair<int, int> bestEnd = {-1, -1};
        
        for (int y = height - 2; y >= 1; --y) {
            for (int x = width - 2; x >= 1; --x) {
                if (maze[y][x] == '-') {
                    int distance = std::abs(x - start.first) + std::abs(y - start.second);
                    if (distance > maxDistance) {
                        maxDistance = distance;
                        bestEnd = {x, y};
                    }
                }
            }
        }
        
        if (bestEnd.first != -1) {
            // Make an exit adjacent to this point
            if (bestEnd.second < height-2) {
                end = {bestEnd.first, bestEnd.second+1};
                maze[bestEnd.second+1][bestEnd.first] = 'O';
            } else {
                end = {bestEnd.first, bestEnd.second-1};
                maze[bestEnd.second-1][bestEnd.first] = 'O';
            }
            std::cout << "End point set at internal position: (" << end.first << "," << end.second << ")" << std::endl;
            endFound = true;
        }
    }
    
    // Verify that end point was set
    if (end.first == -1 || end.second == -1) {
        std::cerr << "ERROR: Failed to set end point!" << std::endl;
        // Force an end point as last resort, opposite corner from start
        end = {width-2, height-2};
        maze[height-2][width-2] = 'O';
        std::cout << "Forced end point at (" << end.first << "," << end.second << ")" << std::endl;
    }
    
    // Ensure start and end are different
    if (start.first == end.first && start.second == end.second) {
        std::cerr << "ERROR: Start and end points are the same!" << std::endl;
        // Force them to be different
        if (end.first < width-2) {
            end.first++;
        } else {
            end.first--;
        }
        maze[end.second][end.first] = 'O';
        std::cout << "Adjusted end point to avoid overlap: (" << end.first << "," << end.second << ")" << std::endl;
    }
    
    // Final verification
    std::cout << "Final start point: (" << start.first << "," << start.second << ")" << std::endl;
    std::cout << "Final end point: (" << end.first << "," << end.second << ")" << std::endl;
}

bool MazeGenerator::isValid(int x, int y) const {
    return x > 0 && x < width - 1 && y > 0 && y < height - 1;
}

const MyVector<MyVector<char>>& MazeGenerator::getMaze() const {
    return maze;
}

std::pair<int, int> MazeGenerator::getStartPoint() const {
    return start;
}

std::pair<int, int> MazeGenerator::getEndPoint() const {
    return end;
}