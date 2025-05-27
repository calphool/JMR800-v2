#include "modes/ConfigMode.h"
#include "ScreenManager.h"
#include "Logging.h"


void ConfigMode::onEnter() {
    log(LOG_VERBOSE, "Entering System Config Mode");
}

void ConfigMode::onExit() {
    log(LOG_VERBOSE, "Exiting System Config Mode");
}

void ConfigMode::loop() {
    // Background logic for config changes or timeouts
}

void ConfigMode::showScreen() {
    screenManager.setScreen(&screen);
}
