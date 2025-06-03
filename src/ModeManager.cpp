#include "ModeManager.h"
#include "HardwareManager.h"
#include "Logging.h"


ModeManager modeManager;

/* --------------------------------------------------------------
   |  addMode() -- adds another mode class to the mode manager  |
   -------------------------------------------------------------- */
void ModeManager::addMode(AppMode* mode) {
    modes.push_back(mode);
}

/* --------------------------------------------------------------
   |  init() -- initializes the modeManager                     |
   -------------------------------------------------------------- */
void ModeManager::init() {
    if (!modes.empty()) {
      modes[0]->onEnter();
      modes[0]->showScreen();  // Let the mode decide its screen
    }
}

/* --------------------------------------------------------------------------------
   |  loop() -- watches for button 3 and 4 pressed simultaneously                 |
   |  if it sees that, it advances to the next mode                               |
   -------------------------------------------------------------------------------- */
void ModeManager::loop() {
    // Get current state of both buttons
    bool button2 = hardware.isButtonPressed(2);  // Button 3 (index 2)
    bool button3 = hardware.isButtonPressed(3);  // Button 4 (index 3)

    // New condition: both must be pressed, and were not pressed together last time
    bool currentCombo = button2 && button3;

    if (currentCombo && !prevButtonState && !modes.empty()) {
        sprintf(buffer_64, "Cycling from %s to %s", modes[currentIndex]->name(), 
            modes[(currentIndex + 1) % modes.size()]->name());
        log(LOG_INFO, buffer_64);


        modes[currentIndex]->onExit();
        currentIndex = (currentIndex + 1) % modes.size();
        hardware.loadKnobs(); // might make this conditional based on mode

        modes[currentIndex]->onEnter();
        modes[currentIndex]->showScreen();  // Let the mode present its screen
    }

    // Update previous button combination state
    prevButtonState = currentCombo;

    if (!modes.empty()) {
        modes[currentIndex]->loop();
    }
}


AppMode* ModeManager::currentMode() const {
    return modes.empty() ? nullptr : modes[currentIndex];
}

