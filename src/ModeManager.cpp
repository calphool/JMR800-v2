/**
 * @file ModeManager.cpp
 * @brief Implements the ModeManager class responsible for managing application modes.
 *
 * The ModeManager maintains a list of `AppMode` instances, handles transitions between them
 * based on hardware input, and delegates lifecycle events such as `onEnter()`, `onExit()`, and `loop()`.
 */

#include "ModeManager.h"
#include "HardwareInterface.h"
#include "Logging.h"
#include <stdio.h>

/// Global singleton instance of the ModeManager.
ModeManager modeManager;

extern HardwareInterface* hardware;

/**
 * @brief Adds an application mode to the internal mode list.
 *
 * This method should be called during setup to register all available operational modes
 * (e.g., Run Mode, Config Mode, etc.).
 *
 * @param mode Pointer to an `AppMode` instance to register.
 */
void ModeManager::addMode(AppMode* mode) {
    log(LOG_VERBOSE, "inside ModeManager->addMode()", __func__);

    modes.push_back(mode);
}

/**
 * @brief Initializes the mode system and enters the first registered mode.
 *
 * This method calls `onEnter()` and `showScreen()` on the first mode if at least one mode is registered.
 */
void ModeManager::init() {
    log(LOG_VERBOSE, "inside ModeManager->init()", __func__);

    if (!modes.empty()) {
      modes[0]->onEnter();
      modes[0]->showScreen();  // Let the mode decide its screen
    }
}


/**
 * @brief Monitors mode-switching input and runs the active mode loop.
 *
 * If buttons 3 and 4 are pressed simultaneously (rising edge), the ModeManager exits the
 * current mode and transitions to the next registered mode in a circular sequence.
 * After switching, it calls the new mode's `onEnter()` and `showScreen()` methods.
 * Then, regardless of mode switching, it invokes the current mode's `loop()` method.
 */
void ModeManager::loop() {
    // Get current state of both buttons
    bool button2 = hardware->isButtonPressed(2);  // Button 3 (index 2)
    bool button3 = hardware->isButtonPressed(3);  // Button 4 (index 3)

    // New condition: both must be pressed, and were not pressed together last time
    bool currentCombo = button2 && button3;

    if (currentCombo && !prevButtonState && !modes.empty()) {
        sprintf(buffer_64, "Cycling from %s to %s", modes[currentIndex]->name(), 
            modes[(currentIndex + 1) % modes.size()]->name());
        log(LOG_INFO, buffer_64, __func__);


        modes[currentIndex]->onExit();
        currentIndex = (currentIndex + 1) % modes.size();
        hardware->loadKnobs(); // might make this conditional based on mode

        modes[currentIndex]->onEnter();
        modes[currentIndex]->showScreen();  // Let the mode present its screen
    }

    // Update previous button combination state
    prevButtonState = currentCombo;

    if (!modes.empty()) {
        modes[currentIndex]->loop();
    }
}

/**
 * @brief Returns a pointer to the currently active application mode.
 *
 * @return AppMode* Currently active mode, or nullptr if no modes are registered.
 */
AppMode* ModeManager::currentMode() const {
    log(LOG_VERBOSE, "inside ModeManager->currentMode()", __func__);   
    return modes.empty() ? nullptr : modes[currentIndex];
}

