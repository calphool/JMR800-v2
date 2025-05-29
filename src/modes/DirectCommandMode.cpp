#include "modes/DirectCommandMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"



void DirectCommandMode::onEnter() {
    log(LOG_VERBOSE, "Entering Direct Command Mode");
}

void DirectCommandMode::onExit() {
    log(LOG_VERBOSE, "Exiting Direct Command Mode");
}

void DirectCommandMode::loop() {
    if(hardware.buttonStateChanged(0, true, true)) {
        screen.advanceActiveControl();
    }
}

void DirectCommandMode::showScreen() {
    screenManager.setScreen(&screen);
}
