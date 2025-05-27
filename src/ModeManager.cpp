#include "ModeManager.h"
#include "HardwareManager.h"
#include "Logging.h"


ModeManager modeManager;


void ModeManager::addMode(AppMode* mode) {
    modes.push_back(mode);
}

void ModeManager::init() {
    if (!modes.empty()) {
      modes[0]->onEnter();
      modes[0]->showScreen();  // Let the mode decide its screen
    }
}

void ModeManager::loop() {
    bool currentButton = hardware.isButtonPressed(3);  // Button 4 (index 3)

    if (currentButton && !prevButtonState && !modes.empty()) {
      sprintf(buffer_64, "Cycling from %s to %s", modes[currentIndex]->name(), 
        modes[(currentIndex + 1) % modes.size()]->name());
      log(LOG_INFO, buffer_64);

      modes[currentIndex]->onExit();
      currentIndex = (currentIndex + 1) % modes.size();
      modes[currentIndex]->onEnter();
      modes[currentIndex]->showScreen();  // Let the mode present its screen
    }

    prevButtonState = currentButton;

    if (!modes.empty()) {
        modes[currentIndex]->loop();
    }
}

AppMode* ModeManager::currentMode() const {
    return modes.empty() ? nullptr : modes[currentIndex];
}

