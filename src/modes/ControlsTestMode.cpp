#include "modes/ControlsTestMode.h"
#include "ScreenManager.h"
#include "Logging.h"


void ControlsTestMode::onEnter() {
    log(LOG_VERBOSE, "Entering Controls Test Mode");
}

void ControlsTestMode::onExit() {
    log(LOG_VERBOSE, "Exiting Controls Test Mode");
}

void ControlsTestMode::loop() {
    // Add control diagnostics or monitoring if needed
}

void ControlsTestMode::showScreen() {
    screenManager.setScreen(&screen);
}
