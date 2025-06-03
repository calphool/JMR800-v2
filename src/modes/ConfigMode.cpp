#include "modes/ConfigMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"


void ConfigMode::onEnter() {
    log(LOG_VERBOSE, "Entering System Config Mode");
}

void ConfigMode::onExit() {
    log(LOG_VERBOSE, "Exiting System Config Mode");
}

void ConfigMode::loop() {
    uint active_knob = hardware.getEncoderZeroTo(NUM_KNOBS);

    theConfigScreen.highlightActiveKnob(active_knob);

    if(hardware.encoderSwitchStateChanged(true, true )) {
        theConfigScreen.changeScreenMode(active_knob);
    }
}

void ConfigMode::showScreen() {
    screenManager.setScreen(&theConfigScreen);
}
