#ifndef TARGET_TEENSY

#include "HardwareSimWindow.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>

bool HardwareSimWindow::initialize() {
    window = SDL_CreateWindow("Hardware Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Failed to create hardware sim window: %s\n", SDL_GetError());
        return false;
    }
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 130");
    return true;
}

void HardwareSimWindow::handleEvent(const SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_WINDOWEVENT && event.window.windowID == SDL_GetWindowID(window)) {
        if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
            closed = true;
        }
    }
}

void HardwareSimWindow::renderFrame() {
    SDL_GL_MakeCurrent(window, gl_context);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hardware Simulation");
    ImGui::Text("(Hardware controls would appear here)");
    ImGui::End();

    ImGui::Render();
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void HardwareSimWindow::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    if (gl_context) SDL_GL_DeleteContext(gl_context);
    if (window) SDL_DestroyWindow(window);
    window = nullptr;
    gl_context = nullptr;
    closed = false;
}


#endif