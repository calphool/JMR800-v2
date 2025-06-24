#ifndef TARGET_TEENSY

#include "HardwareSimWindow.h"
#include "SimulatedTeensyHardwareManager.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include "Logging.h"
#include "defines.h"

bool HardwareSimWindow::initialize() {
    window = SDL_CreateWindow("Hardware Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Failed to create hardware sim window: %s\n", SDL_GetError());
        return false;
    }
    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        printf("Failed to create GL context: %s\n", SDL_GetError());
        return false;   
    }
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

void HardwareSimWindow::shutdown() {
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (gl_context) {
        SDL_GL_DeleteContext(gl_context);
        gl_context = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}


void HardwareSimWindow::renderFrame(SimulatedTeensyHardwareState* state) {
    int display_w, display_h;

    SDL_GL_MakeCurrent(window, gl_context);

    SDL_GL_GetDrawableSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(960, 640), ImGuiCond_FirstUseEver);


    ImGui::Begin("Hardware Simulator");

    // Rotary Encoder
    ImGui::Text("Rotary Encoder");
    ImGui::SliderInt("Position", &state->encoderPosition, 0, 127);
    ImGui::Checkbox("Encoder Pressed", &state->encoderPressed);
    ImGui::Separator();

    // Potentiometers Grid
    ImGui::Text("Potentiometers");
    ImGui::BeginChild("PotsGrid", ImVec2(700, 220));
    for (int row = 0; row < 7; ++row) {
        for (int col = 0; col < 8; ++col) {
            int idx = row * 8 + col;
            char label[8];
            snprintf(label, sizeof(label), "P%02d", idx);
            ImGui::PushID(idx);
            ImGui::VSliderInt(label, ImVec2(20, 100), &state->potentiometers[idx], 0, 1023);
            ImGui::PopID();
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
    ImGui::EndChild();
    //ImGui::SameLine();

    // Buttons with Red and Green LED indicators
    ImGui::BeginChild("Buttons", ImVec2(150, 220));
    for (int i = 0; i < 4; ++i) {
        char label[8];
        snprintf(label, sizeof(label), "B%d", i);
        ImGui::Checkbox(label, &state->buttons[i]);
        ImGui::TextColored(state->redLED[i] ? ImVec4(1,0,0,1) : ImVec4(0.3f,0.3f,0.3f,1), "R");
        ImGui::SameLine();
        ImGui::TextColored(state->greenLED[i] ? ImVec4(0,1,0,1) : ImVec4(0.3f,0.3f,0.3f,1), "G");
        ImGui::Spacing();
    }
    ImGui::EndChild();

    ImGui::End(); // End main window

    ImGui::Render();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}
#endif