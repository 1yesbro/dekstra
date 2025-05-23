// maze.h
#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include "myvector.h"
#include <utility>

class MazeGenerator {
public:
    MazeGenerator(int width, int height);
    void generate();
    const MyVector<MyVector<char>>& getMaze() const;
    std::pair<int, int> getStartPoint() const;
    std::pair<int, int> getEndPoint() const;

private:
    int width, height;
    MyVector<MyVector<char>> maze;
    std::pair<int, int> start, end;

    bool isValid(int x, int y) const;
    void carvePath(int x, int y);
    void setStartEndPoints();
};

#endif