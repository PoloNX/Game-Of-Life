#include "Grid.hpp"
#include <iostream>
#include <thread>

Grid::Grid(int width, int height) : width(width), height(height), cells(height, std::vector<Cell>(width)) {}

int Grid::getWidth() const { return width; }

int Grid::getHeight() const { return height; }

int Grid::getCellCount() const { return width * height; }

std::vector<std::vector<Cell>>& Grid::getCells() {
    return cells;
}

const std::vector<std::vector<Cell>>& Grid::getCells() const {
    return cells;
}

std::vector<Cell> Grid::getNeighbors(int x, int y) {
    std::vector<Cell> neighbors;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue; 

            int neighborX = x + i;
            int neighborY = y + j;

            if (neighborX >= 0 && neighborX < width && neighborY >= 0 && neighborY < height) {
                neighbors.push_back(cells[neighborY][neighborX]);
            }
        }
    }

    return neighbors;
}

void Grid::update() {
    std::vector<std::vector<Cell>> updatedCells(height, std::vector<Cell>(width));

    // Iterate through each cell in the grid
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int aliveNeighbors = 0;

            // Calculate alive neighbors
            for (int ni = i - 1; ni <= i + 1; ni++) {
                for (int nj = j - 1; nj <= j + 1; nj++) {
                    // Ensure neighbor indices are within bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width && (ni != i || nj != j)) {
                        if (cells[ni][nj].isAlive()) {
                            aliveNeighbors++;
                        }
                    }
                }
            }

            // Apply game rules
            if (!cells[i][j].isAlive() && aliveNeighbors == 3 ) {
                updatedCells[i][j].setAlive(true);
            }
            else if (cells[i][j].isAlive() && (aliveNeighbors < 2 || aliveNeighbors > 3)) {
                updatedCells[i][j].setAlive(false);
            }
            else {
                updatedCells[i][j].setAlive(cells[i][j].isAlive());
            }
        }
    }

    // Update the grid with the updated states
    cells = updatedCells;
}

void Grid::clear() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cells[i][j].setAlive(false);
		}
	}
}

void Grid::randomize() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			cells[i][j].setAlive(rand() % 2 == 0);
		}
	}
}

void Grid::simulation(bool& active, bool& running, int& simulation_tick) {
    while (running) {
		if(active)
            update();
		std::this_thread::sleep_for(std::chrono::microseconds(1000000/simulation_tick));
	}
}