#include <Arduino.h>
#include <stdarg.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "defines.h"
#include "HardwareManager.h"
#include "ScreenManager.h"
#include "RunModeScreen.h"
#include "Logging.h"

#include "ModeManager.h"
#include "RunMode.h"
#include "ControlsTestMode.h"
#include "DirectCommandMode.h"
#include "ConfigMode.h"


ModeManager modeManager;
RunMode runMode;
ControlsTestMode testMode;
DirectCommandMode directMode;
ConfigMode configMode;

                                                                                            // SDA = 18, SCL = 19
void setup() {
  if(!Serial) {
    Serial.begin(9600);
    while (!Serial && millis() < 1000);  // wait for USB connection for 1 second
  }
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
