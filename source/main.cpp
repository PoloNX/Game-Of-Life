#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <iostream>
#include <SDL2/SDL.h>

#include "thread_pool.hpp"
#include "grid.hpp"
#include "events.hpp"
#include "render.hpp"

int main(int argc, char* argv[]) {
    int width = 1080;
    int height = 720;
    int cellSize = 20;

    int offsetX = 0;
    int offsetY = 0;
    bool dragging = false;
    int dragStartX = 0;
    int dragStartY = 0;

    Grid grid(width, height);
    Renderer renderer(width, height, cellSize);
    EventManager eventManager;
    ThreadPool threadPool;
    threadPool.Start();

    if (!renderer.initialize()) {
        return -1;
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(renderer.getWindow(), renderer.getRenderer());
    ImGui_ImplSDLRenderer2_Init(renderer.getRenderer());

    bool running = true;
    bool simulation = false;
    int simulation_tick = 1;

    threadPool.QueueJob([&grid, &simulation, &running, &simulation_tick ] {grid.simulation(simulation, running, simulation_tick); });
	
    while (running) {
        running = eventManager.pollEvents(grid, width, height, cellSize, simulation, offsetX, offsetY, dragging, dragStartX, dragStartY);

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Menu");
            if(ImGui::Button("Simulation")) {
                simulation = !simulation;
            }
            if (ImGui::Button("Clear")) {
                simulation = false;
                grid.clear();
            }
            if (ImGui::Button("Randomize")) {
                simulation = false;
                grid.randomize();
            }
            ImGui::SliderInt("Simulation Tick", &simulation_tick, 1, 1000);
            ImGui::End();
        }
		
        ImGui::Render();

        renderer.render(grid, offsetX, offsetY);

        SDL_Delay(1000/240);
    }


    threadPool.Stop();
    return 0;
}
