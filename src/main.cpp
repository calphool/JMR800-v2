#include <Arduino.h>
#include <stdarg.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "defines.h"
#include "HardwareManager.h"
#include "ScreenManager.h"
#include "screens/RunModeScreen.h"
#include "Logging.h"

#include "ModeManager.h"
#include "modes/RunMode.h"
#include "modes/ControlsTestMode.h"
#include "modes/DirectCommandMode.h"
#include "modes/ConfigMode.h"


RunMode runMode;
ControlsTestMode testMode;
DirectCommandMode directMode;
ConfigMode configMode;

                                                                                            
void setup() {
  log(LOG_VERBOSE, "Inside main->setup()");

  hardware.init();
  screenManager.init();
  modeManager.addMode(&runMode);
  modeManager.addMode(&testMode);
  modeManager.addMode(&directMode);
  modeManager.addMode(&configMode);
  modeManager.init();
}

void loop() {
  hardware.loop();
  modeManager.loop();
  screenManager.loop();
}
