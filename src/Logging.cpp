#include "Logging.h"


/* --------------------------------------------------------------
   |  log -- logs a message to the Serial console with the       |
   |  given verbosity level (VERBOSE, INFO, WARNING, ERROR)      |
   |  if it meets or exceeds the global LOG_LEVEL_THRESHOLD      |
   -------------------------------------------------------------- */
void log(LogLevel level, const String& message) {
    if (level < LOG_LEVEL_THRESHOLD) return;

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
 
