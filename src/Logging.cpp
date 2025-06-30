/**
 * @file Logging.cpp
 * @brief Implements a simple logging utility for emitting serial debug messages with verbosity filtering.
 *
 * Supports verbosity levels (VERBOSE, INFO, WARNING, ERROR) and defers serial initialization
 * to avoid early startup issues. Also uses visual feedback via LEDs if serial is unavailable.
 */

#include "Logging.h"
#include "IHardwareManager.h"

#ifndef TARGET_TEENSY
#include <stdio.h>
#endif

/// Global timestamp of the last attempt to initialize the Serial interface.
unsigned long lastSerialInitAttemptMillis = 0;

extern IHardwareManager* hardware;


void outstr(const char* s) {
#ifdef TARGET_TEENSY
    Serial.print(s);
#else
    printf("%s", s);
#endif
}

/**
 * @brief Logs a message to the serial console with a specified verbosity level.
 *
 * If the verbosity level is equal to or higher than `LOG_LEVEL_THRESHOLD`, the message is formatted
 * with a prefix (e.g., `[INFO]`) and printed to the Serial console. If Serial is not yet initialized,
 * the method attempts to initialize it every 300 seconds and provides LED-based feedback.
 *
 * @param level LogLevel to categorize the message (e.g., LOG_INFO, LOG_ERROR).
 * @param message The message to be logged.
 */
void log(LogLevel level, const char* message, const char* _func_name) {
    if (level < LOG_LEVEL_THRESHOLD) return;

#ifdef TARGET_TEENSY
    if(!Serial) {                      // if we haven't initialized the Serial object, turn on the system's LEDs, 
        unsigned long now = millis();  // initialize it, and then turn them off keep rechecking every 300 seconds if it remains undefined
        if (now - lastSerialInitAttemptMillis >= 300000 || lastSerialInitAttemptMillis == 0) {
            lastSerialInitAttemptMillis = now;
            hardware->saveLedState();
            for(uint8_t i = 0; i < NUM_BUTTONS; i++) hardware->setButtonLights(i, true, true);
            Serial.begin(9600);
            for(uint8_t i = 0; i < NUM_BUTTONS; i++) hardware->setButtonLights(i, false, false);
            hardware->restoreLedState();
        }
    }
#endif

    switch (level) {
        case LOG_VERBOSE:
            outstr("[VERBOSE] ");
            break;
        case LOG_INFO:
            outstr("[INFO] ");
            break;
        case LOG_WARNING:
            outstr("[WARN] ");
            break;
        case LOG_ERROR:
            outstr("[ERROR] ");
            break;
    }

    outstr(_func_name);
    outstr(": ");

    outstr(message);
    outstr("\n");
}


/**
 * @brief Convenience overload to log a message at the default `LOG_INFO` level.
 *
 * @param message The message to be logged.
 */
void log(const char* message, const char* _func_name) {
    log(LOG_VERBOSE, message, _func_name);
} 