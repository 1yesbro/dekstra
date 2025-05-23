// dekstra.cpp
#include "dekstra.h"
#include "iostream"

dekstra::dekstra(const MyVector<MyVector<char>>& maze)
    : maze(maze), width(maze[0].size()), height(maze.size()), finished(false) {
    // Print maze dimensions for debugging
    std::cout << "Maze dimensions: " << width << "x" << height << std::endl;
    reset();
}

void dekstra::reset() {
    dist = MyVector<MyVector<int>>(height, MyVector<int>(width, std::numeric_limits<int>::max()));
    distFromEnd = MyVector<MyVector<int>>(height, MyVector<int>(width, std::numeric_limits<int>::max()));
    prev = MyVector<MyVector<std::pair<int, int>>>(height, MyVector<std::pair<int, int>>(width, {-1, -1}));
    visited = MyVector<MyVector<bool>>(height, MyVector<bool>(width, false));
    visitedFromEnd = MyVector<MyVector<bool>>(height, MyVector<bool>(width, false));
    pq = std::priority_queue<std::pair<int, std::pair<int, int>>,
                            std::vector<std::pair<int, std::pair<int, int>>>,
                            std::greater<std::pair<int, std::pair<int, int>>>>();
    pqFromEnd = std::priority_queue<std::pair<int, std::pair<int, int>>,
                                   std::vector<std::pair<int, std::pair<int, int>>>,
                                   std::greater<std::pair<int, std::pair<int, int>>>>();
    finished = false;
}

bool dekstra::step() {
    if (pq.empty() || pqFromEnd.empty() || finished) {
        finished = true;
        return false;
    }

    if (!pq.empty()) {
        current = pq.top().second;
        pq.pop();

        if (current.first >= 0 && current.first < width &&
            current.second >= 0 && current.second < height &&
            !visited[current.second][current.first]) {

            visited[current.second][current.first] = true;

            for (auto& neighbor : getNeighbors(current.first, current.second)) {
                if (neighbor.first >= 0 && neighbor.first < width &&
                    neighbor.second >= 0 && neighbor.second < height) {
                    int newDist = dist[current.second][current.first] + 1;
                    if (newDist < dist[neighbor.second][neighbor.first]) {
                        dist[neighbor.second][neighbor.first] = newDist;
                        prev[neighbor.second][neighbor.first] = current;
                        pq.push({newDist, neighbor});
                    }
                }
            }
        }
    }

    if (!pqFromEnd.empty()) {
        currentFromEnd = pqFromEnd.top().second;
        pqFromEnd.pop();

        if (currentFromEnd.first >= 0 && currentFromEnd.first < width &&
            currentFromEnd.second >= 0 && currentFromEnd.second < height &&
            !visitedFromEnd[currentFromEnd.second][currentFromEnd.first]) {

            visitedFromEnd[currentFromEnd.second][currentFromEnd.first] = true;

            for (auto& neighbor : getNeighbors(currentFromEnd.first, currentFromEnd.second)) {
                if (neighbor.first >= 0 && neighbor.first < width &&
                    neighbor.second >= 0 && neighbor.second < height) {
                    int newDist = distFromEnd[currentFromEnd.second][currentFromEnd.first] + 1;
                    if (newDist < distFromEnd[neighbor.second][neighbor.first]) {
                        distFromEnd[neighbor.second][neighbor.first] = newDist;
                        pqFromEnd.push({newDist, neighbor});
                    }
                }
            }
        }
    }

    // Check if paths have met
    if ((current.first >= 0 && current.first < width && current.second >= 0 && current.second < height &&
         visitedFromEnd[current.second][current.first]) ||
        (currentFromEnd.first >= 0 && currentFromEnd.first < width && currentFromEnd.second >= 0 && currentFromEnd.second < height &&
         visited[currentFromEnd.second][currentFromEnd.first])) {
        finished = true;
    }

    return true;
}

MyVector<std::pair<int, int>> dekstra::findShortestPath(const std::pair<int, int>& start, const std::pair<int, int>& end) {
    // Create empty path to return in case of failures
    MyVector<std::pair<int, int>> path;
    
    // Debug info for start and end positions
    std::cout << "Finding path from (" << start.first << "," << start.second << ") to ("
              << end.first << "," << end.second << ")" << std::endl;
    
    // Validate start and end points
    if (start.first < 0 || start.first >= width || 
        start.second < 0 || start.second >= height ||
        end.first < 0 || end.first >= width || 
        end.second < 0 || end.second >= height) {
        std::cerr << "Error: Start or end point is out of bounds" << std::endl;
        return path; // Return empty path if start or end is out of bounds
    }
    
    // Print maze cell values at start and end
    std::cout << "Start cell: " << maze[start.second][start.first] << std::endl;
    std::cout << "End cell: " << maze[end.second][end.first] << std::endl;
    
    // Check if start and end points are valid maze locations
    if (!isValid(start.first, start.second)) {
        std::cerr << "Error: Start position (" << start.first << "," << start.second 
                  << ") is not valid: " << maze[start.second][start.first] << std::endl;
        return path;
    }
    
    if (!isValid(end.first, end.second)) {
        std::cerr << "Error: End position (" << end.first << "," << end.second 
                  << ") is not valid: " << maze[end.second][end.first] << std::endl;
        return path;
    }
    
    reset();
    
    try {
        dist[start.second][start.first] = 0;
        distFromEnd[end.second][end.first] = 0;
        pq.push({0, start});
        pqFromEnd.push({0, end});
        
        // Run algorithm until completion or until queues are empty
        while (!pq.empty() && !pqFromEnd.empty() && !finished) {
            step();
        }
        
        // Only attempt path reconstruction if algorithm finished successfully
        if (finished) {
            std::pair<int, int> meetPoint = current;
            
            // Verify if current point is valid and has been visited
            if (current.first >= 0 && current.first < width && 
                current.second >= 0 && current.second < height && 
                visited[current.second][current.first] && 
                visitedFromEnd[current.second][current.first]) {
                // Valid meeting point found
            } 
            // Check if currentFromEnd is a better meeting point
            else if (currentFromEnd.first >= 0 && currentFromEnd.first < width && 
                     currentFromEnd.second >= 0 && currentFromEnd.second < height && 
                     visited[currentFromEnd.second][currentFromEnd.first] && 
                     visitedFromEnd[currentFromEnd.second][currentFromEnd.first]) {
                meetPoint = currentFromEnd;
            } else {
                // No valid meeting point found, check if we can find one
                bool foundMeetingPoint = false;
                for (int y = 0; y < height && !foundMeetingPoint; y++) {
                    for (int x = 0; x < width && !foundMeetingPoint; x++) {
                        if (visited[y][x] && visitedFromEnd[y][x]) {
                            meetPoint = {x, y};
                            foundMeetingPoint = true;
                        }
                    }
                }
                
                if (!foundMeetingPoint) {
                    return path; // No path exists
                }
            }
            
            // Reconstruct path from meetPoint to start
            MyVector<std::pair<int, int>> pathToStart;
            std::pair<int, int> at = meetPoint;
            
            while (at != std::make_pair(-1, -1)) {
                if (at.first < 0 || at.first >= width || at.second < 0 || at.second >= height) {
                    break; // Safety check to prevent out of bounds access
                }
                pathToStart.push_back(at);
                
                // Check if we've reached the start
                if (at.first == start.first && at.second == start.second) {
                    break;
                }
                
                // Safely get the next point by checking bounds
                if (at.second >= 0 && at.second < prev.size() && 
                    at.first >= 0 && at.first < prev[at.second].size()) {
                    at = prev[at.second][at.first];
                } else {
                    break; // Break if we would access out of bounds
                }
            }
            
            // Need to reverse since we went from meetPoint to start
            std::reverse(pathToStart.begin(), pathToStart.end());
            
            // Add pathToStart to final path
            for (auto& point : pathToStart) {
                path.push_back(point);
            }
            
            // Only add meetPoint to end path if it's not already the end
            if (!(meetPoint.first == end.first && meetPoint.second == end.second)) {
                // Reconstruct path from meetPoint to end
                at = meetPoint;
                bool firstPoint = true;
                
                while (!(at.first == end.first && at.second == end.second)) {
                    if (firstPoint) {
                        firstPoint = false; // Skip the first point since it's already in pathToStart
                        
                        // Find next point toward end
                        int minDist = std::numeric_limits<int>::max();
                        std::pair<int, int> nextPoint = {-1, -1};
                        
                        for (auto& neighbor : getNeighbors(at.first, at.second)) {
                            if (neighbor.first >= 0 && neighbor.first < width && 
                                neighbor.second >= 0 && neighbor.second < height && 
                                visitedFromEnd[neighbor.second][neighbor.first]) {
                                int d = distFromEnd[neighbor.second][neighbor.first];
                                if (d < minDist) {
                                    minDist = d;
                                    nextPoint = neighbor;
                                }
                            }
                        }
                        
                        if (nextPoint.first == -1) break; // No path found
                        at = nextPoint;
                    }
                    
                    if (at.first < 0 || at.first >= width || at.second < 0 || at.second >= height) {
                        break; // Safety check to prevent out of bounds access
                    }
                    
                    path.push_back(at);
                    
                    // If we've reached the end, break
                    if (at.first == end.first && at.second == end.second) {
                        break;
                    }
                    
                    // Find next point toward end
                    int minDist = std::numeric_limits<int>::max();
                    std::pair<int, int> nextPoint = {-1, -1};
                    
                    for (auto& neighbor : getNeighbors(at.first, at.second)) {
                        if (neighbor.first >= 0 && neighbor.first < width && 
                            neighbor.second >= 0 && neighbor.second < height && 
                            visitedFromEnd[neighbor.second][neighbor.first]) {
                            int d = distFromEnd[neighbor.second][neighbor.first];
                            if (d < minDist) {
                                minDist = d;
                                nextPoint = neighbor;
                            }
                        }
                    }
                    
                    if (nextPoint.first == -1) break; // No path found
                    at = nextPoint;
                }
            }
        }
    } catch (const std::exception& e) {
        // Catch any exceptions that might occur
        std::cerr << "Exception caught during path finding: " << e.what() << std::endl;
        return MyVector<std::pair<int, int>>(); // Return empty path
    }
    
    return path;
}

const MyVector<MyVector<bool>>& dekstra::getVisited() const {
    return visited;
}

const std::pair<int, int>& dekstra::getCurrent() const {
    return current;
}

const std::pair<int, int>& dekstra::getCurrentFromEnd() const {
    return currentFromEnd;
}

bool dekstra::isValid(int x, int y) const {
    // Check bounds first
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    
    char cell = maze[y][x];
    bool valid = (cell == '-' || cell == 'I' || cell == 'O');
    
    if (!valid) {
        // Add debug output for invalid cells
        std::cout << "Cell at (" << x << "," << y << ") with value '" << cell << "' is not valid for movement" << std::endl;
    }
    
    return valid;
}

MyVector<std::pair<int, int>> dekstra::getNeighbors(int x, int y) const {
    MyVector<std::pair<int, int>> neighbors;
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};

    // Debug print current cell info
    std::cout << "Getting neighbors for cell (" << x << "," << y << ") with value: ";
    if (x >= 0 && x < width && y >= 0 && y < height) {
        std::cout << maze[y][x] << std::endl;
    } else {
        std::cout << "out of bounds" << std::endl;
    }

    // Special case for start position (I) - check surrounding cells
    if (x >= 0 && x < width && y >= 0 && y < height && maze[y][x] == 'I') {
        // Try to find valid moves from entrance
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                // Check if this is a valid cell to move to
                std::cout << "  Checking neighbor (" << nx << "," << ny << ") with value: " << maze[ny][nx] << std::endl;
                if (maze[ny][nx] == '-') {
                    std::cout << "  Valid neighbor found at (" << nx << "," << ny << ")" << std::endl;
                    neighbors.push_back({nx, ny});
                }
            }
        }
        return neighbors;
    }

    // Special case for end position (O) - also check surrounding cells
    if (x >= 0 && x < width && y >= 0 && y < height && maze[y][x] == 'O') {
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                std::cout << "  Checking end neighbor (" << nx << "," << ny << ") with value: " << maze[ny][nx] << std::endl;
                if (maze[ny][nx] == '-') {
                    std::cout << "  Valid neighbor for end found at (" << nx << "," << ny << ")" << std::endl;
                    neighbors.push_back({nx, ny});
                }
            }
        }
        return neighbors;
    }

    // Normal case - check all four directions
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // Check bounds before accessing maze
        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            std::cout << "  Checking normal neighbor (" << nx << "," << ny << ") with value: " << maze[ny][nx] << std::endl;
        }
        
        // Special case - if neighbor is 'I' or 'O', allow movement
        if (nx >= 0 && nx < width && ny >= 0 && ny < height && 
            (maze[ny][nx] == 'I' || maze[ny][nx] == 'O')) {
            std::cout << "  Found entrance/exit at (" << nx << "," << ny << ")" << std::endl;
            neighbors.push_back({nx, ny});
        }
        // Otherwise use normal isValid check
        else if (isValid(nx, ny)) {
            std::cout << "  Valid neighbor found at (" << nx << "," << ny << ")" << std::endl;
            neighbors.push_back({nx, ny});
        }
    }

    if (neighbors.size() == 0) {
        std::cout << "No valid neighbors found for (" << x << "," << y << ")" << std::endl;
    }

    return neighbors;
}
