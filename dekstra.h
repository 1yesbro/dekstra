// dekstra.h
#ifndef DEKSTRA_H
#define DEKSTRA_H

#include "myvector.h"
#include <utility>

class dekstra {
public:
    dekstra(const MyVector<MyVector<char>>& maze);
    MyVector<std::pair<int, int>> findShortestPath(const std::pair<int, int>& start, const std::pair<int, int>& end);
    bool step();
    void reset();
    const MyVector<MyVector<bool>>& getVisited() const;
    const std::pair<int, int>& getCurrent() const;
    const std::pair<int, int>& getCurrentFromEnd() const;

private:
    const MyVector<MyVector<char>>& maze;
    int width, height;
    MyVector<MyVector<int>> dist;
    MyVector<MyVector<int>> distFromEnd;
    MyVector<MyVector<std::pair<int, int>>> prev;
    MyVector<MyVector<bool>> visited;
    MyVector<MyVector<bool>> visitedFromEnd;
    std::priority_queue<
        std::pair<int, std::pair<int, int>>,
        std::vector<std::pair<int, std::pair<int, int>>>,
        std::greater<std::pair<int, std::pair<int, int>>>
    > pq;
    std::priority_queue<
        std::pair<int, std::pair<int, int>>,
        std::vector<std::pair<int, std::pair<int, int>>>,
        std::greater<std::pair<int, std::pair<int, int>>>
    > pqFromEnd;
    std::pair<int, int> current;
    std::pair<int, int> currentFromEnd;
    bool finished;

    bool isValid(int x, int y) const;
    MyVector<std::pair<int, int>> getNeighbors(int x, int y) const;
};

#endif