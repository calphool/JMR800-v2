#include "modes/RunMode.h"
#include "ScreenManager.h"
#include "Logging.h"


void RunMode::onEnter() {
    log(LOG_VERBOSE, "Entering Run Mode");
}

void RunMode::onExit() {
    log(LOG_VERBOSE, "Exiting Run Mode");
}

void RunMode::loop() {
    // Insert mode-specific polling or logic here
}


void RunMode::showScreen() {
    screenManager.setScreen(&screen);
}


