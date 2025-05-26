#include "DirectCommandMode.h"
#include "ScreenManager.h"
#include "Logging.h"



void DirectCommandMode::onEnter() {
    log(LOG_VERBOSE, "Entering Direct Command Mode");
}

void DirectCommandMode::onExit() {
    log(LOG_VERBOSE, "Exiting Direct Command Mode");
}

void DirectCommandMode::loop() {
    // Handle command queue or live text entry
}

void DirectCommandMode::showScreen() {
    screenManager.setScreen(&screen);
}
