#pragma once

#include <vector>
#include "Cell.hpp"

class Grid {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> cells;

public:
    Grid(int width, int height);
    int getWidth() const;
    int getHeight() const;
    int getCellCount() const;

    void update();
    void clear();
    void randomize();

    void simulation(bool& active, bool& running, int& simulation_tick);

    std::vector<std::vector<Cell>>& getCells(); 
    const std::vector<std::vector<Cell>>& getCells() const; 
    std::vector<Cell> getNeighbors(int x, int y);
};

