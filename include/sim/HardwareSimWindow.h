#pragma once

#ifndef TARGET_TEENSY

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "sim/SimulatedTeensyHardwareManager.h"

class HardwareSimWindow {
public:
    bool initialize();
    void handleEvent(const SDL_Event& event);
    void renderFrame(SimulatedTeensyHardwareState* h);
    void shutdown();
    bool shouldClose() const { return closed; }

private:
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = nullptr;
    bool closed = false;
};

#endif