#pragma once

#include <SDL2/SDL.h>

#include "grid.hpp"


class EventManager {
public:
	EventManager() {}
	bool pollEvents(Grid& grid, int width, int height, int& cellSize, bool simulation, int& offsetX, int& offsetY, bool& dragging, int& dragStartX, int& dragStartY);
private:
	void handleMouseDown(SDL_MouseButtonEvent& buttonEvent, Grid& grid, int width, int height, int cellSize, int offsetX, int offsetY);
	void handleMouseWheel(SDL_MouseWheelEvent& wheelEvent, int& cellSize, int& offsetX, int& offsetY, int mouseX, int mouseY);
};