#pragma once

#include <SDL2/SDL.h>
#include "Grid.hpp"

class Renderer {
public:
    Renderer(int width, int height, int& cellSize);
    ~Renderer();

    bool initialize();
    void render(Grid& grid, int offsetX, int offsetY);
    void clear();
    void present();

    SDL_Window* getWindow() { return window; }
    SDL_Renderer* getRenderer() { return renderer; }

private:
    int width;
    int height;
    int& cellSize;
    SDL_Window* window;
    SDL_Renderer* renderer;
};
