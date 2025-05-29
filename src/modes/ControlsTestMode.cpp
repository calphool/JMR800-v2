#include "modes/ControlsTestMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"


void ControlsTestMode::onEnter() {
    log(LOG_VERBOSE, "Entering Controls Test Mode");
}

void ControlsTestMode::onExit() {
    log(LOG_VERBOSE, "Exiting Controls Test Mode");
    for(int i=0; i < NUM_BUTTONS; i++)
        hardware.setButtonLights(i, false, false);
}

void ControlsTestMode::loop() {
    for(int i=0;i < NUM_BUTTONS; i++) {
        if(hardware.buttonStateChanged(i, true, true)) {
            if(hardware.redIsLit(i) == false && hardware.greenIsLit(i) == false) {
                hardware.setButtonLights(i, true, false);
            }
            else
            if(hardware.redIsLit(i) == true && hardware.greenIsLit(i) == false) {
                hardware.setButtonLights(i, true, true);
            }
            else
            if(hardware.redIsLit(i) == true && hardware.greenIsLit(i) == true) {
                hardware.setButtonLights(i, false, false);
            }
        }
    }
}

void ControlsTestMode::showScreen() {
    screenManager.setScreen(&screen);
}
