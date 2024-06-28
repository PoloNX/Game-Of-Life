#include "events.hpp"

#include <imgui_impl_sdl2.h>

bool EventManager::pollEvents(Grid& grid, int width, int height, int& cellSize, bool simulation, int& offsetX, int& offsetY, bool& dragging, int& dragStartX, int& dragStartY) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (ImGui::GetIO().WantCaptureMouse) {
            continue;
        }

        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                dragging = true;
                dragStartX = event.button.x - offsetX;
                dragStartY = event.button.y - offsetY;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT && !simulation) {
                handleMouseDown(event.button, grid, width, height, cellSize, offsetX, offsetY);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                dragging = false;
            }
            break;
        case SDL_MOUSEMOTION:
            if (dragging) {
                offsetX = event.motion.x - dragStartX;
                offsetY = event.motion.y - dragStartY;
            }
            break;
        case SDL_MOUSEWHEEL:
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            handleMouseWheel(event.wheel, cellSize, offsetX, offsetY, mouseX, mouseY);
            break;
        default:
            break;
        }
    }
    return true;
}




void EventManager::handleMouseDown(SDL_MouseButtonEvent& buttonEvent, Grid& grid, int width, int height, int cellSize, int offsetX, int offsetY) {
    int mouseX = buttonEvent.x - offsetX;
    int mouseY = buttonEvent.y - offsetY;

    int cellX = mouseX / cellSize;
    int cellY = mouseY / cellSize;

    if (cellX >= 0 && cellX < width && cellY >= 0 && cellY < height) {
        grid.getCells()[cellY][cellX].setAlive(!grid.getCells()[cellY][cellX].isAlive());
    }
}


void EventManager::handleMouseWheel(SDL_MouseWheelEvent& wheelEvent, int& cellSize, int& offsetX, int& offsetY, int mouseX, int mouseY) {
    float zoomFactor = 0.1f; // Adjust this factor to make zooming less sensitive
    int newSize = cellSize ;
    if (wheelEvent.y > 0)
        newSize++;
    else if (wheelEvent.y < 0)
        newSize--;
    if (newSize < 1) newSize = 1;

    // Calculate the cell position under the mouse cursor
    int cellX = (mouseX - offsetX) / cellSize;
    int cellY = (mouseY - offsetY) / cellSize;

    // Calculate the new offsetX and offsetY to keep the cell under the cursor
    offsetX = mouseX - cellX * newSize;
    offsetY = mouseY - cellY * newSize;

    cellSize = newSize;
}


