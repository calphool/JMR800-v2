/**
 * @file Logging.cpp
 * @brief Implements a simple logging utility for emitting serial debug messages with verbosity filtering.
 *
 * Supports verbosity levels (VERBOSE, INFO, WARNING, ERROR) and defers serial initialization
 * to avoid early startup issues. Also uses visual feedback via LEDs if serial is unavailable.
 */

#include "Logging.h"
#include "HardwareInterface.h"

/// Global timestamp of the last attempt to initialize the Serial interface.
unsigned long lastSerialInitAttemptMillis = 0;

extern HardwareInterface* hardware;


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

    Serial.print(_func_name);
    Serial.print(": ");

    Serial.println(message);
}


/**
 * @brief Convenience overload to log a message at the default `LOG_INFO` level.
 *
 * @param message The message to be logged.
 */
void log(const char* message, const char* _func_name) {
    log(LOG_VERBOSE, message, _func_name);
} 