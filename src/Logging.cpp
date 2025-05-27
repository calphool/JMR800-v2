#include "Logging.h"
#include "HardwareManager.h"

unsigned long lastSerialInitAttemptMillis = 0;

/* -------------------------------------------------------------------------------------------------------------------- 
   |  log -- logs a message to the Serial console with the given verbosity level (VERBOSE, INFO, WARNING, ERROR)      |
   |  if it meets or exceeds the global LOG_LEVEL_THRESHOLD                                                           |
   -------------------------------------------------------------------------------------------------------------------- */
void log(LogLevel level, const String& message) {
    if (level < LOG_LEVEL_THRESHOLD) return;

    if(!Serial) {                      // if we haven't initialized the Serial object, turn on the system's LEDs, 
        unsigned long now = millis();  // initialize it, and then turn them off keep rechecking every 30 seconds if it remains undefined
        if (now - lastSerialInitAttemptMillis >= 30000 || lastSerialInitAttemptMillis == 0) {
            lastSerialInitAttemptMillis = now;
            for(uint8_t i = 0; i < NUM_BUTTONS; i++) hardware.setButtonLights(i, true, true);
            Serial.begin(9600);
            for(uint8_t i = 0; i < NUM_BUTTONS; i++) hardware.setButtonLights(i, false, false);
        }
    }

    switch (level) {
        case LOG_VERBOSE:
            Serial.print("[VERBOSE] ");
            break;
        case LOG_INFO:
            Serial.print("[INFO] ");
            break;
        case LOG_WARNING:
            Serial.print("[WARN] ");
            break;
        case LOG_ERROR:
            Serial.print("[ERROR] ");
            break;
    }

    Serial.println(message);
}


/* --------------------------------------------------------------
   |  log -- convenience overload that logs a message at the     |
   |  default LOG_INFO level                                     |
   -------------------------------------------------------------- */
void log(const String& message) {
    log(LOG_INFO, message);
}