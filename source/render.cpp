#include "render.hpp"
#include <iostream>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

Renderer::Renderer(int width, int height, int& cellSize)
    : width(width), height(height), cellSize(cellSize), window(nullptr), renderer(nullptr) {}

Renderer::~Renderer() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

bool Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Game of Life", 100, 100, 1920, 1080, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        return false;
    }

    //SDL_SetWindowFullscreen(window, true);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return true;
}

void Renderer::render(Grid& grid, int offsetX, int offsetY) {
    int gridWidth = grid.getWidth() * cellSize;
    int gridHeight = grid.getHeight() * cellSize;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            if (grid.getCells()[y][x].isAlive()) {
                SDL_Rect cellRect = { offsetX + x * cellSize, offsetY + y * cellSize, cellSize, cellSize };
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100);

    for (int i = 0; i <= grid.getWidth(); ++i) {
        SDL_RenderDrawLine(renderer, offsetX + i * cellSize, offsetY, offsetX + i * cellSize, offsetY + gridHeight);
    }

    for (int j = 0; j <= grid.getHeight(); ++j) {
        SDL_RenderDrawLine(renderer, offsetX, offsetY + j * cellSize, offsetX + gridWidth, offsetY + j * cellSize);
    }

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(renderer);
}




void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}
