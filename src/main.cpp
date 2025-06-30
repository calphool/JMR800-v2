/**
 * @file main.cpp
 * @brief Entry point for the JMR800-V2 firmware.
 *
 * This file initializes the core subsystems of the JMR800 controller,
 * including hardware abstraction, screen management, and mode handling.
 * It defines the main setup and loop routines required by the Arduino runtime.
 * * The firmware supports multiple modes of operation, such as Run Mode,
 * Controls Test Mode, Direct Command Mode, and Configuration Mode.
 * 
 * This file is part of the JMR800-V2 project, which is an open-source project
 * designed to provide a flexible and extensible platform for controlling
 * various hardware components using a graphical interface.
 * 
 * @author Joe Rounceville
 * @date 2025-06-04
 */

#ifdef TARGET_TEENSY
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "TeensyHardwareManager.h"
#include "SH110XDisplay.h"
#else
#include <SDL2/SDL.h>
#include "sim/SimulatedTeensyHardwareManager.h"
#include "sim/SDLDisplay.h"
#include "sim/HardwareSimWindow.h"
#include <iostream>
#endif

#include <stdarg.h>
#include "defines.h"
#include "ScreenManager.h"
#include "screens/RunModeScreen.h"
#include "Logging.h"
#include "IHardwareManager.h"

#include "ModeManager.h"
#include "modes/RunMode.h"
#include "modes/ControlsTestMode.h" 
#include "modes/DirectCommandMode.h"
#include "modes/ConfigMode.h"

IHardwareManager* hardware = nullptr;
RunMode runMode;
ControlsTestMode testMode;
DirectCommandMode directMode;
ConfigMode configMode;

/**
 * @brief Initializes core subsystems and configures operational modes.
 *
 * Called once at startup. This routine initializes:
 * - The hardware abstraction layer (`hardware.init()`)
 * - The screen display system (`screenManager.init()`)
 * - The application mode manager, including:
 *   - Run Mode
 *   - Controls Test Mode
 *   - Direct Command Mode
 *   - Configuration Mode
 *
 * After registering all modes, the mode manager is initialized to activate
 * the appropriate entry point for the application.
 */
void setup() {
  log(LOG_VERBOSE, "Inside main->setup()", __func__);

#ifdef TARGET_TEENSY
    hardware = new TeensyHardwareManager();
    hardware->init();
    screenManager.init(new SH110XDisplay());
#else
    // hardware is already assigned in main() for simulation; just initialize it
    hardware->init();
#endif
  modeManager.addMode(&runMode);
  modeManager.addMode(&testMode);
  modeManager.addMode(&directMode);
  modeManager.addMode(&configMode);
  modeManager.init();
}


/**
 * @brief Executes the main program loop for the JMR800 controller.
 *
 * Called repeatedly by the Arduino runtime. This routine sequentially
 * runs the periodic update logic for:
 * - Hardware monitoring and input polling
 * - Mode-specific behavior execution
 * - Screen updates and redraws
 */
void loop() {
  hardware->loop();
  modeManager.loop();
  screenManager.loop();
}

#ifndef TARGET_TEENSY

int main(int argc, char* argv[]) {
    SDLDisplay screenDisplay;
    HardwareSimWindow hwWindow;
    SimulatedTeensyHardwareManager* simManager = new SimulatedTeensyHardwareManager();
    hardware = simManager;

    if (!screenDisplay.initialize()) {
        std::cerr << "Failed to initialize SDLDisplay." << std::endl;
        return -1;
    }

    if (!hwWindow.initialize()) {
        std::cerr << "Failed to initialize HardwareSimWindow." << std::endl;
        return -1;
    }

    // Center both windows horizontally on the screen
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int screenW = dm.w;
    int screenH = dm.h;

    int displayWidth = 128 * 6; // SDLDisplay: 128x64, pixelSize=6
    int displayHeight = 64 * 6;
    int simWidth = 800; // HardwareSimWindow width
    int simHeight = 700;

    int totalWidth = displayWidth + simWidth;
    int startX = (screenW - totalWidth) / 2;
    int centerY = (screenH - ((displayHeight > simHeight) ? displayHeight : simHeight)) / 2;

    screenDisplay.setWindowPosition(startX, centerY);
    hwWindow.setWindowPosition(startX + displayWidth, centerY);

    screenManager.init(&screenDisplay);
    setup();

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            screenDisplay.handleEvent(event);
            hwWindow.handleEvent(event);
        }

        loop();

        screenDisplay.renderFrame();
        hwWindow.renderFrame(simManager->getState());

        if (screenDisplay.shouldClose() || hwWindow.shouldClose()) {
            running = false;
        }
    }

    screenDisplay.shutdown();
    hwWindow.shutdown();
    delete simManager;
    return 0;
}
#endif
