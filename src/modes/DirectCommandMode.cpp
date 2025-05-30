#include "modes/DirectCommandMode.h"
#include "ScreenManager.h"
#include "Logging.h"
#include "HardwareManager.h"
#include "widgets/Widget.h"



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

    if(hardware.encoderSwitchStateChanged(true, true)) {
        Widget* w = screen.getActiveWidget();
        if(w != NULL) {
            if(w->getType() == WidgetType::PushButton) {
                hardware.sendParameter(screen.getCmdValue(), screen.getByteValue());
            }
        }
    }
}

void DirectCommandMode::showScreen() {
    screenManager.setScreen(&screen);
}
