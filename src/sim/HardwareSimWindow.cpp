#ifndef TARGET_TEENSY

#include "sim/HardwareSimWindow.h"
#include "sim/SimulatedTeensyHardwareManager.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <cstdio>
#include "Logging.h"
#include "defines.h"

bool HardwareSimWindow::initialize() {
    window = SDL_CreateWindow("Hardware Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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

    // Make the main window take all available space
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::Begin("Hardware Simulator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

    // Rotary Encoder
    ImGui::Text("Rotary Encoder");
    ImGui::BeginGroup();
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderInt(" ", &state->encoderPosition, -1023, 1023);
    ImGui::EndGroup();
    
    ImVec4 activeColor = ImVec4(0.2f, 0.7f, 0.2f, 1.0f);
    if (state->encoderPressed) ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
    ImGui::Button("Encoder Pressed", ImVec2(ImGui::GetFrameHeight() * 3.0f, ImGui::GetFrameHeight() * 1.5f));
    bool encoderActive = ImGui::IsItemActive();
    if (state->encoderPressed) ImGui::PopStyleColor();
    state->encoderPressed = encoderActive;

    ImGui::Separator();

    // Potentiometers Grid + Buttons to the right
    ImGui::Text("Potentiometers");
    float totalWidth = ImGui::GetContentRegionAvail().x;
    float sliderWidth = 20.0f;
    float minSpacing = 2.0f;
    float margin = 24.0f; // Space on each side
    float buttonsWidth = 170.0f; // Width for buttons area
    float usableWidth = totalWidth - 2 * margin - buttonsWidth;
    float spacing = (usableWidth - (16 * sliderWidth)) / 15.0f;
    if (spacing < minSpacing) spacing = minSpacing;
    float usedWidth = 16 * sliderWidth + 15 * spacing;
    float offsetX = margin + ((usableWidth > usedWidth) ? (usableWidth - usedWidth) * 0.5f : 0.0f);
    float startY = ImGui::GetCursorPosY();
    ImGui::BeginGroup(); // Begin potentiometer+button group
    ImGui::BeginGroup(); // Potentiometer group
    for (int row = 0; row < 4; ++row) { // 4 rows of 16
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX);
        for (int col = 0; col < 16; ++col) {
            int idx = row * 16 + col;
            if (idx >= 56) break;
            if (col > 0) ImGui::SameLine(0.0f, spacing);
            ImGui::PushID(idx);
            ImGui::BeginGroup();
            char label[8];
            snprintf(label, sizeof(label), "P%02d", idx);
            ImGui::TextUnformatted(label);
            ImGui::VSliderInt("##slider", ImVec2(sliderWidth, 100), &state->potentiometers[idx], 0, 1023);
            ImGui::EndGroup();
            ImGui::PopID();
        }
        ImGui::NewLine();
    }
    ImGui::EndGroup(); // End potentiometer group
    ImGui::SameLine();
    // Calculate height needed for all buttons (4 groups)
    float buttonGroupHeight = 0.0f;
    for (int i = 0; i < 4; ++i) {
        buttonGroupHeight += ImGui::GetTextLineHeight() * 3 + ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y * 4;
    }
    float extraPadding = 16.0f; // Add extra space for safety
    float buttonsChildHeight = buttonGroupHeight + ImGui::GetStyle().WindowPadding.y * 2 + extraPadding;
    ImGui::BeginChild("Buttons", ImVec2(buttonsWidth, buttonsChildHeight), false);
    float buttonAreaWidth = buttonsWidth;
    float groupWidth = 0.0f;
    // First, measure the widest button group
    float labelWidth = ImGui::CalcTextSize("B0").x;
    float rgWidth = ImGui::CalcTextSize("R").x + ImGui::CalcTextSize("G").x + ImGui::GetStyle().ItemSpacing.x;
    float checkboxWidth = ImGui::GetFrameHeight();
    groupWidth = labelWidth;
    if (rgWidth > groupWidth) groupWidth = rgWidth;
    if (checkboxWidth > groupWidth) groupWidth = checkboxWidth;
    float totalGroupsWidth = groupWidth * 4 + ImGui::GetStyle().ItemSpacing.x * 3;
    float startX = (buttonAreaWidth > totalGroupsWidth) ? (buttonAreaWidth - totalGroupsWidth) * 0.5f : 0.0f;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + startX);
    float maxButtonGroupHeight = 0.0f;
    // First, measure the tallest button group
    for (int i = 0; i < 4; ++i) {
        float groupHeight = ImGui::GetTextLineHeight() * 3 + ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y * 4;
        if (groupHeight > maxButtonGroupHeight) maxButtonGroupHeight = groupHeight;
    }
    float baseY = ImGui::GetCursorPosY();
    for (int i = 0; i < 4; ++i) {
        if (i > 0) ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.x);
        ImGui::SetCursorPosY(baseY); // Align all button groups to the same Y
        char label[8];
        snprintf(label, sizeof(label), "B%d", i);
        ImGui::BeginGroup();
        ImGui::TextUnformatted(label); // Header above
        ImGui::TextColored(state->redLED[i] ? ImVec4(1,0,0,1) : ImVec4(0.3f,0.3f,0.3f,1), "R");
        ImGui::SameLine();
        ImGui::TextColored(state->greenLED[i] ? ImVec4(0,1,0,1) : ImVec4(0.3f,0.3f,0.3f,1), "G");
        ImGui::NewLine();
        char button_id[16];
        snprintf(button_id, sizeof(button_id), "B%d", i);
        if (state->buttons[i]) ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
        ImGui::Button(button_id, ImVec2(ImGui::GetFrameHeight() * 1.5f, ImGui::GetFrameHeight() * 1.5f));
        bool isActive = ImGui::IsItemActive();
        if (state->buttons[i]) ImGui::PopStyleColor();
        state->buttons[i] = isActive;
        ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::EndGroup(); // End potentiometer+button group

    ImGui::End(); // End main window

    // Handle keyboard shortcuts for buttons: U/I/O/P toggle B0/B1/B2/B3, Y toggles encoder pressed
    ImGuiIO& io = ImGui::GetIO();
    struct KeyButtonMap { ImGuiKey key; int buttonIdx; };
    static const KeyButtonMap keyMap[] = {
        { ImGuiKey_U, 0 },
        { ImGuiKey_I, 1 },
        { ImGuiKey_O, 2 },
        { ImGuiKey_P, 3 },
    };
    for (const auto& mapping : keyMap) {
        state->buttons[mapping.buttonIdx] = ImGui::IsKeyDown(mapping.key);
    }
    // Y key is momentary for encoder pressed
    if(state->encoderPressed == false)
        state->encoderPressed = ImGui::IsKeyDown(ImGuiKey_Y);

    ImGui::Render();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

void HardwareSimWindow::setWindowPosition(int x, int y) {
    if (window) SDL_SetWindowPosition(window, x, y);
}
#endif